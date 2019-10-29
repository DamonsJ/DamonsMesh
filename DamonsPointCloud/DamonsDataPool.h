#ifndef _DAMONSDATAPOOL_HEADER_
#define _DAMONSDATAPOOL_HEADER_

#include <mutex>
#include <atomic>
#include <functional>
#include <memory>
#include <map>
#include <assert.h>

#include <vtkSmartPointer.h>
#include <vtkActor.h>

#include "ModelObject.h"

class DDataPool {

	//////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	// singleton class process
private:
	DDataPool();
	DDataPool(const DDataPool&) = delete;
	DDataPool& operator=(const DDataPool&) = delete;
	~DDataPool();
public:
	// 定义为单例（多线程安全）
	static DDataPool* GetInstance();
	static void DestroyInstance();

public:


protected:
	// common functions
	void Destroy();

private:
	static DDataPool * instance_;
	static std::once_flag instance_alloc_;
	static std::once_flag instance_free_;
	static bool initialized_;

protected:
	std::map<unsigned int, ModelObject *> m_ModelContainer;
	std::map<unsigned int, vtkSmartPointer<vtkActor> > m_ActorContainer;

};

#endif// 2019/07/22