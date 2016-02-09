#ifndef _FDL_NO_CONFIG
#	include "FDL_Config.hpp"
#endif

#ifdef _FDL_POSIX
#	include "Platform_Posix.hpp"
#	define FDL_IS_POSIX true
#	define FDL_IS_WINDOWS false
#elif defined(_FDL_WINDOWS)
#	include "Platform_Windows.hpp"
#	define FDL_IS_POSIX false
#	define FDL_IS_WINDOWS true
#else
#	error "Platform not designated, FDL failed"
#endif

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
