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
	* \brief mesh model class/ half-edge structure for non-manoid triangle mesh
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
	
	public:
		//////////////////////////////////////////////////////////////////////////
		//half-edge info
		//************************************  
		// @brief : find all the face that a vertex adjacent 
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: void
		// @param[in] : int v_ind: the vertex which should be searched
		// @param[out] : std::vector<index_type> : all the faces found
		//************************************ 
		void vertex_face(const int v_ind, std::vector<index_type> &leaving_face) {
			std::vector<index_type > leaving_list;
			vertex_half_edges(v_ind, leaving_list);
			for (auto &he : leaving_list)
			{
				if (-1 != m_halfedges[he].face) {
					leaving_face.push_back(m_halfedges[he].face);
				}
			}
			std::vector<index_type>().swap(leaving_list);
		}

		std::vector<index_type> vertex_face(const int v_ind) {
			std::vector<index_type> leaving_face;
			std::vector<index_type > leaving_list;
			vertex_half_edges(v_ind, leaving_list);
			for (auto &he : leaving_list)
			{
				if (-1 != m_halfedges[he].face) {
					leaving_face.push_back(m_halfedges[he].face);
				}
			}
			std::vector<index_type>().swap(leaving_list);

			return leaving_face;
		}

		//************************************  
		// @brief : find all the half-edge that leaving a vertex
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: void
		// @param[in] : int v_ind: the vertex which should be searched
		// @param[out] : std::vector<index_type> : all the half-edges found 
		//************************************ 
		void vertex_half_edges(const int v_ind, std::vector<index_type> &leaving_list) 
		{
			DamonsVertex &v = m_meshPoints[v_ind];
			int he_index = v.edge_out;
			assert(-1 != he_index);
			leaving_list.push_back(he_index);

			DamonsHalfEdge &h = m_halfedges[he_index];
			int he_next = m_halfedges[h.pair].next;
			// forward search
			while (he_next != -1 && he_next != he_index)
			{
				leaving_list.push_back(he_next);
				he_next = m_halfedges[he_next].pair;
				if (-1 != he_next)
					he_next = m_halfedges[he_next].next;
			}

			he_index = v.edge_out;
			DamonsHalfEdge &p = m_halfedges[he_index];
			int he_prev_pair = m_halfedges[p.prev].pair;
			//backward search
			while (he_prev_pair != -1 && he_prev_pair != he_index)
			{
				leaving_list.push_back(he_prev_pair);
				he_prev_pair = m_halfedges[he_prev_pair].prev;
				if(-1 != he_prev_pair)
					he_prev_pair = m_halfedges[he_prev_pair].pair;
			}
			//remove duplicate
			std::sort(leaving_list.begin(), leaving_list.end());
			auto iter = std::unique(leaving_list.begin(), leaving_list.end());
			leaving_list.erase(iter, leaving_list.end());
		}

		std::vector<index_type> vertex_half_edges(const int v_ind) {
			std::vector<index_type> leaving_list;
			DamonsVertex &v = m_meshPoints[v_ind];
			int he_index = v.edge_out;
			assert(-1 != he_index);
			leaving_list.push_back(he_index);

			DamonsHalfEdge &h = m_halfedges[he_index];
			int he_next = m_halfedges[h.pair].next;

			while (he_next != -1 && he_next != he_index)
			{
				leaving_list.push_back(he_next);
				he_next = m_halfedges[he_next].pair;
				he_next = m_halfedges[he_next].next;
			}

			return leaving_list;
		}
		//************************************  
		// @brief : find all the vertex adjacent with given vertex 
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: void
		// @param : void  
		//************************************ 
		void vertex_vertex(const int v_ind, std::vector<index_type> &leaving_vertex) {
			std::vector<index_type > leaving_list;
			vertex_half_edges(v_ind, leaving_list);

			for (auto &he : leaving_list)
			{
				if (-1 != m_halfedges[he].pair) {
					leaving_vertex.push_back(m_halfedges[m_halfedges[he].pair].start_vert);
				}
			}
		}
		std::vector<index_type> vertex_vertex(const int v_ind) {
			std::vector<index_type> leaving_vertex;

			std::vector<index_type > leaving_list;
			vertex_half_edges(v_ind, leaving_list);

			for (auto &he : leaving_list)
			{
				if (-1 != m_halfedges[he].pair) {
					leaving_vertex.push_back(m_halfedges[m_halfedges[he].pair].start_vert);
				}
			}

			return leaving_vertex;
		}

		//************************************  
		// @brief :  
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: void
		// @param : void  
		//************************************ 
		std::vector<index_type> face_face(const int v_ind) {

			std::vector<index_type> faces;

			DamonsFace &f = m_meshIndex[v_ind];
			int border = f.edge;
			int next = m_halfedges[border].next;
			int fid = m_halfedges[m_halfedges[border].pair].face;
			if (-1 != fid) 
				faces.push_back(fid);

			while (border != next)
			{
				fid = m_halfedges[m_halfedges[next].pair].face;
				if (-1 != fid) {
					faces.push_back(fid);
				}
				next = m_halfedges[next].next;
			}

			return faces;
		}

		void face_face(const int v_ind, std::vector<index_type> &faces) {

			DamonsFace &f = m_meshIndex[v_ind];
			int border = f.edge;
			int next = m_halfedges[border].next;
			int fid = m_halfedges[m_halfedges[border].pair].face;
			if (-1 != fid)
				faces.push_back(fid);

			while (border != next)
			{
				fid = m_halfedges[m_halfedges[next].pair].face;
				if (-1 != fid) {
					faces.push_back(fid);
				}
				next = m_halfedges[next].next;
			}
		}
		//************************************  
		// @brief : find all the boundaries in mesh
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: void
		// @param : void  
		//************************************ 
		std::vector< std::vector<index_type > > findBoundary() {

			std::map<index_type, index_type> boundarys_points;
			std::vector<index_type > curhalfedges;
			for(int i = 0; i < m_halfedges.size();++i)
			{
				DamonsHalfEdge &hf = m_halfedges[i];
				if (hf.face == -1) {
					boundarys_points[index_type(hf.start_vert)] = i;
					curhalfedges.push_back(i);
				}
			}

			std::vector< std::vector<index_type > > all_boundarys;
			std::vector<index_type > boundarys;
			index_type curedge = curhalfedges[0];
			boundarys.push_back(m_halfedges[curedge].start_vert);

			while (boundarys_points.size())
			{
				if (boundarys.size() == 1 || (boundarys.size() > 1 && boundarys[0] != boundarys.back()))
				{
					int pair_id = m_halfedges[curedge].pair;
					index_type id = m_halfedges[pair_id].start_vert;
					if (boundarys_points.find(id) != boundarys_points.end()) {
						curedge = boundarys_points[id];
						boundarys.push_back(m_halfedges[curedge].start_vert);
						boundarys_points.erase(id);
					}
				}
				else if (boundarys[0] == boundarys.back()) {
					// another boundary
					all_boundarys.push_back(boundarys);
					boundarys.clear();
					curedge = boundarys_points.begin()->second;
					boundarys.push_back(m_halfedges[curedge].start_vert);
				}
			};

			if (boundarys.size())
				all_boundarys.push_back(boundarys);

			return all_boundarys;
		}
	public:
		//************************************  
		// @brief : get the vertex
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: void
		// @param : void  
		//************************************ 
		const DamonsVertex* getVertex(const int v_ind) {
			const DamonsVertex *p = &m_meshPoints[v_ind];
			return p;
		}
		//************************************  
		// @brief : get half-edge  
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: void
		// @param : void  
		//************************************ 
		const DamonsHalfEdge* getHalfEdge(const int e_ind) {
			const DamonsHalfEdge *he = &m_halfedges[e_ind];
			return he;
		}
		//************************************  
		// @brief : get face 
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: void
		// @param : void  
		//************************************ 
		const DamonsFace* getFace(const int f_ind) {
			const DamonsFace *f = &m_meshIndex[f_ind];
			return f;
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