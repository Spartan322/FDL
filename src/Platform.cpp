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

CStr convertString(CStr originalStr)
{
	if(originalStr[strlen(originalStr)-1] == '/') return NULL;
	return _convertString_Platform(originalStr);
}

bool createFileNS(CStr path, bool recursive)
{
	_createFile_Platform(path, recursive);
}

bool deleteFileNS(CStr path)
{
	_deleteFile_Platform(path);
}
