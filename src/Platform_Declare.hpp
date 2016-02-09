#ifndef _FDL_PLATFORM_DECLARE_H
#define _FDL_PLATFORM_DECLARE_H

#include <FDL/FDL.hpp>

#include <cstring>
#include <cstdio>

#if !defined(_FDL_POSIX) && !defined(_FDL_WINDOWS)
#	error "Filesystem Type not designated, FDL failed"
#endif

///////////////////////////////////////
//	Platform Declarations
///////////////////////////////////////

//	Converts String to acceptable path string
const char* _convertString_Platform(const char* orignalString);
//	Creates file based on the path, supports recursion
bool _createFile_Platform(const char* path, bool recursive);
//	Deletes a file based on path
bool _deleteFile_Platform(const char* path);

//	Verify a string can be inserted as a path
bool _verifyString(const char* string);

#endif /* _FDL_PLATFORM_DECLARE_H */
