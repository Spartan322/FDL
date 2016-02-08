#include <FDL/FDL.hpp>

#if defined(_FDL_USE_CONFIG)
#	include "FDL_Config.hpp"
#endif

#include <cstring>
#include <cstdio>

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

const char* _convertString_Platform(const char* orignalString);
bool _createFile_Platform(const char* path, bool recursive);
bool _deleteFile_Platform(const char* path);

bool _verifyString(const char* string);
