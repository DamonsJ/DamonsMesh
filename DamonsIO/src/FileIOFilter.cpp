#include "..\include\FileIOFilter.h"
#include "..\include\STLFilter.h"
#include "..\include\OFFFilter.h"
#include <fstream>

//system
#include <cassert>
#include <vector>

namespace DamonsIO {
	//! Available filters
	/** Filters are uniquely recognized by their 'file filter' string.
		We use a std::vector so as to keep the insertion ordering!
	**/
	static FileIOFilter::FilterContainer s_ioFilters;

	std::vector< std::string > FileIOFilter::getFileFilters() const
	{
		std::vector<std::string > vec;

		for (FilterContainer::const_iterator it = s_ioFilters.begin(); it != s_ioFilters.end(); ++it)
		{
			std::string  otherFilters = (*it)->getFileFilter();
			vec.push_back(otherFilters);
		}

		return vec;
	}

	void FileIOFilter::InitInternalFilters()
	{
		//from the most useful to the less one!
		//Register(Shared(new AsciiFilter()));
		//Register(Shared(new PlyFilter()));
		Register(Shared(new STLFilter()));
		Register(Shared(new OFFFilter()));
		//Register(Shared(new OBJFilter()));
	}

	void FileIOFilter::Register(Shared filter)
	{
		if (!filter)
		{
			assert(false);
			return;
		}

		// check for an existing copy of this filter or one with the same ID
		const std::string id = filter->uid;

		auto compareFilters = [filter, id](const Shared& filter2)
		{
			return (filter == filter2) || (filter2->uid == id);
		};

		if (std::any_of(s_ioFilters.cbegin(), s_ioFilters.cend(), compareFilters))
		{
			return;
		}
		s_ioFilters.push_back(filter);
	}

	void FileIOFilter::UnregisterAll()
	{
		for (auto & filter : s_ioFilters)
		{
			filter->unregister();
		}

		s_ioFilters.clear();
	}

	FileIOFilter::Shared FileIOFilter::GetFilter(const std::string& fileFilter)
	{
		if (!fileFilter.empty())
		{
			for (FilterContainer::const_iterator it = s_ioFilters.begin(); it != s_ioFilters.end(); ++it)
			{
				std::string otherFilters = (*it)->getFileFilter();
				if (otherFilters == fileFilter)
					return  *it;
			}
		}

		return Shared(nullptr);
	}

	const FileIOFilter::FilterContainer& FileIOFilter::GetFilters()
	{
		return s_ioFilters;
	}

	const std::vector< std::string > FileIOFilter::GetAllFilters() {
		std::vector<std::string > vec;

		for (FilterContainer::const_iterator it = s_ioFilters.begin(); it != s_ioFilters.end(); ++it)
		{
			std::string  otherFilters = (*it)->getFileFilter();
			vec.push_back(otherFilters);
		}

		return vec;
	}

	FileIOFilter::Shared FileIOFilter::FindBestFilterForExtension(const std::string& ext)
	{
		std::string lowerEXT = ext;
		std::transform(lowerEXT.begin(), lowerEXT.end(), lowerEXT.begin(), ::tolower);

		for (const auto &filter : s_ioFilters)
		{
			std::string otherFilters = filter->getDefaultExtension();
			if (otherFilters == lowerEXT)
				return  filter;
		}

		return FileIOFilter::Shared(nullptr);
	}

	DMeshLib::ModelObject* FileIOFilter::LoadFromFile(
		const std::string& filename,
		LoadParameters& loadParameters,
		Shared filter,
		DAMONS_FILE_ERROR& result)
	{
		if (!filter)
		{
			result = CC_FERR_CONSOLE_ERROR;
			assert(false);
			return nullptr;
		}

		//check file existence
		std::ifstream fi(filename);
		if (!fi.good())
		{
			fi.close();
			result = CC_FERR_CONSOLE_ERROR;
			return nullptr;
		}
		fi.close();
		DMeshLib::ModelObject *mobj = nullptr;
		try
		{
			result = filter->loadFile(filename, mobj,loadParameters);
		}
		catch (const std::exception& e)
		{
			if (mobj)
			{
				delete mobj;
			}
			result = CC_FERR_CONSOLE_ERROR;
		}
		catch (...)
		{
			if (mobj)
			{
				delete mobj;
			}
			result = CC_FERR_CONSOLE_ERROR;
		}

		if (result != CC_FERR_NO_ERROR)
		{
			std::cout << " error happened" << std::endl;
		}

		return mobj;
	}


	DMeshLib::ModelObject* FileIOFilter::LoadFromFile(
		const std::string& filename,
		LoadParameters& loadParameters,
		DAMONS_FILE_ERROR& result,
		const std::string fileFilter)
	{
		Shared filter(nullptr);

		//if the right filter is specified by the caller
		if (!fileFilter.empty())
		{
			filter = GetFilter(fileFilter);
			if (!filter)
			{
				result = CC_FERR_CONSOLE_ERROR;
				return nullptr;
			}
		}
		else //we need to guess the I/O filter based on the file format
		{
			//look for file extension (we trust Qt on this task)
			std::string::size_type pos = filename.find_last_of(".");
			std::string extension = filename.substr(pos + 1);
			if (pos == std::string::npos || extension.empty())
			{
				result = CC_FERR_CONSOLE_ERROR;
				return nullptr;
			}
			//convert extension to file format
			filter = FindBestFilterForExtension(extension);
			//unknown extension?
			if (!filter)
			{
				result = CC_FERR_CONSOLE_ERROR;
				return nullptr;
			}
		}

		return LoadFromFile(filename, loadParameters, filter, result);
	}

	DAMONS_FILE_ERROR FileIOFilter::SaveToFile(
		DMeshLib::ModelObject* entities,
		const std::string& filename,
		const SaveParameters& parameters,
		Shared filter)
	{
		if (!entities || filename.empty() || !filter)
			return CC_FERR_BAD_ARGUMENT;

		//if the file name has no extension, we had a default one!
		std::string completeFileName(filename);
		std::string::size_type pos = completeFileName.find_last_of(".");

		if (std::string::npos == pos)
			completeFileName += filter->getDefaultExtension();

		DAMONS_FILE_ERROR result = CC_FERR_NO_ERROR;
		try
		{
			result = filter->saveToFile(entities, completeFileName, parameters);
		}
		catch (...)
		{
			std::cout << "[I/O] CC has caught an unhandled exception while saving file " + filename << std::endl;
			result = CC_FERR_CONSOLE_ERROR;
		}

		if (result == CC_FERR_NO_ERROR)
		{
			std::cout << "[I/O] File '%1' saved successfully" + filename << std::endl;
		}

		return result;
	}

	DAMONS_FILE_ERROR FileIOFilter::SaveToFile(
		DMeshLib::ModelObject* entities,
		const std::string& filename,
		const SaveParameters& parameters,
		const std::string& fileFilter)
	{
		if (fileFilter.empty())
			return CC_FERR_BAD_ARGUMENT;

		Shared filter = GetFilter(fileFilter);
		if (!filter)
		{
			std::cout << "[Load] Internal error: no filter corresponds to filter  " + fileFilter << std::endl;
			return CC_FERR_UNKNOWN_FILE;
		}

		return SaveToFile(entities, filename, parameters, filter);
	}

}