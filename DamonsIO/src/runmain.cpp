#include "..\include\FileIOFilter.h"
#include "..\..\DamonsDataBase\include\MeshModel.h"
#include<iostream>
using namespace DMeshLib;
int main(int argc, char **argv) {

	DamonsIO::FileIOFilter::InitInternalFilters();
	
	auto curfilter = DamonsIO::FileIOFilter::FindBestFilterForExtension("stl");

	DamonsIO::FileIOFilter::LoadParameters parameters;
	{
		parameters.alwaysDisplayLoadDialog = true;
	}
	DamonsIO::DAMONS_FILE_ERROR result = DamonsIO::CC_FERR_NO_ERROR;
	std::string filename = "D:\\OpenSource\\meshlab\\src\\distrib\\sample\\conrod.stl";
	DMeshLib::ModelObject* mesh = DamonsIO::FileIOFilter::LoadFromFile(filename,parameters,curfilter, result);

	if (mesh) {
		delete mesh;
		mesh = nullptr;
	}

	system("pause");
	return 0;
}

