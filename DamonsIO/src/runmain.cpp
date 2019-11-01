#include "..\include\FileIOFilter.h"
#include "..\..\DamonsDataBase\include\MeshModel.h"
#include<iostream>
using namespace DMeshLib;
int main(int argc, char **argv) {

	DamonsIO::FileIOFilter::InitInternalFilters();
	
	std::string curfilter = "stl";
	const std::vector<std::string>	fileFilterList = DamonsIO::FileIOFilter::GetAllFilters();

	for (const std::string &fileFilter : fileFilterList)
	{
		if (curfilter == fileFilter)
		{
			break;
		}
	}
	std::string meshname = "aaa";
	DMeshLib::MeshModel* mesh = new DMeshLib::MeshModel(meshname);
	DamonsIO::FileIOFilter::LoadParameters parameters;
	{
		parameters.alwaysDisplayLoadDialog = true;
	}
	DamonsIO::DAMONS_FILE_ERROR result = DamonsIO::CC_FERR_NO_ERROR;
	std::string filename = "D:\\Program Files (x86)\\OpenCASCADE-7.3.0-vc14-64\\opencascade-7.3.0\\data\\stl\\bearing.stl";
	DamonsIO::FileIOFilter::LoadFromFile(filename,parameters, result,curfilter);
	system("pause");
	return 0;
}

