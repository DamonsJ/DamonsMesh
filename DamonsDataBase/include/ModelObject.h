#ifndef _MODELOBJECT_HEADER_
#define _MODELOBJECT_HEADER_

#include "..\..\DamonsMath\include\DamonsPoint.h"
#include "..\..\DamonsMath\include\DamonsBox.h"
#include "..\include\damons_db.h"

#include "..\include\MeshDefines.h"

#include <iostream>
#include <memory>

//////////////////////////////////////////////////////////////////////////
//Bits for object type flags (64 bits)
#define CC_HIERARCH_BIT					0x00000000000001	//Hierarchical object
#define CC_CLOUD_BIT					0x00000000000100	//Point Cloud
#define CC_MESH_BIT						0x00000000000200	//Mesh

namespace DMeshLib {

//////////////////////////////////////////////////////////////////////////
//! object type flags,add when necessary

	using DB_CLASS_ENUM = int64_t;
	namespace DB_TYPES
	{
		enum : DB_CLASS_ENUM {
			OBJECT = 0,
			HIERARCHY_OBJECT = CC_HIERARCH_BIT,
			POINT_CLOUD = HIERARCHY_OBJECT | CC_CLOUD_BIT,
			MESH = HIERARCHY_OBJECT | CC_MESH_BIT
		};
	}

	//////////////////////////////////////////////////////////////////////////

	/*!
	 * \class UniqueIDGenerator
	 *
	 * \brief Unique ID generator (should be unique for the whole application instance - with plugins, etc.)
	 *
	 * \author Damons
	 * \date 2019-7
	 */

	class DAMONS_DB_LIB_API UniqueIDGenerator
	{
	public:
		//! Shared type
		using Shared = std::shared_ptr<UniqueIDGenerator>;
		//! Default constructor
		UniqueIDGenerator() : m_lastUniqueID(0) {}
		//! Resets the unique ID
		void reset() { m_lastUniqueID = 0; }
		//! Returns a (new) unique ID
		unsigned fetchOne() { return ++m_lastUniqueID; }
		//! Returns the value of the last generated unique ID
		unsigned getLast() const { return m_lastUniqueID; }
		//! Updates the value of the last generated unique ID with the current one
		void update(unsigned ID) { if (ID > m_lastUniqueID) m_lastUniqueID = ID; }
	protected:
		unsigned m_lastUniqueID;
	};

	/*!
	 * \class ModelObject
	 *
	 * \brief root class of all models
	 *
	 * \author Damons
	 * \date 2019-7
	 */
	class DAMONS_DB_LIB_API ModelObject
	{
	public:
		ModelObject(std::string name = "");
		ModelObject(const ModelObject& object);
		~ModelObject();
	public:
		//! Changes unique ID
		/** WARNING: HANDLE WITH CARE!
			Updates persistent settings (last unique ID) if necessary.
		**/
		virtual void setUniqueID(unsigned ID);
		//! Returns class ID
		virtual DB_CLASS_ENUM getClassID() const = 0;
		//! Returns object name
		virtual inline std::string getName() const { return m_name; }
		//! Sets object name
		virtual inline void setName(const std::string& name) { m_name = name; }
		//! Returns object unique ID
		virtual inline unsigned getUniqueID() const { return m_uniqueID; }

	public:
		//************************************  
		// @brief : get object type
		// @author: SunHongLei
		// @date  : 2019/07/21
		// @return: 
		// @param :   
		//************************************ 
		inline bool isA(DB_CLASS_ENUM type) const { return (getClassID() == type); }

	public:
		//************************************  
		// @brief : get model bound box
		// @author: SunHongLei
		// @date  : 2019/07/26  
		// @return: void
		// @param : void  
		//************************************ 
		DGraphic::DBox<data_type> getBoundBox() const { return m_box; }
		//************************************  
		// @brief : using for refresh bound box
		// @author: SunHongLei
		// @date  : 2019/07/26  
		// @return: void
		// @param : void  
		//************************************ 
		virtual void refreshBoundBox() = 0;
	public:
		//! Returns a new unassigned unique ID
		/** Unique IDs are handled with persistent settings
			in order to assure consistency between main app
			and plugins!
		**/
		static unsigned GetNextUniqueID();

		//! Returns last assigned unique ID
		/** Unique IDs are handled with persistent settings
			in order to assure consistency between main app
			and plugins!
		**/
		static unsigned GetLastUniqueID();

	protected:
		//! Object unique ID
		unsigned m_uniqueID;
		std::string m_name;

		DGraphic::DBox<data_type > m_box;
	};

}
#endif// 2019/07/19