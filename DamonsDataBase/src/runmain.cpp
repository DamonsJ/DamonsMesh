#include<iostream>
#include<fstream>
#include<sstream>
#include<map>

#include "..\include\MeshModel.h"

using namespace  DMeshLib;
int main(int argc, char **argv) {

	using halfedge_pairs = std::map< std::pair<DMeshLib::index_type, DMeshLib::index_type>, DMeshLib::index_type >;
	halfedge_pairs m_edgePairs;
	
	std::pair<DMeshLib::index_type, DMeshLib::index_type> p = std::make_pair(0, 1);
	m_edgePairs[p] = index_type(0);
	//std::pair<DMeshLib::index_type, DMeshLib::index_type> p1 = std::make_pair(1, 0);
	//m_edgePairs[p1] = index_type(1);
	//
	//std::pair<DMeshLib::index_type, DMeshLib::index_type> p2 = std::make_pair(1, 2);
	//m_edgePairs[p2] = index_type(2);


	std::string filename = "../data/test.off";
	std::ifstream infile(filename);
	std::string sline;
	std::getline(infile, sline);
	std::getline(infile, sline);

	MeshModel *mesh = new MeshModel;

	std::istringstream iss(sline);
	int vertexNum = 0, faceNum = 0;
	if (!(iss >> vertexNum >> faceNum))
		return 0;
	//read vertex and store in mesh
	int i = 0;
	double x = 0.0, y = 0.0, z = 0.0;
	while (i < vertexNum) {
		std::getline(infile, sline);
		if (sline.size() < 1)
			continue;
		++i;
		std::istringstream istmline(sline);
		if (istmline >> x >> y >> z)
		{
			mesh->addPoint(x,y,z);
		}
	}
	i = 0;
	while (i < faceNum)
	{
		std::getline(infile, sline);
		if (sline.size() < 1)
			continue;

		++i;
		unsigned int id0 = 0, id1 = 0,id2 = 0, trinum = 0;
		std::istringstream istmline(sline);
		istmline >> trinum;
		if(3 == trinum)
		{
			istmline >> id0 >> id1 >> id2;
			mesh->addTriangle(id0, id1, id2);
		}
	}

	mesh->build();

	if (mesh) {
		delete mesh;
	}

	system("pause");
	return 0;
}