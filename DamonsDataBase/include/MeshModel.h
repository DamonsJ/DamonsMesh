#ifndef _MESHMODEL_HEADER_
#define _MESHMODEL_HEADER_

//////////////////////////////////////////////////////////////////////////
#include "..\include\damons_db.h"
#include "..\include\ModelObject.h"

#include "..\..\DamonsMath\include\DamonsPoint.h"

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
		// mesh points -> all the points
		std::vector<DamonsVertex > m_meshPoints;
		// triangle index -> all the faces
		std::vector<DamonsFace > m_meshIndex;
		// point normals
		using DamonsNormal = DGraphic::DPoint<DMeshLib::data_type>;
		std::vector<DamonsNormal > m_meshNormals;
		//all the half-edges 
		std::vector<DamonsHalfEdge > m_halfedges;
		using halfedge_pairs = std::map< std::pair<DMeshLib::index_type, DMeshLib::index_type>, DMeshLib::index_type >;
		halfedge_pairs m_edgePairs;
		
	public:
		MeshModel(std::string name = "");
		MeshModel(const MeshModel& object);
		~MeshModel();

		//************************************  
		// @brief : build halfedge structure; 
		// @author: SunHongLei
		// @date  : 2019/10/30  
		// @return: void
		// @param : void  
		//************************************ 
		void build();

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
		void addPoint(data_type x, data_type y, data_type z) {
			DMeshLib::DamonsVertex p(x,y,z);
			p.id = m_meshPoints.size();
			m_meshPoints.push_back(p); 
		}
		void addPoint(DMeshLib::DamonsVertex p) { 
			p.id = m_meshPoints.size();
			m_meshPoints.push_back(p); 
		}

		void setPoint(unsigned int i, data_type x, data_type y, data_type z) {
			DMeshLib::DamonsVertex p(x, y, z);
			p.id = i;
			m_meshPoints[i] = p;
		}
		void setPoint(unsigned int i, DMeshLib::DamonsVertex p) { 
			p.id = i;
			m_meshPoints[i] = p; 
		}
		// add and set triangle value
		void addTriangle(index_type id1, index_type id2, index_type id3) {
			DamonsFace m_face;
			m_face.point_ids.resize(3);

			m_face.point_ids[0] = id1;
			m_face.point_ids[1] = id2;
			m_face.point_ids[2] = id3;

			m_face.id = m_meshIndex.size();
			m_meshIndex.push_back(m_face);
		}
		void setTriangle(unsigned int i, index_type id1, index_type id2, index_type id3) {
			m_meshIndex[i].point_ids.resize(3);
			m_meshIndex[i].point_ids[0] = id1;
			m_meshIndex[i].point_ids[1] = id2;
			m_meshIndex[i].point_ids[2] = id3;
			m_meshIndex[i].id = i;
		}
		// add and set normal value
		void addNormal(data_type x, data_type y, data_type z) { m_meshNormals.push_back(DGraphic::DPoint<data_type>(x, y, z)); }
		void addNormal(DGraphic::DPoint<data_type> p) { m_meshNormals.push_back(p); }

		void setNormal(unsigned i, data_type x, data_type y, data_type z) {
			m_meshNormals[i] = DGraphic::DPoint<data_type>(x, y, z);
		}
		void setNormal(unsigned i, DGraphic::DPoint<data_type> p) { m_meshNormals[i] = p; }

	public:
		// get triangle numbers
		unsigned int getTriangleNumber() const { return m_meshIndex.size(); }
		// get points numbers
		unsigned int getPointsNumber() const { return m_meshPoints.size(); }
		// get point
		void getPoint(unsigned int index, data_type &x, data_type &y, data_type &z) {
			assert(index < m_meshPoints.size());
			x = m_meshPoints[index].x;
			y = m_meshPoints[index].y;
			z = m_meshPoints[index].z;
		}

		void getPoint(unsigned int index, DMeshLib::DamonsVertex &p) {
			assert(index < m_meshPoints.size());
			p = m_meshPoints[index];
		}

		// get triangle
		void getTriangleIndex(unsigned int index, index_type &index1, index_type &index2, index_type &index3) {
			assert(index < m_meshIndex.size());
			index1 = m_meshIndex[index].point_ids[0];
			index2 = m_meshIndex[index].point_ids[1];
			index3 = m_meshIndex[index].point_ids[2];
		}

		void getTriangleVertices(unsigned int index, DMeshLib::DamonsVertex &p1, DMeshLib::DamonsVertex &p2, DMeshLib::DamonsVertex &p3) {
			assert(index < m_meshIndex.size());
			index_type xindex = m_meshIndex[index].point_ids[0];
			index_type yindex = m_meshIndex[index].point_ids[1];
			index_type zindex = m_meshIndex[index].point_ids[2];
			p1 = m_meshPoints[xindex];
			p2 = m_meshPoints[yindex];
			p3 = m_meshPoints[zindex];
		}
		// get element
		unsigned int getElementVertexNumber(unsigned int index) {
			assert(index < m_meshIndex.size());
			return m_meshIndex[index].point_ids.size();
		}

		void getElementIndex(unsigned int index, std::vector<index_type > &ids) {
			assert(index < m_meshIndex.size());
			ids = m_meshIndex[index].point_ids;
		}
		std::vector<index_type > getElementIndex(unsigned int index) {
			assert(index < m_meshIndex.size());
			return m_meshIndex[index].point_ids;
		}

	protected:
		//////////////////////////////////////////////////////////////////////////
		// half-edge concern

		//************************************  
		// @brief : add the half-edge e to container
		// @author: SunHongLei
		// @date  : 2019/10/30  
		// @return: void
		// @param : void  
		//************************************ 
		int addHalfEdgePair(index_type v1, index_type v2);

		//************************************  
		// @brief : given vertex pair find whether this half-edge exist
		// @author: SunHongLei
		// @date  : 2019/10/30  
		// @return: int
		// @param : void  
		//************************************ 
		int findHalfEdge(index_type v1, index_type v2);
	};
};
#endif// 2019/07/26