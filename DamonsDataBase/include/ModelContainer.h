#ifndef _MODELCONTAINER_HEADER_
#define _MODELCONTAINER_HEADER_

#include "..\include\ModelObject.h"
#include <mutex>
#include <atomic>
#include <functional>
#include <memory>
#include <map>
#include <assert.h>

namespace DMeshLib {
	/*!
	 * \class container of mesh models
	 *
	 * \brief implemention of singleton
	 *
	 * \author Damons
	 * \date 十月 2019
	 */
	class DAMONS_DB_LIB_API DModelContainer {

		//////////////////////////////////////////////////////////////////////////
		//-----------------------------------------------------------------------
		// singleton class process
	private:
		DModelContainer();
		DModelContainer(const DModelContainer&) = delete;
		DModelContainer& operator=(const DModelContainer&) = delete;
		~DModelContainer();
	public:
		// 定义为单例（多线程安全）
		static DModelContainer* GetInstance();
		static void DestroyInstance();

	public:
		//************************************  
		// @brief : add a model to container 
		//			if model id is exist return false
		// @author: SunHongLei
		// @date  : 2019/07/26  
		// @return: void
		// @param : void  
		//************************************ 
		bool Register(ModelObject *object);
		//************************************  
		// @brief : erase data from container but do not delete memory 
		// @author: SunHongLei
		// @date  : 2019/07/26  
		// @return: void
		// @param : void  
		//************************************ 
		bool EraseData(unsigned int _id);
		//************************************  
		// @brief : delete data memory
		// @author: SunHongLei
		// @date  : 2019/07/26  
		// @return: void
		// @param : void  
		//************************************ 
		bool DeleteData(unsigned int _id);
		//************************************  
		// @brief : is model exist 
		// @author: SunHongLei
		// @date  : 2019/07/26  
		// @return: void
		// @param : void  
		//************************************ 
		bool IsModelExist(unsigned int _id);
	public:
		//************************************  
		// @brief : get model data
		// @author: SunHongLei
		// @date  : 2019/07/26  
		// @return: void
		// @param : void  
		//************************************ 
		ModelObject * GetModel(unsigned int _id);
		//************************************  
		// @brief : get model size  
		// @author: SunHongLei
		// @date  : 2019/07/26  
		// @return: void
		// @param : void  
		//************************************ 
		unsigned int GetModelSize() const {
			return m_ModelContainer.size();
		}
	protected:
		// common functions
		void Destroy();

	private:
		static DModelContainer * instance_;
		static std::once_flag instance_alloc_;
		static std::once_flag instance_free_;
		static bool initialized_;

	protected:
		std::map<unsigned int, ModelObject *> m_ModelContainer;
	};
}
#endif// 2019/07/26