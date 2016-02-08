#include <windows.h>

const char* _convertString_Platform(const char* orignalString)
{
	throw UnsupportedException("String Conversion not supported on Windows yet");
	return NULL;
}

bool _createFile_Platform(const char* path, bool recursive)
{
	throw UnsupportedException("File Creation is not supported on Windows yet");
	return false;
}

bool _deleteFile_Platform(const char* path)
{
	throw UnsupportedException("File Deletion is not supported on Windows yet");
	return false;
}

// TODO: Create Windows Handling
