#include <dirent.h>

const char* _convertString_Platform(const char* orignalString)
{
	throw UnsupportedException("String conversion not supported for Posix compliance yet");
	return NULL;
}

bool _createFile_Platform(const char* path, bool recursive)
{
	throw UnsupportedException("File Creation is not supported for Posix compliance yet");
	return false;
}

bool _deleteFile_Platform(const char* path)
{
	throw UnsupportedException("File Deletion is not supported for Posix compliance yet");
	return false;
}

// TODO: Create POSIX handling
