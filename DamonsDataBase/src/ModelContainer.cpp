#include "..\include\ModelContainer.h"

namespace DMeshLib {
	DModelContainer * DModelContainer::instance_ = nullptr;
	std::once_flag DModelContainer::instance_alloc_;
	std::once_flag DModelContainer::instance_free_;
	bool DModelContainer::initialized_ = false;

	//////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	DModelContainer* DModelContainer::GetInstance() {
		std::call_once(instance_alloc_, [&]() {
			assert(instance_ == nullptr);
			instance_ = new DModelContainer;
			initialized_ = true;
		});
		return instance_;
	}

	void DModelContainer::DestroyInstance()
	{
		std::call_once(instance_free_, [&]() {
			if (nullptr != instance_) {
				delete instance_;
			}
			instance_ = nullptr;
		});
	}
	//////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------

	DModelContainer::DModelContainer() {

	}

	DModelContainer::~DModelContainer() {
		Destroy();
	}

	void DModelContainer::Destroy() {
		for (auto &item : m_ModelContainer)
		{
			if (item.second) {
				delete item.second;
				item.second = nullptr;
			}
		}
		m_ModelContainer.clear();

	}

	//////////////////////////////////////////////////////////////////////////

	bool DModelContainer::Register(ModelObject *object) {
		unsigned int id = object->getUniqueID();
		if (m_ModelContainer.find(id) != m_ModelContainer.end())
			return false;

		m_ModelContainer[id] = object;
		return true;
	}


	ModelObject * DModelContainer::GetModel(unsigned int _id) {

		if (m_ModelContainer.find(_id) != m_ModelContainer.end())
			return nullptr;

		return m_ModelContainer[_id];
	}

	bool DModelContainer::EraseData(unsigned int _id) {
		if (m_ModelContainer.find(_id) != m_ModelContainer.end()) {
			m_ModelContainer.erase(_id);
			return true;
		}

		return false;
	}

	bool DModelContainer::DeleteData(unsigned int _id) {
		if (m_ModelContainer.find(_id) != m_ModelContainer.end())
		{
			if (m_ModelContainer[_id])
			{
				delete m_ModelContainer[_id];
				m_ModelContainer[_id] = nullptr;
				m_ModelContainer.erase(_id);

				return true;
			}
		}

		return false;
	}

	bool DModelContainer::IsModelExist(unsigned int _id) {
		if (m_ModelContainer.find(_id) != m_ModelContainer.end()) {
			return true;
		}

		return false;
	}
}