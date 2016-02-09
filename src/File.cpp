#include "Platform.hpp"

using namespace FDL;

File::File(CStr path) : m_fullPath(convertString(path))
{
	if(!_verifyString(m_fullPath))
	{
		free(m_fullPath)
		throw BadPathException("m_fullPath is invalid");
	}
}

File::File(CStr root, CStr path)
{
	if(sprintf(m_fullPath, "%s/%s", root, path) < 0 || !_verifyString(convertString(m_fullPath)))
	{
		throw BadPathException("m_fullPath could not be formatted");
	}
}

File::File(File root, CStr path): File::File(root.getFullPath(), path) {}

File::~File()
{}

String const File::getFullPath() const
{
	return String(m_fullPath);
}

String const File::getExtension() const
{
	char* path = strchr(m_fullPath, '.');
	return String(++path);
}
