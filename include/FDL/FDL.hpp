#ifndef _FDL_INCLUDE_
#define _FDL_INCLUDE_

#include <exception>

/* Snippet from GLFW */
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
#	define _WIN32
#endif /* _WIN32 */

#if defined(FDL_DLL) && defined(_FDL_BUILD_DLL)
 	/* FDL_DLL must be defined by applications that are linking against the DLL
 	* version of the FDL.  _FDL_BUILD_DLL is defined by the FDL
 	* configuration header when compiling the DLL version of the library.
 	*/
#	error "You must not have both FDL_DLL and _FDL_BUILD_DLL defined"
#endif

#if defined(_WIN32) && defined(_FDL_BUILD_DLL)
 	/* We are building FDL as a Win32 DLL */
#	define FDLAPI __declspec(dllexport)
#elif defined(_WIN32) && defined(FDL_DLL)
 	/* We are calling FDL as a Win32 DLL */
#	define FDLAPI __declspec(dllimport)
#elif defined(__GNUC__) && defined(_FDL_BUILD_DLL)
 	/* We are building FDL as a shared / dynamic library */
#	define FDLAPI __attribute__((visibility("default")))
#else
	/* Building or calling FDL as static */
#	define FDLAPI
#endif
/* End of GLFW Snippet */

#define FDL_TRUE	1
#define FDL_FALSE	0

#define FDL_EXCEPTION_CREATE(EXCEPT_NAME) \
class FDLAPI EXCEPT_NAME : public FDL::Exception \
{ \
public: \
EXCEPT_NAME(CStr message=""); \
~EXCEPT_NAME(); \
} \

#define FDL_EXCEPTION_CREATE_EXTEND(EXCEPT_NAME, EXTEND_NAME) \
class FDLAPI EXCEPT_NAME : public EXTEND_NAME \
{
public: \
EXCEPT_NAME(CStr message="");
~EXCEPT_NAME();
} \

namespace FDL {

typedef const char* CStr;
typedef const char* Bytes;

/// \brief An 8 bit signed integer
typedef signed char Int8;
/// \brief An 8 bit unsigned integer
typedef unsigned char Uint8;

///	\brief	A 16 bit signed integer
typedef signed   short Int16;
///	\brief	A 16 bit unsigned integer
typedef unsigned short Uint16;

///	\brief	A 32 bit signed integer
typedef signed   int Int32;
///	\brief	A 32 bit unsigned integer
typedef unsigned int Uint32;

#if defined(_MSC_VER)
///	\brief	A 64 bit signed integer
typedef signed   __int64 Int64;
///	\brief	A 64 bit unsigned integer
typedef unsigned __int64 Uint64;
#else
///	\brief	A 64 bit signed integer
typedef signed   long long Int64;
///	\brief	A 64 bit unsigned integer
typedef unsigned long long Uint64;
#endif

class Exception;
class File;
class Directory;
class FileStream;
class StreamHandler;

class FileManager
{
private:

std::vector<StreamHandler> m_streamHandlerList;
public:

bool registerStreamHandler(StreamHandler handler);

StreamHandler getStreamHandlerFor(File file);
} manager;

class Exception : public std::exception
{
protected:

CStr m_message;
public:

////////////////////////////////////////////////////////
///	\brief	Constructor for an Exception
///
///	\param	message	The message to assign
///
////////////////////////////////////////////////////////
virtual Exception(CStr message="");
////////////////////////////////////////////////////////
///	\brief	The Default Destructor
///
////////////////////////////////////////////////////////
virtual ~Exception();
////////////////////////////////////////////////////////
///	\brief	return the message
///
////////////////////////////////////////////////////////
CStr what();
////////////////////////////////////////////////////////
///	\brief	Casts the exception to return the message into a new string
///
////////////////////////////////////////////////////////
CStr operator CStr();
////////////////////////////////////////////////////////
///	\brief	Assigns a message
///
///	\param	message	The message to assign
///
////////////////////////////////////////////////////////
Exception operator=(CStr message);
};

class FDLAPI File
{
protected:

const char* m_fullPath;
public:

FDL_EXCEPTION_CREATE(FileFailException);
FDL_EXCEPTION_CREATE_EXTEND(FileMissingException, FileFailException);

////////////////////////////////////////////////////////
///	\brief	Constructor for a File
///
///	\param	path	The path File points to, relative or absoulte, empty
///		points to working directory
///
////////////////////////////////////////////////////////
File(CStr path = "");
////////////////////////////////////////////////////////
///	\brief	Constructor for a File
///
///	\param	root	The root to start in
///	\param	path	The path File points to according to root
///
////////////////////////////////////////////////////////
File(CStr root, CStr path);
////////////////////////////////////////////////////////
///	\brief	Constructor for a File
///
///	\param	root	The root to start in
///	\param	path	The path File points to according to root
///
////////////////////////////////////////////////////////
File(File root, CStr path="");
////////////////////////////////////////////////////////
///	\brief	Default destructor
///
////////////////////////////////////////////////////////
~File();
////////////////////////////////////////////////////////
///	\brief	Retrieves the full path of the file
///
////////////////////////////////////////////////////////
CStr getFullPath();
////////////////////////////////////////////////////////
///	\brief	Retrieves the extension of the file
///
///	\note	return NULL if isDirectory is true
///
////////////////////////////////////////////////////////
CStr getExtension();
////////////////////////////////////////////////////////
///	\brief Retrieves the root path of the file
///
////////////////////////////////////////////////////////
CStr getRootPath();
////////////////////////////////////////////////////////
///	\brief	Retrieves the file name
///
////////////////////////////////////////////////////////
CStr getFileName();
////////////////////////////////////////////////////////
///	\brief	Retrieves the file name and extension
///
////////////////////////////////////////////////////////
CStr getFullName();
////////////////////////////////////////////////////////
///	\brief	Retrieves the byte size of the File
///
///	\throws	File::FileFailException	If size retevial can't be done
///	\throws	File::FileMissingException	If file does not exist
///
////////////////////////////////////////////////////////
UInt64 getSize();
////////////////////////////////////////////////////////
///	\brief	Whether the file exists
///
////////////////////////////////////////////////////////
bool doesExist();
////////////////////////////////////////////////////////
///	\brief	Whether the file is a directory
///
////////////////////////////////////////////////////////
virtual bool isDirectory();
////////////////////////////////////////////////////////
///	\brief	Whether the file is a binary file, according to extension
///
///	\note	Does not actually check File for binary designation
///
////////////////////////////////////////////////////////
virtual bool isBinary();
////////////////////////////////////////////////////////
///	\brief	Creates the file according to the path
///
///	\param	recursive	Whether recursive creation is done
///
///	\throws	File::FileFailException	If File can not be created
///	\throws	File::FileMissingException	If recursive is false and path does
///		not exist
///
///	\return	Whether the file creation succeeded
////////////////////////////////////////////////////////
bool create(bool recursive=true);
////////////////////////////////////////////////////////
///	\brief	Deletes the file according to the path
///
///	\return	Whether the file deletion succeeded
////////////////////////////////////////////////////////
bool delete();
////////////////////////////////////////////////////////
///	\brief	Moves the file according to newPath
///
///	\throws	File::FileFailException	If newFile can't be created
///	\throws File::FileMissingException	If recursive is false and newFile's
///		root path does not exist
///
///	\return	Whether the file move succeeded
////////////////////////////////////////////////////////
bool move(File newFile, bool recursive=true);
////////////////////////////////////////////////////////
///	\brief	Opens the File in a stream
///
///	\throws	File::FileFailException	If FileStream can't be opened
///	\throws File::FileMissingException	If File does not exist
///
///	\return	A FileStream pointing to the File's stream
////////////////////////////////////////////////////////
FileStream open();
////////////////////////////////////////////////////////
///	\brief	Converts the File to an appropriate OS native path
///
///	\return	On Unix, return m_fullPath. On Windows, return Windows compliant
///		version of m_fullPath
////////////////////////////////////////////////////////
CStr toNativePath();
////////////////////////////////////////////////////////
///	\brief	Retrieves a File based on a native path
///
///	\return	On Unix, assigns path to the File. On Windows, converts Windows
///		path to Unix compliant path
////////////////////////////////////////////////////////
static File convertFromNativePath(CStr path);
};

class Directory : public File
{
public:

Directory(CStr path="");
Directory(CStr root, CStr path);
Directory(File root, CStr path="");

File open(CStr path);
};

class FileStream
{
private:

File m_file;
std::fstream* mp_fileStream;
public:

FDL_EXCEPTION_CREATE(EOSException); // End Of Stream Exception
FDL_EXCEPTION_CREATE(IsDirectoryException);

////////////////////////////////////////////////////////
///	\brief	Constructor for FileStream, assumes binary
///		through file.isBinary()
///
///	\param	path	The file to open
///
///	\throws FileStream::IsDirectoryException	If FileStream attempts to open
///		a directory, will not cause memory leaks
///
////////////////////////////////////////////////////////
FileStream(File file);
////////////////////////////////////////////////////////
///	\brief	Constructor for FileStream, explicitedly handles binary files
///
///	\param	path	The file to open
///	\param	handleBinary	Whether file is handled as binary file
///
///	\throws FileStream::IsDirectoryException	If FileStream attempts to open
///		a directory, will not cause memory leaks
///
////////////////////////////////////////////////////////
FileStream(File file, bool handleBinary);

~FileStream();

bool open();

bool isOpen();

void close();

void write(Bytes data, Int64 size=-1);
Int64 read(Bytes data);

void seekWrite(UInt64 position);
UInt64 tellWrite();

void seekRead(UInt64 position);
UInt64 tellRead();

bool flush();
};

class FileStreamHandler
{
protected:

File m_file;
public:

virtual FileStreamHandler(File file) = 0;

virtual ~FileStreamHandler() {};

virtual UInt64 read(Bytes data, UInt64 size) = 0;

virtual UInt64 seek(UInt64 position) = 0;

virtual UInt64 tell() = 0;

virtual bool canHandleFile();
};

} /* namespace FDL */

#endif /* _FDL_INCLUDE_ */
