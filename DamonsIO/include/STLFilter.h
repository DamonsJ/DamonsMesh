//##########################################################################
//#                                                                        #
//#                              CLOUDCOMPARE                              #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 or later of the License.      #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          COPYRIGHT: EDF R&D / TELECOM ParisTech (ENST-TSI)             #
//#                                                                        #
//##########################################################################

#ifndef CC_STL_FILTER_HEADER
#define CC_STL_FILTER_HEADER

#include "..\include\FileIOFilter.h"
#include "..\include\qtHeaders.h"
#include "..\..\DamonsDataBase\include\MeshModel.h"

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
	virtual CC_FILE_ERROR loadFile(const std::string& filename, ModelObject *&container, LoadParameters& parameters) override;
	virtual CC_FILE_ERROR saveToFile(ModelObject* entity, const std::string& filename, const SaveParameters& parameters) override;
	virtual  std::string  getFileFilter(bool onImport) const override {  return "stl";}
	virtual std::string getDefaultExtension() const override { return "stl"; }
	virtual bool canLoadExtension(const std::string& upperCaseExt) const override;
	virtual bool canSave(DB_CLASS_ENUM type, bool& multiple, bool& exclusive) const override;

protected:
	bool IsSTLBinary(const char * filename);
	//! Custom save method
	CC_FILE_ERROR saveToASCIIFile(MeshModel* mesh, FILE *theFile, QWidget* parentWidget = 0);
	CC_FILE_ERROR saveToBINFile(MeshModel* mesh, FILE *theFile, QWidget* parentWidget = 0);

	//! Custom load method for ASCII files
	CC_FILE_ERROR loadASCIIFile(std::string filename,
								MeshModel* mesh,
								LoadParameters& parameters);

	//! Custom load method for binary files
	CC_FILE_ERROR loadBinaryFile(std::string filename,
								MeshModel* mesh,
								LoadParameters& parameters);
};

#endif //CC_STL_FILTER_HEADER
