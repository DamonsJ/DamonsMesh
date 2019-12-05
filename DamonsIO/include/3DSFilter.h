#ifndef CC_3DS_FILTER_HEADER
#define CC_3DS_FILTER_HEADER

#include "..\include\FileIOFilter.h"
#include "..\..\DamonsDataBase\include\MeshModel.h"

// lib3ds headers
#include "..\include\lib3ds\file.h"                        
#include "..\include\lib3ds\camera.h"
#include "..\include\lib3ds\mesh.h"
#include "..\include\lib3ds\node.h"
#include "..\include\lib3ds\material.h"
#include "..\include\lib3ds\matrix.h"
#include "..\include\lib3ds\vector.h"
#include "..\include\lib3ds\light.h"

namespace DamonsIO {
	//! 3ds file filter

	class DAMONS_IO_LIB_API TDSFilter : public FileIOFilter
	{

	public:
		TDSFilter() :FileIOFilter() {
			uid = "3DS_Filter";
			numVertices = 0;
			numFaces = 0;
		}
	public:

		//inherited from FileIOFilter
		virtual bool importSupported() const override { return true; }
		virtual bool exportSupported() const override { return true; }
		virtual DAMONS_FILE_ERROR loadFile(const std::string& filename, DMeshLib::ModelObject *&container, LoadParameters& parameters) override;
		virtual DAMONS_FILE_ERROR saveToFile(DMeshLib::ModelObject* entity, const std::string& filename, const SaveParameters& parameters) override;
		virtual std::string getFileFilter() const override { return "3ds mesh (*.3ds)"; }
		virtual std::string getDefaultExtension() const override { return "3ds"; }
		virtual bool canLoadExtension(const std::string& upperCaseExt) const override;

	protected:
		//! Custom save method
		

		//! Custom load method 
		DAMONS_FILE_ERROR loadFileImpl(Lib3dsFile *file, 
									Lib3dsNode *node,
									DMeshLib::MeshModel* mesh,
									LoadParameters& parameters);

		DAMONS_FILE_ERROR ReadNode(Lib3dsFile *file,Lib3dsNode *node, DMeshLib::MeshModel* _dmesh);

		int numVertices;
		int numFaces;
	};
}
#endif //CC_STL_FILTER_HEADER
