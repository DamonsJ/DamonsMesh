#include "..\include\OFFFilter.h"

//System
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace DamonsIO {

	bool OFFFilter::canLoadExtension(const std::string& upperCaseExt) const
	{
		return (upperCaseExt == "OFF");
	}


	DAMONS_FILE_ERROR OFFFilter::saveToFile(DMeshLib::ModelObject* entity, const std::string& filename, const SaveParameters& parameters)
	{
		if (!entity)
			return CC_FERR_BAD_ARGUMENT;

		if (!entity->isA(DMeshLib::DB_TYPES::MESH))
		{
			std::cerr<<"[OFF] This filter can only save one mesh at a time!";
			return CC_FERR_BAD_ENTITY_TYPE;
		}

		DMeshLib::MeshModel* mesh =static_cast<DMeshLib::MeshModel*>(entity);
		if (!mesh || mesh->getTriangleNumber() == 0)
		{
			std::cerr << "[OFF] Input mesh is empty!";
			return CC_FERR_NO_SAVE;
		}

		std::ofstream fout(filename);
		//header: "OFF"
		fout << "OFF" << std::endl;

		//2nd line: vertices count / faces count / edges count
		unsigned vertCount = mesh->getPointsNumber();
		unsigned triCount = mesh->getTriangleNumber();
		fout << vertCount << ' ' << triCount << ' ' << 0 << std::endl;

		//save vertices
		{
			DMeshLib::data_type x, y, z;
			for (unsigned i = 0; i < vertCount; ++i)
			{
				mesh->getPoint(i, x, y, z);
				fout << x << ' ' << y << ' ' << z << std::endl;
			}
		}

		//save triangles
		{
			DMeshLib::index_type id1, id2, id3;
			for (unsigned i = 0; i < triCount; ++i)
			{
				mesh->getTriangleIndex(i, id1, id2, id3);
				fout << "3 " << id1 << ' ' << id2 << ' ' << id3 << std::endl;
			}
		}

		fout.close();

		return CC_FERR_NO_ERROR;
	}


	static std::string GetNextLine(std::ifstream& stream)
	{
		std::string currentLine;
		//skip comments
		do
		{
			if (!std::getline(stream, currentLine))
				return "";
		} while (currentLine[0] == '#' || currentLine.empty());

		return currentLine;
	}

	DAMONS_FILE_ERROR OFFFilter::loadFile(const std::string& filename, DMeshLib::ModelObject *&container, LoadParameters& parameters)
	{
		std::ifstream fin(filename);
		if (!fin.is_open()) {
			fin.close();
			return CC_FERR_READING;
		}

		std::string currentLine;
		std::getline(fin, currentLine);
		std::transform(currentLine.begin(), currentLine.end(), currentLine.begin(),::toupper);
		std::string token = currentLine.substr(0, 3);
		if (token != "OFF")
			return CC_FERR_MALFORMED_FILE;

		// find next line 
		std::string tokens = GetNextLine(fin);
		unsigned vertCount = 0, triCount = 0;
		std::stringstream ss(tokens);
		ss >> vertCount >> triCount;

		if (!(ss >> vertCount >> triCount))
			return CC_FERR_MALFORMED_FILE;
		
		DMeshLib::MeshModel* mesh = new DMeshLib::MeshModel("off_mesh");
		mesh->ResizePoints(vertCount);

		//read vertices
		{
			DMeshLib::data_type x, y, z;
			for (unsigned i = 0; i < vertCount; ++i)
			{
				currentLine = GetNextLine(fin);
				std::stringstream sstrm(currentLine);
				sstrm >> x >> y >> z;
				if (!(sstrm >> x >> y >> z))
				{
					delete mesh;
					mesh = nullptr;
					container = mesh;
					return CC_FERR_MALFORMED_FILE;
				}
				mesh->setPoint(i, x, y, z);
			}
		}
		//mesh->ResizeTriangles(triCount);
		//load triangles
		{
			DMeshLib::index_type indexes[4];
			unsigned polyVertCount;

			bool ignoredPolygons = false;
			for (unsigned i = 0; i < triCount; ++i)
			{
				currentLine = GetNextLine(fin);
				std::stringstream sstrm(currentLine);

				sstrm >> polyVertCount;
				if (!(sstrm >> polyVertCount))
				{
					delete mesh;
					mesh = nullptr;
					return CC_FERR_MALFORMED_FILE;
				}

				if (polyVertCount == 3 || polyVertCount == 4)
				{
					//decode indexes
					for (unsigned j = 0; j < polyVertCount; ++j)
					{
						if (!(sstrm >> indexes[j]))
						{
							delete mesh;
							return CC_FERR_MALFORMED_FILE;
						}
					}
					//triangle or quad only
					mesh->addTriangle(indexes[0], indexes[1], indexes[2]);
					if (polyVertCount == 4)
						mesh->addTriangle(indexes[0], indexes[2], indexes[3]);
				}
				else
				{
					ignoredPolygons = true;
				}
			}

			if (ignoredPolygons)
			{
				std::cerr<<"[OFF] Some polygons with an unhandled size (i.e. > 4) were ignored!";
			}
		}

		fin.close();

		if (mesh->getTriangleNumber() == 0)
		{
			delete mesh;
			mesh = nullptr;
		}
		else
		{
			mesh->refreshBoundBox();
			mesh->build();
		}
		container = mesh;
		return CC_FERR_NO_ERROR;
	}

}