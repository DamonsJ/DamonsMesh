#ifndef _MESHMODEL_HEADER_
#define _MESHMODEL_HEADER_

//////////////////////////////////////////////////////////////////////////
#include "..\include\damons_db.h"
#include "..\include\ModelObject.h"

#include "..\..\DamonsMath\include\DamonsVector.h"
#include "..\..\DamonsMath\include\DamonsPoint.h"

#include <iostream>
#include <vector>

namespace DMeshLib {
	//////////////////////////////////////////////////////////////////////////
	/*!
	* \class MeshModel
	*
	* \brief mesh model class
	*
	* \author Damons
	* \date 2019-7
	*/
	class DAMONS_DB_LIB_API MeshModel : public ModelObject {

	protected:
		// mesh points
		using DamonsPoint = DGraphic::DPoint<float>;
		std::vector<DamonsPoint > m_meshPoints;
		// triangle index
		using DamonsTriangleIndex = DMath::DVector<unsigned int, 3>;
		std::vector<DamonsTriangleIndex > m_meshIndex;
		// triangle normals
		using DamonsNormal = DGraphic::DPoint<float>;
		std::vector<DamonsNormal > m_meshNormals;

	public:
		MeshModel(std::string name = "");
		MeshModel(const MeshModel& object);
		~MeshModel();

	public:
		// Returns class ID
		inline DB_CLASS_ENUM getClassID() const override { return DB_TYPES::MESH; }
		//************************************  
		// @brief : refresh current model boundbox 
		// @author: SunHongLei
		// @date  : 2019/07/26  
		// @return: void
		// @param : void  
		//************************************ 
		void refreshBoundBox()  override;
	public:
		//************************************  
		// @brief : clone this mesh deep copy 
		// @author: SunHongLei
		// @date  : 2019/07/26  
		// @return: void
		// @param : void  
		//************************************ 
		MeshModel * CloneMesh(MeshModel * _mesh);

	public:

		// resize container size 
		void ResizePoints(unsigned int nbpt) { m_meshPoints.resize(nbpt); }
		void ResizeTriangles(unsigned int nbpt) { m_meshIndex.resize(nbpt); }
		void ResizeNormals(unsigned int nbpt) { m_meshNormals.resize(nbpt); }
		// add and set point value
		void addPoint(float x, float y, float z) { m_meshPoints.push_back(DGraphic::DPoint<float>(x, y, z)); }
		void addPoint(DGraphic::DPoint<float> p) { m_meshPoints.push_back(p); }

		void setPoint(unsigned i, float x, float y, float z) {
			m_meshPoints[i] = DGraphic::DPoint<float>(x, y, z);
		}
		void setPoint(unsigned i, DGraphic::DPoint<float> p) { m_meshPoints[i] = p; }
		// add and set triangle value
		void addTriangle(unsigned int id1, unsigned int id2, unsigned int id3) { m_meshIndex.push_back(DMath::DVector<unsigned int, 3>(id1, id2, id3)); }
		void setTriangle(unsigned i, unsigned int id1, unsigned int id2, unsigned int id3) { m_meshIndex[i] = DMath::DVector<unsigned int, 3>(id1, id2, id3); }
		// add and set normal value
		void addNormal(float x, float y, float z) { m_meshNormals.push_back(DGraphic::DPoint<float>(x, y, z)); }
		void addNormal(DGraphic::DPoint<float> p) { m_meshNormals.push_back(p); }

		void setNormal(unsigned i, float x, float y, float z) {
			m_meshNormals[i] = DGraphic::DPoint<float>(x, y, z);
		}
		void setNormal(unsigned i, DGraphic::DPoint<float> p) { m_meshNormals[i] = p; }

	public:
		// get triangle numbers
		unsigned int getTriangleNumber() const { return m_meshIndex.size(); }
		// get points numbers
		unsigned int getPointsNumber() const { return m_meshPoints.size(); }
		// get point
		void getPoint(unsigned int index, float &x, float &y, float &z) {
			assert(index < m_meshPoints.size());
			x = m_meshPoints[index].x();
			y = m_meshPoints[index].y();
			z = m_meshPoints[index].z();
		}

		void getPoint(unsigned int index, MeshModel::DamonsPoint &p) {
			assert(index < m_meshPoints.size());
			p = m_meshPoints[index];
		}

		// get triangle
		void getTriangleIndex(unsigned int index, unsigned int &index1, unsigned int &index2, unsigned int &index3) {
			assert(index < m_meshIndex.size());
			index1 = m_meshIndex[index].x();
			index2 = m_meshIndex[index].y();
			index3 = m_meshIndex[index].z();
		}

		void getTriangleVertices(unsigned int index, MeshModel::DamonsPoint &p1, MeshModel::DamonsPoint &p2, MeshModel::DamonsPoint &p3) {
			assert(index < m_meshIndex.size());
			unsigned int xindex = m_meshIndex[index].x();
			unsigned int yindex = m_meshIndex[index].y();
			unsigned int zindex = m_meshIndex[index].z();
			p1 = m_meshPoints[xindex];
			p2 = m_meshPoints[yindex];
			p3 = m_meshPoints[zindex];
		}

	};
}
#endif// 2019/07/26