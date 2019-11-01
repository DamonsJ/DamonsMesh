#include "..\include\ModelObject.h"
#include <stdexcept>


namespace DMeshLib {
	//! Default unique ID generator (using the system persistent settings as we did previously proved to be not reliable)
	static UniqueIDGenerator::Shared s_uniqueIDGenerator(new UniqueIDGenerator);

	unsigned ModelObject::GetNextUniqueID()
	{
		if (!s_uniqueIDGenerator)
		{
			std::runtime_error("have not initialized unique id generator! Create One");
			s_uniqueIDGenerator = UniqueIDGenerator::Shared(new UniqueIDGenerator);
		}
		return s_uniqueIDGenerator->fetchOne();
	}

	unsigned ModelObject::GetLastUniqueID()
	{
		return s_uniqueIDGenerator ? s_uniqueIDGenerator->getLast() : 0;
	}

	void ModelObject::setUniqueID(unsigned ID)
	{
		m_uniqueID = ID;

		//updates last unique ID
		if (s_uniqueIDGenerator)
			s_uniqueIDGenerator->update(m_uniqueID);
		else
			std::runtime_error("have not initialized unique id generator! But set unique id already");
	}

	//////////////////////////////////////////////////////////////////////////

	ModelObject::ModelObject(std::string name /*= ""*/)
		: m_name(name.empty() ? "unnamed" : name)
		, m_uniqueID(GetNextUniqueID())
	{}

	ModelObject::ModelObject(const ModelObject& object)
		: m_name(object.m_name)
		, m_uniqueID(GetNextUniqueID())
	{}


	ModelObject::~ModelObject() {

	}

}