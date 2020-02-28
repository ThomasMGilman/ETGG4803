#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

#ifdef _WINDOWS
	#include <direct.h>
	#define GET_CURRENT_DIR _getcwd
#else
	#include <unistd.h>
	#define GET_CURRENT_DIR getcwd
#endif


class File_Editer
{
private:
	std::fstream mFileEditor;
protected:
public:
	File_Editer(std::string fileName, bool newFile, std::string path = NULL)
	{
		if (!fileName.empty())
		{
			mFileEditor.open(fileName.c_str() + path.empty() ? path.c_str() : "", std::ios::in | std::ios::out);
		}
		else
			throw new std::exception("Empty File Name Error!! Need to pass a valid file name of atleast 1 char!");
	}

	/// Check if the file at the specified path exists
	inline bool file_exists(const std::string& name, const std::string& path = _)
	{
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}
};
