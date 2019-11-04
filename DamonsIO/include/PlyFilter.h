#ifndef CC_PLY_FILTER_HEADER
#define CC_PLY_FILTER_HEADER

#include "..\include\FileIOFilter.h"
#include "..\..\DamonsDataBase\include\MeshModel.h"
#include "rply.h"

namespace DamonsIO {
	//! PLY format types
	static const char e_ply_type_names[][12] = {
		"PLY_INT8", "PLY_UINT8", "PLY_INT16", "PLY_UINT16",
		"PLY_INT32", "PLY_UIN32", "PLY_FLOAT32", "PLY_FLOAT64",
		"PLY_CHAR", "PLY_UCHAR", "PLY_SHORT", "PLY_USHORT",
		"PLY_INT", "PLY_UINT", "PLY_FLOAT", "PLY_DOUBLE",
		"PLY_LIST"
	};

	//! PLY format storage modes
	static const char e_ply_storage_mode_names[][24] =
	{ "PLY_BIG_ENDIAN","PLY_LITTLE_ENDIAN","PLY_ASCII","PLY_DEFAULT" };

	//! PLY file properties
	struct plyProperty
	{
		p_ply_property prop;
		const char* propName;
		e_ply_type type;
		e_ply_type length_type;
		e_ply_type value_type;
		int elemIndex;
	};

	//! PLY file nuclear element
	struct plyElement
	{
		p_ply_element elem;
		const char* elementName;
		long elementInstances;
		std::vector<plyProperty> properties;
		int propertiesCount;
		bool isFace;
	};

	//! Stanford PLY file I/O filter
	class DAMONS_IO_LIB_API PlyFilter : public FileIOFilter
	{
	public:

		//static accessors
		static inline std::string GetFileFilter() { return "PLY mesh (*.ply)"; }
		static inline std::string GetDefaultExtension() { return "ply"; }
		static void SetDefaultOutputFormat(e_ply_storage_mode format);

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
#endif //CC_PLY_FILTER_HEADER
