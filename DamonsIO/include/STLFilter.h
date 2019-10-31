#ifndef CC_STL_FILTER_HEADER
#define CC_STL_FILTER_HEADER

#include "..\include\FileIOFilter.h"
#include "..\include\qtHeaders.h"
#include "..\..\DamonsDataBase\include\MeshModel.h"

namespace DamonsIO {
	//! StereoLithography file I/O filter
	/** See http://www.ennex.com/~fabbers/StL.asp
	**/

#define STL_LABEL_SIZE  80

	class DAMONS_IO_LIB_API STLFilter : public FileIOFilter
	{

	public:
		STLFilter() :FileIOFilter() {
			uid = "STL_Filter";
		}
	public:

		//static accessors
		static inline std::string GetFileFilter() { return "STL mesh (*.stl)"; }
		//inherited from FileIOFilter
		virtual bool importSupported() const override { return true; }
		virtual bool exportSupported() const override { return true; }
		virtual DAMONS_FILE_ERROR loadFile(const std::string& filename, DMeshLib::ModelObject *&container, LoadParameters& parameters) override;
		virtual DAMONS_FILE_ERROR saveToFile(DMeshLib::ModelObject* entity, const std::string& filename, const SaveParameters& parameters) override;
		virtual  std::string  getFileFilter() const override { return "stl"; }
		virtual std::string getDefaultExtension() const override { return "stl"; }
		virtual bool canLoadExtension(const std::string& upperCaseExt) const override;

	protected:
		bool IsSTLBinary(const char * filename);
		//! Custom save method
		DAMONS_FILE_ERROR saveToASCIIFile(DMeshLib::MeshModel* mesh, FILE *theFile);
		DAMONS_FILE_ERROR saveToBINFile(DMeshLib::MeshModel* mesh, FILE *theFile);

		//! Custom load method for ASCII files
		DAMONS_FILE_ERROR loadASCIIFile(std::string filename,
			DMeshLib::MeshModel* mesh,
			LoadParameters& parameters);

		//! Custom load method for binary files
		DAMONS_FILE_ERROR loadBinaryFile(std::string filename,
			DMeshLib::MeshModel* mesh,
			LoadParameters& parameters);
	};
}
#endif //CC_STL_FILTER_HEADER
