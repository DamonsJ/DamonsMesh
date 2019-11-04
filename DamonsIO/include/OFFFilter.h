#ifndef CC_OFF_FILTER_HEADER
#define CC_OFF_FILTER_HEADER

#include "..\include\FileIOFilter.h"
#include "..\..\DamonsDataBase\include\MeshModel.h"

namespace DamonsIO {
	//! OFF file I/O filter
	/** See http://people.sc.fsu.edu/~jburkardt/data/off/off.html
	**/
	class DAMONS_IO_LIB_API OFFFilter : public FileIOFilter
	{
	public:
		OFFFilter() :FileIOFilter() {
			uid = "OFF_Filter";
		}
	public:

		//inherited from FileIOFilter
		virtual bool importSupported() const override { return true; }
		virtual bool exportSupported() const override { return true; }
		virtual DAMONS_FILE_ERROR loadFile(const std::string& filename, DMeshLib::ModelObject *&container, LoadParameters& parameters) override;
		virtual DAMONS_FILE_ERROR saveToFile(DMeshLib::ModelObject* entity, const std::string& filename, const SaveParameters& parameters) override;
		virtual std::string getFileFilter() const override { return "OFF mesh (*.off)"; }
		virtual std::string getDefaultExtension() const override { return "off"; }
		virtual bool canLoadExtension(const std::string& upperCaseExt) const override;

	};
}
#endif //CC_OFF_FILTER_HEADER
