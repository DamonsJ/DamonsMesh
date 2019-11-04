#ifndef CC_OBJ_FILTER_HEADER
#define CC_OBJ_FILTER_HEADER

#include "..\include\FileIOFilter.h"
#include "..\..\DamonsDataBase\include\MeshModel.h"

namespace DamonsIO {
	//! Wavefront meshes file I/O filter
	class DAMONS_IO_LIB_API ObjFilter : public FileIOFilter
	{
	public:
		ObjFilter() :FileIOFilter() {
			uid = "OBJ_Filter";
		}
	public:

		//static accessors
		static inline std::string GetFileFilter() { return "OBJ mesh (*.obj)"; }
		static inline std::string GetDefaultExtension() { return "obj"; }

		//inherited from FileIOFilter
		virtual bool importSupported() const override { return true; }
		virtual bool exportSupported() const override { return true; }
		virtual DAMONS_FILE_ERROR loadFile(const std::string& filename, DMeshLib::ModelObject *&container, LoadParameters& parameters) override;
		virtual DAMONS_FILE_ERROR saveToFile(DMeshLib::ModelObject* entity, const std::string& filename, const SaveParameters& parameters) override;
		virtual std::string getFileFilter() const override { return GetFileFilter(); }
		virtual std::string getDefaultExtension() const override { return GetDefaultExtension(); }
		virtual bool canLoadExtension(const std::string& upperCaseExt) const override;

	};
}
#endif //CC_OBJ_FILTER_HEADER
