
#include "..\include\STLFilter.h"

//System
#include <string.h>

namespace DamonsIO {

	bool STLFilter::canLoadExtension(const std::string& upperCaseExt) const
	{
		return (upperCaseExt == "STL");
	}
	
	DAMONS_FILE_ERROR STLFilter::saveToFile(DMeshLib::ModelObject* entity, const std::string& filename, const SaveParameters& parameters)
	{
		if (!entity)
			return CC_FERR_BAD_ARGUMENT;

		if (!entity->isA(DMeshLib::DB_TYPES::MESH))
			return CC_FERR_BAD_ENTITY_TYPE;

		DMeshLib::MeshModel* mesh = static_cast<DMeshLib::MeshModel*>(entity);
		if (!mesh || mesh->getTriangleNumber() == 0)
		{
			return CC_FERR_NO_ERROR;
		}

		//ask for output format
		bool binaryMode = true;

		//try to open file for saving
		FILE* theFile = fopen(filename.c_str(), "wb");
		if (!theFile)
			return CC_FERR_WRITING;

		DAMONS_FILE_ERROR result = CC_FERR_NO_ERROR;
		if (binaryMode)
		{
			result = saveToBINFile(mesh, theFile);
		}
		else
		{
			result = saveToASCIIFile(mesh, theFile);
		}

		fclose(theFile);

		return result;
	}

	DAMONS_FILE_ERROR STLFilter::saveToBINFile(DMeshLib::MeshModel* mesh, FILE *theFile)
	{
		unsigned faceCount = mesh->getTriangleNumber();

		//header
		{
			char header[80];
			memset(header, 0, 80);
			strcpy(header, "Binary STL file generated by CloudCompare!");
			if (fwrite(header, 80, 1, theFile) < 1)
				return CC_FERR_WRITING;
		}

		//UINT32 Number of triangles
		{
			uint32_t tmpInt32 = static_cast<uint32_t>(faceCount);
			if (fwrite((const void*)&tmpInt32, 4, 1, theFile) < 1)
				return CC_FERR_WRITING;
		}

		for (unsigned i = 0; i < faceCount; ++i)
		{
			DMeshLib::index_type id1, id2, id3;
			mesh->getTriangleIndex(i, id1, id2, id3);

			DGraphic::DPoint<DMeshLib::data_type> p1, p2, p3;
			mesh->getTriangleVertices(i, p1, p2, p3);

			DGraphic::DPoint<DMeshLib::data_type> v1 = p2 - p1;
			DGraphic::DPoint<DMeshLib::data_type> v2 = p3 - p1;

			DGraphic::DPoint<DMeshLib::data_type> n = v1.CrossProduct(v2);
			n.Normalize();

			if (fwrite((const void*)(&n[0]), sizeof(DMeshLib::data_type), 3, theFile) < 3)
				return CC_FERR_WRITING;

			//REAL32[3] Vertex 1,2 & 3
			if (fwrite((const void*)(&p1[0]), sizeof(DMeshLib::data_type), 3, theFile) < 3)
				return CC_FERR_WRITING;
			if (fwrite((const void*)(&p2[0]), sizeof(DMeshLib::data_type), 3, theFile) < 3)
				return CC_FERR_WRITING;
			if (fwrite((const void*)(&p3[0]), sizeof(DMeshLib::data_type), 3, theFile) < 3)
				return CC_FERR_WRITING;

			//UINT16 Attribute byte count (not used)
			{
				char byteCount[2] = { 0, 0 };
				if (fwrite(byteCount, 2, 1, theFile) < 1)
					return CC_FERR_WRITING;
			}
		}

		return CC_FERR_NO_ERROR;
	}

	DAMONS_FILE_ERROR STLFilter::saveToASCIIFile(DMeshLib::MeshModel* mesh, FILE *theFile)
	{
		assert(theFile && mesh && mesh->getTriangleNumber() != 0);
		unsigned faceCount = mesh->getTriangleNumber();

		if (fprintf(theFile, "solid %s\n", (mesh->getName()).c_str()) < 0) //empty names are acceptable!
		{
			return CC_FERR_WRITING;
		}

		for (unsigned i = 0; i < faceCount; ++i)
		{
			DMeshLib::index_type id1, id2, id3;
			mesh->getTriangleIndex(i, id1, id2, id3);

			DGraphic::DPoint<DMeshLib::data_type> p1, p2, p3;
			mesh->getTriangleVertices(i, p1, p2, p3);

			DGraphic::DPoint<DMeshLib::data_type> v1 = p2 - p1;
			DGraphic::DPoint<DMeshLib::data_type> v2 = p3 - p1;

			DGraphic::DPoint<DMeshLib::data_type> n = v1.CrossProduct(v2);
			n.Normalize();

			if (fprintf(theFile, "facet normal %e %e %e\n", n.x(), n.y(), n.z()) < 0)
				return CC_FERR_WRITING;
			if (fprintf(theFile, "outer loop\n") < 0)
				return CC_FERR_WRITING;

			if (fprintf(theFile, "vertex %e %e %e\n", p1.x(), p1.y(), p1.z()) < 0)
				return CC_FERR_WRITING;
			
			if (fprintf(theFile, "vertex %e %e %e\n", p2.x(), p2.y(), p2.z()) < 0)
				return CC_FERR_WRITING;
			 
			if (fprintf(theFile, "vertex %e %e %e\n", p3.x(), p3.y(), p3.z()) < 0)
				return CC_FERR_WRITING;
			if (fprintf(theFile, "endloop\nendfacet\n") < 0)
				return CC_FERR_WRITING;

		}

		if (fprintf(theFile, "endsolid %s\n", (mesh->getName()).c_str()) < 0) //empty names are acceptable!
		{
			return CC_FERR_WRITING;
		}

		return CC_FERR_NO_ERROR;
	}

	bool STLFilter::IsSTLBinary(const char * filename) {

		bool binaryFlag = false;
		FILE *fp = fopen(filename, "r");
		/* Find size of file */
		fseek(fp, 0, SEEK_END);
		long file_size = ftell(fp);
		unsigned int facenum;
		/* Check for binary or ASCII file */
		fseek(fp, STL_LABEL_SIZE, SEEK_SET);
		fread(&facenum, sizeof(unsigned int), 1, fp);
		
		// lets'make a test to check that we find only ascii stuff before assuming it is ascii
		unsigned char tmpbuf[1000];
		int byte_to_read = std::min(int(sizeof(tmpbuf)), int(file_size - 80));
		fread(tmpbuf, byte_to_read, 1, fp);
		fclose(fp);
		for (int i = 0; i < byte_to_read; i++)
		{
			if (tmpbuf[i] > 127)
			{
				binaryFlag = true;
				break;
			}
		}
		// Now we know if the stl file is ascii or binary.
		return binaryFlag;
	}

	DAMONS_FILE_ERROR STLFilter::loadFile(const std::string& filename, DMeshLib::ModelObject *&container, LoadParameters& parameters)
	{

		bool ascii = IsSTLBinary(filename.c_str());

		DMeshLib::MeshModel* mesh = new DMeshLib::MeshModel("stl_mesh");

		DAMONS_FILE_ERROR error = CC_FERR_NO_ERROR;
		if (!ascii)
			error = loadASCIIFile(filename, mesh, parameters);
		else
			error = loadBinaryFile(filename, mesh, parameters);

		if (error != CC_FERR_NO_ERROR)
		{
			return CC_FERR_MALFORMED_FILE;
		}

		container = mesh;

		return CC_FERR_NO_ERROR;
	}

	DAMONS_FILE_ERROR STLFilter::loadASCIIFile(std::string filename,
		DMeshLib::MeshModel* mesh,
		LoadParameters& parameters)
	{
		FILE *fp;
		fp = fopen(filename.c_str(), "r");
		if (fp == nullptr)
		{
			return CC_FERR_READING;
		}
		long currentPos = ftell(fp);
		fseek(fp, 0L, SEEK_END);
		long fileLen = ftell(fp);
		fseek(fp, currentPos, SEEK_SET);

		/* Skip the first line of the file */
		while (getc(fp) != '\n') {}

		struct
		{
			float n[3];
			float v1[3];
			float v2[3];
			float v3[3];
		} f;

		int cnt = 0;
		int lineCnt = 0;
		int ret;
		/* Read a single facet from an ASCII .STL file */
		while (!feof(fp))
		{
			ret = fscanf(fp, "%*s %*s %f %f %f\n", &f.n[0], &f.n[1], &f.n[2]); // --> "facet normal 0 0 0"
			if (ret != 3)
			{
				lineCnt++;
				continue;
			}
			ret = fscanf(fp, "%*s %*s"); // --> "outer loop"
			ret = fscanf(fp, "%*s %f %f %f\n", &f.v1[0], &f.v1[1], &f.v1[2]); // --> "vertex x y z"
			if (ret != 3)
				return CC_FERR_READING;
			ret = fscanf(fp, "%*s %f %f %f\n", &f.v2[0], &f.v2[1], &f.v2[2]); // --> "vertex x y z"
			if (ret != 3)
				return CC_FERR_READING;
			ret = fscanf(fp, "%*s %f %f %f\n", &f.v3[0], &f.v3[1], &f.v3[2]); // --> "vertex x y z"
			if (ret != 3)
				return CC_FERR_READING;
			ret = fscanf(fp, "%*s"); // --> "endloop"
			ret = fscanf(fp, "%*s"); // --> "endfacet"
			lineCnt += 7;
			if (feof(fp)) break;

			mesh->addPoint(f.v1[0], f.v1[1], f.v1[2]);
			mesh->addPoint(f.v2[0], f.v2[1], f.v2[2]);
			mesh->addPoint(f.v3[0], f.v3[1], f.v3[2]);

			mesh->addNormal(f.n[0], f.n[1], f.n[2]);
			mesh->addNormal(f.n[0], f.n[1], f.n[2]);
			mesh->addNormal(f.n[0], f.n[1], f.n[2]);

			mesh->addTriangle(cnt, cnt + 1, cnt + 2);
			cnt = cnt + 3;
		}
		fclose(fp);
		mesh->refreshBoundBox();
		mesh->build();
		return CC_FERR_NO_ERROR;
	}

	DAMONS_FILE_ERROR STLFilter::loadBinaryFile(std::string filename,
		DMeshLib::MeshModel* mesh,
		LoadParameters& parameters)
	{
		FILE *fp;
		fp = fopen(filename.c_str(), "rb");
		if (fp == nullptr)
		{
			return CC_FERR_READING;
		}

		int facenum;
		fseek(fp, STL_LABEL_SIZE, SEEK_SET);
		fread(&facenum, sizeof(int), 1, fp);

		mesh->ResizePoints(facenum * 3);
		mesh->ResizeTriangles(facenum);
		mesh->ResizeNormals(facenum * 3);

		// For each triangle read the normal, the three coords and a short set to zero
		for (int i = 0; i < facenum; ++i)
		{
			unsigned short attr;
			float norm[3];
			float tri[9];
			fread(&norm, 3 * sizeof(float), 1, fp);
			fread(&tri, 3 * sizeof(float), 3, fp);
			fread(&attr, sizeof(unsigned short), 1, fp);

			mesh->setPoint(3 * i, tri[0], tri[1], tri[2]);
			mesh->setPoint(3 * i + 1, tri[3], tri[4], tri[5]);
			mesh->setPoint(3 * i + 2, tri[6], tri[7], tri[8]);

			mesh->setNormal(3 * i, norm[0], norm[1], norm[2]);
			mesh->setNormal(3 * i + 1, norm[0], norm[1], norm[2]);
			mesh->setNormal(3 * i + 2, norm[0], norm[1], norm[2]);

			mesh->setTriangle(i, 3 * i, 3 * i + 1, 3 * i + 2);
		}
		fclose(fp);
		mesh->refreshBoundBox();
		mesh->build();

		return CC_FERR_NO_ERROR;
	}

}