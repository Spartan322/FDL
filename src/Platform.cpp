#include "Platform.hpp"

using namespace FDL;

// TODO: Create Generic callable functions
// Example: CreateFile would exist, but is first handled outside of OS calls, then handed to a _CreateFile function call

bool isWindows()
{
	return FDL_IS_WINDOWS;
}

bool isPosix()
{
	return FDL_IS_POSIX;
}

String convertString(String originalStr)
{
	if(originalStr[originalStr.size()-1] == '/') return String::null_str;
	return _convertString_Platform(originalStr);
}

bool createFileNS(String path, bool recursive)
{
	return _createFile_Platform(path, recursive);
}

bool deleteFileNS(String path)
{
	return _deleteFile_Platform(path);
}
