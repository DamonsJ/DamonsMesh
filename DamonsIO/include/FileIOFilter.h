#ifndef _FILEIOFILTER_HEADER_
#define _FILEIOFILTER_HEADER_

#include <memory>

#include "..\include\damons_io.h"

#include "..\..\DamonsDataBase\include\ModelObject.h"

namespace DamonsIO {
	//! Typical I/O filter errors
	enum DAMONS_FILE_ERROR {
		CC_FERR_NO_ERROR,
		CC_FERR_BAD_ARGUMENT,
		CC_FERR_UNKNOWN_FILE,
		CC_FERR_WRONG_FILE_TYPE,
		CC_FERR_WRITING,
		CC_FERR_READING,
		CC_FERR_NO_SAVE,
		CC_FERR_NO_LOAD,
		CC_FERR_BAD_ENTITY_TYPE,
		CC_FERR_CANCELED_BY_USER,
		CC_FERR_NOT_ENOUGH_MEMORY,
		CC_FERR_MALFORMED_FILE,
		CC_FERR_CONSOLE_ERROR,
		CC_FERR_BROKEN_DEPENDENCY_ERROR,
		CC_FERR_FILE_WAS_WRITTEN_BY_UNKNOWN_PLUGIN,
		CC_FERR_THIRD_PARTY_LIB_FAILURE,
		CC_FERR_THIRD_PARTY_LIB_EXCEPTION,
		CC_FERR_NOT_IMPLEMENTED,
	};

	/*!
	 * \class FileIOFilter
	 *
	 * \brief Generic file I/O filter
	 *        Gives static access to file loader.
	 *		  Must be implemented by any specific I/O filter.
	 * \author Damons
	 * \date Ê®ÔÂ 2019
	 */

	class FileIOFilter
	{
	public: 
		//initialization
		FileIOFilter() {}
		//! Destructor
		virtual ~FileIOFilter() {}

		//! Generic loading parameters
		struct LoadParameters
		{
			//! Default constructor
			LoadParameters()
				: alwaysDisplayLoadDialog(true)
				, preserveShiftOnSave(true)
				, autoComputeNormals(false)
				, sessionStart(true)
				, process(0.0)
			{}

			//! Wether to always display a dialog (if any), even if automatic guess is possible
			bool alwaysDisplayLoadDialog;
			//! If applicable, whether shift should be preserved or not (optional)
			bool preserveShiftOnSave;
			//! Whether normals should be computed at loading time (if possible - e.g. for gridded clouds) or not
			bool autoComputeNormals;
			//! Session start (whether the load action is the first of a session)
			bool sessionStart;
			//! load process
			double process;
		};

		//! Generic saving parameters
		struct SaveParameters
		{
			//! Default constructor
			SaveParameters()
				: alwaysDisplaySaveDialog(true)
			{}

			//! Wether to always display a dialog (if any), even if automatic guess is possible
			bool alwaysDisplaySaveDialog;
		};

		//! Shared type
		typedef std::shared_ptr<FileIOFilter> Shared;

	public: 

		//************************************  
		// @brief : Returns whether this I/O filter can import files 
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: void
		// @param : void  
		//************************************ 
		virtual bool importSupported() const { return false; }

		//************************************  
		// @brief : Loads one entity from a file 
		//			This method must be implemented by children classes.
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: error
		// @param:  filename file to load
		// @param:  container container to store loaded entities
		// @param:  parameters generic loading parameters
		//************************************ 
		virtual DAMONS_FILE_ERROR loadFile(const std::string& filename,
											DMeshLib::ModelObject *&container,
											LoadParameters& parameters)
		{
			return CC_FERR_NOT_IMPLEMENTED;
		}

		//! Returns whether this I/O filter can export files
		virtual bool exportSupported() const { return false; }

		//************************************  
		// @brief : Saves an entity (or a group of) to a file 
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @param : entity entity(or group of) to save
		// @param : filename filename
		// @param : parameters generic saving parameters
		// @return :error
		//************************************ 
		virtual DAMONS_FILE_ERROR saveToFile(
			DMeshLib::ModelObject* entity,
			const std::string& filename,
			const SaveParameters& parameters)
		{
			return CC_FERR_NOT_IMPLEMENTED;
		}

		//************************************  
		// @brief : Returns the file filter(s) for this I/O filter
		//			E.g. 'ASCII file (*.asc)'
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: list of filters
		// @param :   
		//************************************ 
		virtual std::string  getFileFilter() const = 0;
		std::vector< std::string > getFileFilters() const;

		//! Returns the default file extension
		virtual std::string getDefaultExtension() const = 0;

		//! Returns whether a specific file can be loaded by this filter
		/** Used when a file is dragged over the application window for instance.
			Should remain simple (guess based on the file extension, etc.)
		**/
		//virtual bool canLoad(QString filename) const = 0;

		//! Returns whether a specific extension can be loaded by this filter
		/** \param upperCaseExt upper case extension
			\return whether the extension is (theoretically) handled by this filter
		**/
		virtual bool canLoadExtension(const std::string& upperCaseExt) const = 0;

	public: 

		//************************************  
		//  @brief :  Loads one or more entities from a file with a known filter
		//			  Shortcut to FileIOFilter::loadFile
		//  @author: SunHongLei
		//  @date  : 2019/10/31  
		//	@param : filename filename
		//	@param : parameters generic loading parameters
		//	@param : filter input filter
		//	@param : [out] result file error code
		//	@return: loaded entities (or 0 if an error occurred)
		//************************************ 

		DAMONS_IO_LIB_API static DMeshLib::ModelObject* LoadFromFile(
														const std::string& filename,
														LoadParameters& parameters,
														Shared filter,
														DAMONS_FILE_ERROR& result);

		//************************************  
		// @brief : Loads one or more entities from a file with known type 
		//			Shortcut to the other version of FileIOFilter::LoadFromFile	
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// @return: loaded entities (or 0 if an error occurred)
		// @param : filename filename
		// @param : parameters generic loading parameters  
		// @param : result file error code 
		// @param : fileFilter input filter 'file filter' (if empty, the best I/O filter will be guessed from the file extension)
		//************************************ 
		DAMONS_IO_LIB_API static DMeshLib::ModelObject* LoadFromFile(
			const std::string& filename,
			LoadParameters& parameters,
			DAMONS_FILE_ERROR& result,
			std::string fileFilter = "");

		//************************************  
		// @brief : Saves an entity (or a group of) to a specific file thanks to a given filter 
		//			Shortcut to FileIOFilter::saveFile
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// param  : entities entity to save (can be a group of other entities)
		// param  : filename filename
		// param  : parameters saving parameters
		// param  : filter output filter
		// return : error type(if any)
		//************************************ 
		DAMONS_IO_LIB_API static DAMONS_FILE_ERROR SaveToFile(
			DMeshLib::ModelObject* entities,
			const std::string& filename,
			const SaveParameters& parameters,
			Shared filter);

		//************************************  
		// @brief : Saves an entity (or a group of) to a specific file thanks to a given filter 
		//			Shortcut to FileIOFilter::saveFile
		// @author: SunHongLei
		// @date  : 2019/10/31  
		// param  : entities entity to save (can be a group of other entities)
		// param  : filename filename
		// param  : parameters saving parameters
		// param  : fileFilter output filter 'file filter'
		// return : error type(if any)
		//************************************ 
		DAMONS_IO_LIB_API static DAMONS_FILE_ERROR SaveToFile(
			DMeshLib::ModelObject* entities,
			const std::string& filename,
			const SaveParameters& parameters,
			const std::string& fileFilter);

	public: //global filters registration mechanism

		//! Init internal filters (should be called once)
		DAMONS_IO_LIB_API static void InitInternalFilters();

		//! Registers a new filter
		DAMONS_IO_LIB_API static void Register(Shared filter);

		//! Returns the filter corresponding to the given 'file filter'
		DAMONS_IO_LIB_API static Shared GetFilter(const std::string& fileFilter);

		//! Returns the best filter (presumably) to open a given file extension
		DAMONS_IO_LIB_API static Shared FindBestFilterForExtension(const std::string& ext);

		//! Type of a I/O filters container
		typedef std::vector< FileIOFilter::Shared > FilterContainer;

		//! Returns the set of all registered filters
		DAMONS_IO_LIB_API static const FilterContainer& GetFilters();
		DAMONS_IO_LIB_API static const std::vector< std::string > GetAllFilters();
		//! Unregisters all filters
		/** Should be called at the end of the application
		**/
		DAMONS_IO_LIB_API static void UnregisterAll();

		//! Called when the filter is unregistered
		/** Does nothing by default **/
		virtual void unregister() {}

	protected:
		std::string  uid;
	};
}
#endif
