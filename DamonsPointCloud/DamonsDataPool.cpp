#include "DamonsDataPool.h"

DDataPool * DDataPool::instance_ = nullptr;
std::once_flag DDataPool::instance_alloc_;
std::once_flag DDataPool::instance_free_;
bool DDataPool::initialized_ = false;

//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------
DDataPool* DDataPool::GetInstance() {
	std::call_once(instance_alloc_, [&]() {
		assert(instance_ == nullptr);
		instance_ = new DDataPool;
		initialized_ = true;
	});
	return instance_;
}

void DDataPool::DestroyInstance()
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

DDataPool::DDataPool() {

}

DDataPool::~DDataPool() {
	Destroy();
}

void DDataPool::Destroy() {
	for (auto &item : m_ModelContainer)
	{
		if (item.second) {
			delete item.second;
			item.second = nullptr;
		}
	}
	m_ModelContainer.clear();

	m_ActorContainer.clear();
}
