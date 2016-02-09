#include <FDL/FDL.hpp>

#include <cstring>
#include <cstdio>

///////////////////////////////////////
//	Platform Definitions
///////////////////////////////////////

//	Converts String to acceptable path string
const char* _convertString_Platform(const char* orignalString);
//	Creates file based on the path, supports recursion
bool _createFile_Platform(const char* path, bool recursive);
//	Deletes a file based on path
bool _deleteFile_Platform(const char* path);

//	Verify a string can be inserted as a path
bool _verifyString(const char* string);
