#include "..\include\ObjFilter.h"

//System
#include <string>
#include <fstream>
#include <sstream>

namespace DamonsIO {
	bool ObjFilter::canLoadExtension(const std::string& upperCaseExt) const
	{
		return (upperCaseExt == "OBJ");
	}

	DAMONS_FILE_ERROR ObjFilter::saveToFile(DMeshLib::ModelObject* entity, const std::string& filename, const SaveParameters& parameters)
	{
		if (!entity)
			return CC_FERR_BAD_ARGUMENT;

		if (!entity->isA(DMeshLib::DB_TYPES::MESH))
			return CC_FERR_BAD_ENTITY_TYPE;

		DMeshLib::MeshModel* mesh = static_cast<DMeshLib::MeshModel*>(entity);
		if (!mesh || mesh->getTriangleNumber() == 0)
		{
			return CC_FERR_NO_SAVE;
		}
		
		unsigned nbPoints = mesh->getPointsNumber();
		unsigned numberOfTriangles = mesh->getTriangleNumber();

		
		std::ofstream ostream(filename);
		ostream << "# " << "create by damons" << std::endl;

		DMeshLib::data_type x, y, z;
		for (unsigned i = 0; i < nbPoints; ++i)
		{
			mesh->getPoint(i, x, y, z);
			ostream << "v " << x << " " << y << " " << z << std::endl;
		}

		//normals
		bool withNormals = mesh->hasNormals();
		if (withNormals)
		{
			for (unsigned i = 0; i < nbPoints; ++i)
			{
				mesh->getNormal(i, x, y, z);
				ostream << "vn " << x << " " << y << " " << z << std::endl;
			}
		}

		//mesh or sub-meshes
		
		unsigned triNum = mesh->getTriangleNumber();
		DMeshLib::index_type id1, id2, id3;

		for (unsigned i = 0; i < triNum; ++i)
		{
			mesh->getTriangleIndex(i, id1, id2, id3);
			
			ostream << "f";
			if (withNormals)
			{
// 				if (withTexCoordinates)
// 				{
// 					stream << " " << i1 << "/" << t1 << "/" << n1;
// 					stream << " " << i2 << "/" << t2 << "/" << n2;
// 					stream << " " << i3 << "/" << t3 << "/" << n3;
// 				}
// 				else
				{
					ostream << " " << id1 << "//" << id1;
					ostream << " " << id2 << "//" << id2;
					ostream << " " << id3 << "//" << id3;
				}
			}
			else
			{
// 				if (withTexCoordinates)
// 				{
// 					stream << " " << i1 << "/" << t1;
// 					stream << " " << i2 << "/" << t2;
// 					stream << " " << i3 << "/" << t3;
// 				}
// 				else
				{
					ostream << " " << id1;
					ostream << " " << id2;
					ostream << " " << id3;
				}
			}
			ostream << std::endl;
		}

		ostream << "#" << triNum << " faces" << std::endl;

		return CC_FERR_NO_ERROR;
	}

	//! Updates point index to a global index starting from 0!
	static bool UpdatePointIndex(int& vIndex, int maxIndex)
	{
		if (vIndex == 0 || -vIndex > maxIndex)
			return false;
		vIndex = (vIndex > 0 ? vIndex - 1 : maxIndex + vIndex);
		return true;
	}

	//! OBJ facet ('f') element
	struct facetElement
	{
		//! A set of indexes (vertex, texture coordinates and normal)
		union
		{
			struct
			{
				int vIndex;		//vertex index
				int tcIndex;	//texture coordinate index
				int nIndex;		//normal index
			};
			int indexes[3];
		};

		//! Default constructor
		facetElement()
			: vIndex(0)
			, tcIndex(0)
			, nIndex(0)
		{
		}

		//! Updates point index to a global index starting from 0!
		inline bool updatePointIndex(int maxIndex)
		{
			return UpdatePointIndex(vIndex, maxIndex);
		}

		//! Updates tex coord index to a global index starting from 0!
		inline bool updateTexCoordIndex(int maxIndex)
		{
			if (-tcIndex > maxIndex)
				return false;
			//if tcIndex == 0 then we return '-1'
			tcIndex = (tcIndex >= 0 ? tcIndex - 1 : maxIndex + tcIndex);
			return true;
		}

		//! Updates normal index to a global index starting from 0!
		inline bool updateNormalIndex(int maxIndex)
		{
			if (-nIndex > maxIndex)
				return false;
			//if nIndex == 0 then we return '-1'
			nIndex = (nIndex >= 0 ? nIndex - 1 : maxIndex + nIndex);
			return true;
		}
	};

	DAMONS_FILE_ERROR ObjFilter::loadFile(const std::string& filename, DMeshLib::ModelObject *&container, LoadParameters& parameters)
	{
		//open file
		std::ifstream fin(filename);
		if(!fin.is_open())
			return CC_FERR_READING;

		int pointsRead = 0;

		//facets
		unsigned int facesRead = 0;
		unsigned int totalFacesRead = 0;
		int maxVertexIndex = -1;

		//base mesh
		DMeshLib::MeshModel* mesh = new DMeshLib::MeshModel;
		mesh->setName("obj_mesh");

		//groups (starting index + name)
		std::vector<std::pair<unsigned, std::string> > groups;

		//materials
		//ccMaterialSet* materials = 0;
		bool hasMaterial = false;
		int currentMaterial = -1;
		bool currentMaterialDefined = false;
		bool materialsLoadFailed = true;

		//texture coordinates
		//TextureCoordsContainer* texCoords = 0;
		bool hasTexCoords = false;
		int texCoordsRead = 0;
		int maxTexCoordIndex = -1;

		//normals
		//NormsIndexesTableType* normals = 0;
		int normsRead = 0;
		bool normalsPerFacet = false;
		int maxTriNormIndex = -1;
 
		//common warnings that can appear multiple time (we avoid to send too many messages to the console!)
		enum OBJ_WARNINGS {
			INVALID_NORMALS = 0,
			INVALID_INDEX = 1,
			NOT_ENOUGH_MEMORY = 2,
			INVALID_LINE = 3,
			CANCELLED_BY_USER = 4,
		};
		bool objWarnings[5] = { false, false, false, false, false };
		bool error = false;

		try
		{
			unsigned lineCount = 0;
			unsigned polyCount = 0;
			std::string currentLine , commentstring, keyword;
			while (!fin.eof() && std::getline(fin, currentLine))
			{
				++lineCount;

				std::stringstream stringstream(currentLine);
				stringstream.unsetf(std::ios_base::skipws);

				stringstream >> std::ws;
				if (stringstream.eof()) {
					commentstring = commentstring + currentLine + "\n";
					continue;
				}
				else if (stringstream.peek() == '#') {
					commentstring = commentstring + currentLine + "\n";
					continue;
				}

				stringstream >> keyword;
				/*** new vertex ***/
				if (keyword == "v")
				{
					DMeshLib::data_type x, y, z;
					char whitespace_v_x, whitespace_x_y, whitespace_y_z;
					stringstream >> whitespace_v_x >> std::ws >> x >> whitespace_x_y >> std::ws >> y >> whitespace_y_z >> std::ws >> z >> std::ws;
					//malformed line?
					if (!stringstream.eof() || !isspace(whitespace_v_x) || !isspace(whitespace_x_y) || !isspace(whitespace_y_z)) {
						objWarnings[INVALID_LINE] = true;
						error = true;
						break;
					}
					
					//shifted point
					mesh->addPoint(x, y, z);
					++pointsRead;
				}
				/*** new vertex texture coordinates ***/
				else if (keyword == "vt")
				{
					//create and reserve memory for tex. coords container if necessary
					/*if (!texCoords)
					{
						texCoords = new TextureCoordsContainer();
						texCoords->link();
					}

					//malformed line?
					if (tokens.size() < 2)
					{
						objWarnings[INVALID_LINE] = true;
						error = true;
						break;
					}

					TexCoords2D T(tokens[1].toFloat(), 0);

					if (tokens.size() > 2) //OBJ specification allows for only one value!!!
					{
						T.ty = tokens[2].toFloat();
					}

					texCoords->addElement(T);*/
					++texCoordsRead;
				}
				/*** new vertex normal ***/
				else if (keyword == "vn") //--> in fact it can also be a facet normal!!!
				{
					DMeshLib::data_type x, y, z;
					char whitespace_vn_x, whitespace_x_y, whitespace_y_z;
					stringstream >> whitespace_vn_x >> std::ws >> x >> whitespace_x_y >> std::ws >> y >> whitespace_y_z >> std::ws >> z >> std::ws;
			
					if (!stringstream.eof() || !isspace(whitespace_vn_x) || !isspace(whitespace_x_y) || !isspace(whitespace_y_z)) {
						objWarnings[INVALID_LINE] = true;
						error = true;
						break;
					}

					
					mesh->addNormal(x, y, z);
					++normsRead;
				}
				/*** new group ***/
				else if (keyword == "g" || keyword == "o")
				{
					//update new group index
					facesRead = 0;
					//get the group name
					char whitespace_mtllib_group_name = ' ';
					if (!stringstream.eof()) {
						stringstream >> whitespace_mtllib_group_name >> std::ws;
					}
					if (!stringstream || !isspace(whitespace_mtllib_group_name)) {
						objWarnings[INVALID_LINE] = true;
						error = true;
						break;
					}
					std::string group_name;
					if (stringstream.eof()) {
						group_name = "default";
					}
					else
					{
						stringstream >> group_name >> std::ws;
						if (!stringstream || !stringstream.eof()) {
							objWarnings[INVALID_LINE] = true;
							error = true;
							break;
						}
					}
					//push previous group descriptor (if none was pushed)
					if (groups.empty() && totalFacesRead > 0)
						groups.emplace_back(0, "default");
					//push new group descriptor
					if (!groups.empty() && groups.back().first == totalFacesRead)
						groups.back().second = group_name; //simply replace the group name if the previous group was empty!
					else
						groups.emplace_back(totalFacesRead, group_name);
					polyCount = 0; //restart polyline count at 0!
				}
				/*** new face ***/
				else if ((keyword == "f") || (keyword == "fo"))
				{
					
					stringstream >> std::ws;
					std::string token;
					std::vector<std::string> tokens;
					while ( stringstream>> token >> std::ws)
					{
						tokens.push_back(token);
					}

					if(token.size())
						tokens.push_back(token);
					//read the face elements (singleton, pair or triplet)
					std::vector<facetElement> currentFace;
					{
						currentFace.reserve(tokens.size());
						for (int i = 0; i < tokens.size(); ++i)
						{
							std::stringstream ss(tokens[i]);
							std::string item;
							std::vector<std::string> elems;
							while (std::getline(ss, item, '/')) {
								elems.push_back(item);							
							}

							if (elems.size() == 0 || elems[0].empty())
							{
								objWarnings[INVALID_LINE] = true;
								error = true;
								break;
							}
							else
							{
								//new vertex
								facetElement fe; //(0,0,0) by default

								fe.vIndex = std::stoi(elems[0]);
								if (elems.size() > 1 && !elems[1].empty())
									fe.tcIndex = std::stoi(elems[1]);
								if (elems.size() > 2 && !elems[2].empty())
									fe.nIndex = std::stoi(elems[2]);

								currentFace.push_back(fe);
							}

							std::vector<std::string>().swap(elems);
						}
					}
					std::vector<std::string>().swap(tokens);

					if (error)
						break;

					if (currentFace.size() < 3)
					{
						//("[OBJ] Malformed file: polygon on line %1 has less than 3 vertices!", lineCount);
						error = true;
						break;
					}

					//first vertex
					std::vector<facetElement>::iterator A = currentFace.begin();

					//the very first vertex of the group tells us about the whole sequence
					if (facesRead == 0)
					{
						//we have a tex. coord index as second vertex element!
						if (!hasTexCoords && A->tcIndex != 0 && !materialsLoadFailed)
						{
							hasTexCoords = true;
						}

						//we have a normal index as third vertex element!
						if (!normalsPerFacet && A->nIndex != 0)
						{
							normalsPerFacet = true;
						}
					}

					//we process all vertices accordingly
					for (facetElement& vertex : currentFace)
					{
						//vertex index
						{
							if (!vertex.updatePointIndex(pointsRead))
							{
								objWarnings[INVALID_INDEX] = true;
								error = true;
								break;
							}
							if (vertex.vIndex > maxVertexIndex)
								maxVertexIndex = vertex.vIndex;
						}
						//should we have a tex. coord index as second vertex element?
						if (hasTexCoords && currentMaterialDefined)
						{
							if (!vertex.updateTexCoordIndex(texCoordsRead))
							{
								objWarnings[INVALID_INDEX] = true;
								error = true;
								break;
							}
							if (vertex.tcIndex > maxTexCoordIndex)
								maxTexCoordIndex = vertex.tcIndex;
						}

						//should we have a normal index as third vertex element?
						if (normalsPerFacet)
						{
							if (!vertex.updateNormalIndex(normsRead))
							{
								objWarnings[INVALID_INDEX] = true;
								error = true;
								break;
							}
							if (vertex.nIndex > maxTriNormIndex)
								maxTriNormIndex = vertex.nIndex;
						}
					}

					//don't forget material (common for all vertices)
					if (currentMaterialDefined && !materialsLoadFailed)
					{
						if (!hasMaterial)
						{
							hasMaterial = true;
						}
					}

					if (error)
						break;

					//Now, let's tesselate the whole polygon
					//FIXME: yeah, we do very ulgy tesselation here!
					std::vector<facetElement>::const_iterator B = A + 1;
					std::vector<facetElement>::const_iterator C = B + 1;
					for (; C != currentFace.end(); ++B, ++C)
					{
						//push new triangle
						mesh->addTriangle(A->vIndex, B->vIndex, C->vIndex);
						++facesRead;
						++totalFacesRead;
						/*
						if (hasMaterial)
							baseMesh->addTriangleMtlIndex(currentMaterial);

						if (hasTexCoords)
							baseMesh->addTriangleTexCoordIndexes(A->tcIndex, B->tcIndex, C->tcIndex);

						if (normalsPerFacet)
							baseMesh->addTriangleNormalIndexes(A->nIndex, B->nIndex, C->nIndex);
						*/
					}
				}
				/*** polyline ***/
				else if (keyword == "l")
				{
// 					malformed line?
// 					if (tokens.size() < 3)
// 					{
// 						objWarnings[INVALID_LINE] = true;
// 						currentLine = stream.readLine();
// 						continue;
// 					}
// 
// 					//read the face elements (singleton, pair or triplet)
// 					ccPolyline* polyline = new ccPolyline(vertices);
// 					if (!polyline->reserve(static_cast<unsigned>(tokens.size() - 1)))
// 					{
// 						//not enough memory
// 						objWarnings[NOT_ENOUGH_MEMORY] = true;
// 						delete polyline;
// 						polyline = 0;
// 						currentLine = stream.readLine();
// 						continue;
// 					}
// 
// 					for (int i = 1; i < tokens.size(); ++i)
// 					{
// 						//get next polyline's vertex index
// 						QStringList vertexTokens = tokens[i].split('/');
// 						if (vertexTokens.size() == 0 || vertexTokens[0].isEmpty())
// 						{
// 							objWarnings[INVALID_LINE] = true;
// 							error = true;
// 							break;
// 						}
// 						else
// 						{
// 							int index = vertexTokens[0].toInt(); //we ignore normal index (if any!)
// 							if (!UpdatePointIndex(index, pointsRead))
// 							{
// 								objWarnings[INVALID_INDEX] = true;
// 								error = true;
// 								break;
// 							}
// 
// 							polyline->addPointIndex(index);
// 						}
// 					}
// 
// 					if (error)
// 					{
// 						delete polyline;
// 						polyline = 0;
// 						break;
// 					}
// 
// 					polyline->setVisible(true);
// 					QString name = groups.empty() ? QString("Line") : groups.back().second + QString(".line");
// 					polyline->setName(QString("%1 %2").arg(name).arg(++polyCount));
// 					vertices->addChild(polyline);

				}
				/*** material ***/
				else if (keyword == "usemtl") //see 'MTL file' below
				{
// 					if (materials) //otherwise we have failed to load MTL file!!!
// 					{
// 						QString mtlName = currentLine.mid(7).trimmed();
// 						//DGM: in case there's space characters in the material name, we must read it again from the original line buffer
// 						//QString mtlName = (tokens.size() > 1 && !tokens[1].isEmpty() ? tokens[1] : "");
// 						currentMaterial = (!mtlName.isEmpty() ? materials->findMaterialByName(mtlName) : -1);
// 						currentMaterialDefined = true;
// 					}
				}
				/*** material file (MTL) ***/
				else if (keyword == "mtllib")
				{
					//malformed line?
					/*if (tokens.size() < 2 || tokens[1].isEmpty())
					{
						objWarnings[INVALID_LINE] = true;
					}
					else
					{
						//we build the whole MTL filename + path
						//DGM: in case there's space characters in the filename, we must read it again from the original line buffer
						//QString mtlFilename = tokens[1];
						QString mtlFilename = currentLine.mid(7).trimmed();
						//remove any quotes around the filename (Photoscan 1.4 bug)
						if (mtlFilename.startsWith("\""))
						{
							mtlFilename = mtlFilename.right(mtlFilename.size() - 1);
						}
						if (mtlFilename.endsWith("\""))
						{
							mtlFilename = mtlFilename.left(mtlFilename.size() - 1);
						}
						ccLog::Print(QString("[OBJ] Material file: ") + mtlFilename);
						QString mtlPath = QFileInfo(filename).canonicalPath();
						//we try to load it
						if (!materials)
						{
							materials = new ccMaterialSet("materials");
							materials->link();
						}

						size_t oldSize = materials->size();
						QStringList errors;
						if (ccMaterialSet::ParseMTL(mtlPath, mtlFilename, *materials, errors))
						{
							ccLog::Print("[OBJ] %i materials loaded", materials->size() - oldSize);
							materialsLoadFailed = false;
						}
						else
						{
							ccLog::Error(QString("[OBJ] Failed to load material file! (should be in '%1')").arg(mtlPath + '/' + QString(mtlFilename)));
							materialsLoadFailed = true;
						}

						if (!errors.empty())
						{
							for (int i = 0; i < errors.size(); ++i)
								ccLog::Warning(QString("[OBJ::Load::MTL parser] ") + errors[i]);
						}
						if (materials->empty())
						{
							materials->release();
							materials = 0;
							materialsLoadFailed = true;
						}
					}*/
				}
				///*** shading group ***/
				//else if (tokens.front() == "s")
				//{
				//	//ignored!
				//}

				if (error)
					break;
			}
		}
		catch (const std::bad_alloc&)
		{
			//not enough memory
			objWarnings[NOT_ENOUGH_MEMORY] = true;
			error = true;
		}


		//1st check
		if (!error && pointsRead == 0)
		{
			//of course if there's no vertex, that's the end of the story ...
			//ccLog::Warning("[OBJ] Malformed file: no vertex in file!");
			error = true;
		}

		if (!error)
		{
			if (maxVertexIndex >= pointsRead
				|| maxTexCoordIndex >= texCoordsRead
				|| maxTriNormIndex >= normsRead)
			{
				//hum, we've got a problem here
				if (maxVertexIndex >= pointsRead)
				{
					error = true;
				}
				else
				{
					objWarnings[INVALID_INDEX] = true;
					if (maxTexCoordIndex >= texCoordsRead)
					{
// 						texCoords->release();
// 						texCoords = 0;
// 						materials->release();
// 						materials = 0;
					}
					if (maxTriNormIndex >= normsRead)
					{
// 						normals->release();
// 						normals = 0;
					}
				}
			}
		}

		if (error)
		{
			if (mesh)
				delete mesh;
		}
		fin.close();

		if (error)
		{
			if (mesh)
				delete mesh;

			if (objWarnings[NOT_ENOUGH_MEMORY])
				return CC_FERR_NOT_ENOUGH_MEMORY;
			else if (objWarnings[CANCELLED_BY_USER])
				return CC_FERR_CANCELED_BY_USER;
			else
				return CC_FERR_MALFORMED_FILE;
		}

		if (mesh) {
			mesh->refreshBoundBox();
			mesh->build();
		}
		container = mesh;

		return CC_FERR_NO_ERROR;
	}
}