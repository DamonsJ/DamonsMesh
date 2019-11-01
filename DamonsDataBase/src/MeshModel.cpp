#include "..\include\MeshModel.h"
#include <algorithm>
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
namespace DMeshLib {
// 	MeshModel::MeshModel(std::string name /*= ""*/) :ModelObject((name.empty() ? "unnamed_mesh" : name)) {
// 	
// 	}

	MeshModel::MeshModel(const MeshModel& object) : ModelObject(object) {
		std::copy(object.m_meshPoints.begin(), object.m_meshPoints.end(), std::back_inserter(m_meshPoints));
		std::copy(object.m_meshIndex.begin(), object.m_meshIndex.end(), std::back_inserter(m_meshIndex));
		std::copy(object.m_meshNormals.begin(), object.m_meshNormals.end(), std::back_inserter(m_meshNormals));
		
#ifdef __BUILD_HALFEDGE_TOPO__
		build();
#endif // __BUILD_HALFEDGE_TOPO__
		
	}

	MeshModel::~MeshModel() {
		std::vector<DamonsVertex >().swap(m_meshPoints);
		std::vector<DamonsFace >().swap(m_meshIndex);
		std::vector<DamonsNormal >().swap(m_meshNormals);
		std::vector<DamonsHalfEdge >().swap(m_halfedges);

		m_edgePairs.clear();
	}

	//////////////////////////////////////////////////////////////////////////

	MeshModel * MeshModel::CloneMesh(MeshModel *_mesh) {
		assert(_mesh);

		MeshModel *mm = new MeshModel(*_mesh);
		return mm;
	}

	void MeshModel::refreshBoundBox() {
		for (auto &dp : m_meshPoints)
		{
			DGraphic::DPoint<data_type> p(dp.x,dp.y,dp.z);
			m_box.ExtendBox(p);
		}
	}

	int MeshModel::addHalfEdgePair(index_type v1, index_type v2) {
		DamonsHalfEdge e;
		m_halfedges.push_back(e);
		std::pair<DMeshLib::index_type, DMeshLib::index_type> p = std::make_pair(v1, v2);
		m_edgePairs[p] = index_type(m_halfedges.size() - 1);

		return m_halfedges.size()-1;
	}

	int  MeshModel::findHalfEdge(index_type v1, index_type v2) {
		std::pair<index_type, index_type> p = std::make_pair(v1, v2);
		if (m_edgePairs.find(p) != m_edgePairs.end()) {
			index_type hf_index = m_edgePairs[p];
			return hf_index;
		}
		return -1;
	}

	void MeshModel::build() {
		m_edgePairs.clear();
		m_halfedges.clear();
		m_halfedges.shrink_to_fit();

		for (int fid = 0; fid < m_meshIndex.size(); ++fid)
		{
			auto &face = m_meshIndex[fid];
			int trinum = face.point_ids.size();
			std::vector<index_type > indexs = face.point_ids;
			indexs.push_back(indexs[0]);

			std::vector<int > curedges;
			curedges.resize(trinum + 1);

			for (int k = 0; k < trinum; ++k)
			{
				unsigned int f = indexs[k];
				unsigned int s = indexs[k + 1];

				//whether this half-edge exist
				int es = findHalfEdge(f, s);
				bool isesNew = es != -1 ? false : true;
				int ep = findHalfEdge(s, f);
				bool isepNew = ep != -1 ? false : true;
				// create new if half-edge does not exist
				if (isesNew) {
					es = addHalfEdgePair(f, s);
				}
				if (isepNew) {
					ep = addHalfEdgePair(s, f);
				}

				//set vertex leaving edge
				m_meshPoints[f].edge_out = es;
				m_meshPoints[s].edge_out = ep;
				//set face edge
				face.edge = es;
				curedges[k] = es;

				if (isesNew && isepNew) {
					//set half-edge properties
				
					m_halfedges[es].face = fid;
					m_halfedges[es].pair = ep;
					m_halfedges[es].start_vert = f;

					m_halfedges[ep].pair = es;
					m_halfedges[ep].start_vert = s;
				}
				else {
					m_halfedges[es].face = fid;
				}

			}

			curedges[trinum] = curedges[0];

			// set this half-edge's next and prev edge
			for (int k = 0; k < trinum; ++k)
			{
				m_halfedges[curedges[k]].next = curedges[k + 1];
				m_halfedges[curedges[trinum - k]].prev = curedges[trinum - k - 1];
			}
			std::vector<index_type >().swap(indexs);
			std::vector<int >().swap(curedges);
		}
	}
};
