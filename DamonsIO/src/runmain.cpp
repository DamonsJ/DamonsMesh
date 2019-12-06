#include "..\include\FileIOFilter.h"
#include "..\..\DamonsDataBase\include\MeshModel.h"
#include<iostream>
using namespace DMeshLib;
int main(int argc, char **argv) {

	DamonsIO::FileIOFilter::InitInternalFilters();
	
	auto curfilter = DamonsIO::FileIOFilter::FindBestFilterForExtension("3ds");

	DamonsIO::FileIOFilter::LoadParameters parameters;
	{
		parameters.alwaysDisplayLoadDialog = true;
	}
	DamonsIO::DAMONS_FILE_ERROR result = DamonsIO::CC_FERR_NO_ERROR;
	std::string filename = "F:/DamonsPointCloud/DamonsIO/data/lion.3ds";
	DMeshLib::ModelObject* mesh = DamonsIO::FileIOFilter::LoadFromFile(filename,parameters,curfilter, result);
	
	std::cout << "read file end" << std::endl;
	DamonsIO::FileIOFilter::SaveParameters sparameters;
	{
		sparameters.alwaysDisplaySaveDialog = true;
	}
	DamonsIO::FileIOFilter::SaveToFile(mesh, "F:/DamonsPointCloud/DamonsIO/data/testr.3ds", sparameters, curfilter);

	if (mesh) {
		delete mesh;
		mesh = nullptr;
	}

	system("pause");
	return 0;
}

