#include "..\include\MeshModel.h"
#include <algorithm>
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
namespace DMeshLib {
	MeshModel::MeshModel(std::string name /*= ""*/) :ModelObject(name.empty() ? "unnamed_mesh" : name) {

	}

	MeshModel::MeshModel(const MeshModel& object) : ModelObject(object) {
		std::copy(object.m_meshPoints.begin(), object.m_meshPoints.end(), std::back_inserter(m_meshPoints));
		std::copy(object.m_meshIndex.begin(), object.m_meshIndex.end(), std::back_inserter(m_meshIndex));
		std::copy(object.m_meshNormals.begin(), object.m_meshNormals.end(), std::back_inserter(m_meshNormals));
	}

	MeshModel::~MeshModel() {
		std::vector<DamonsPoint >().swap(m_meshPoints);
		std::vector<DamonsTriangleIndex >().swap(m_meshIndex);
		std::vector<DamonsNormal >().swap(m_meshNormals);
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
			m_box.ExtendBox(dp);
		}
	}

}