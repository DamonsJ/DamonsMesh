#ifndef _DAMONS_OBJECT_H_
#define _DAMONS_OBJECT_H_

namespace DGraphic {

	class DObject
	{
	public:
		DObject() :objID(0) {}
		~DObject() {}
	public:
		void SetID(unsigned int _id) { objID = _id; }
	protected:
		unsigned int objID;
	};
};

#endif