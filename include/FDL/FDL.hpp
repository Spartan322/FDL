#ifndef _FDL_INCLUDE_
#define _FDL_INCLUDE_

#include <cstring>
#include <iterator>
#include <fstream>
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

#define FDL_EXCEPTION_CREATE_EXTEND(EXCEPT_NAME, EXTEND_NAME) \
class FDLAPI EXCEPT_NAME : public EXTEND_NAME \
{ \
public: \
EXCEPT_NAME(CStr message="") : EXTEND_NAME(message) {} \
~EXCEPT_NAME() { ~EXTEND_NAME(); } \
} \

#define FDL_EXCEPTION_CREATE(EXCEPT_NAME) FDL_EXCEPTION_CREATE_EXTEND(EXCEPT_NAME, FDL::Exception)

namespace FDL {

typedef const char* CStr;
typedef const char* Bytes;

//	8 bit integers
typedef signed char Int8;
typedef unsigned char Uint8;

//	16 bit integers
typedef signed   short Int16;
typedef unsigned short Uint16;

//	32 bit integers
typedef signed   int Int32;
typedef unsigned int Uint32;

#if defined(_MSC_VER)
//	64 bit integers
typedef signed   __int64 Int64;
typedef unsigned __int64 Uint64;
#else
//	64 bit integers
typedef signed   long long Int64;
typedef unsigned long long Uint64;
#endif

class Exception;
class File;
class Directory;
class FileStream;
class StreamHandler;
template<typename T>
class ImmutableList;

////////////////////////////////////////////////////////
///	\brief	A base exception class for FDL
///
////////////////////////////////////////////////////////
class FDLAPI Exception : public std::exception
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
	FDL_EXCEPTION_CREATE_EXTEND(FileSizeFailureException, FileFailException);

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
	///	\throws	File::FileSizeFailException	If size retevial can't be done
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
	///	\throws File::FileMissingException	If recursiveCreate is false and newFile's
	///		root path does not exist
	///
	///	\return	Whether the file move succeeded
	////////////////////////////////////////////////////////
	bool move(File newFile, bool recursiveCreate=true);
	
	////////////////////////////////////////////////////////
	///	\brief	Opens the File in a stream, auto-determines binary treatment
	///
	///	\see FDL::File::open(bool binaryOpen)	Manually determines binary treatment
	///
	///	\throws	File::FileFailException	If FileStream can't be opened
	///	\throws File::FileMissingException	If File does not exist
	///
	///	\return	A FileStream pointing to the File's stream
	////////////////////////////////////////////////////////
	FileStream open();
	
	////////////////////////////////////////////////////////
	///	\brief	Opens the File in a stream
	///
	///	\param	binaryOpen	Determines whether the file is treated as a binary
	///
	///	\throws	File::FileFailException	If FileStream can't be opened
	///	\throws File::FileMissingException	If File does not exist
	///
	///	\return	A FileStream pointing to the File's stream
	////////////////////////////////////////////////////////
	FileStream open(bool binaryOpen);
	
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

	////////////////////////////////////////////////////////
	///	\brief	Constructor for a Directory
	///
	///	\param	path	The path File points to, relative or absoulte, empty
	///		points to working directory
	///
	////////////////////////////////////////////////////////
	Directory(CStr path="");
	
	////////////////////////////////////////////////////////
	///	\brief	Constructor for a File
	///
	///	\param	root	The root to start in
	///	\param	path	The path File points to
	///
	////////////////////////////////////////////////////////
	Directory(CStr root, CStr path);
	
	////////////////////////////////////////////////////////
	///	\brief	Constructor for a File
	///
	///	\param	root	The root to start in
	///	\param	path	The path File points to
	///
	////////////////////////////////////////////////////////
	Directory(File root, CStr path="");
	
	////////////////////////////////////////////////////////
	///	\brief	Opens a File in the Directory
	///
	///	\param	path	A path to search for File
	///
	////////////////////////////////////////////////////////
	File open(CStr path);
	
	////////////////////////////////////////////////////////
	///	\brief	Retrieves an ImmutableList of the contained files
	///
	////////////////////////////////////////////////////////
	ImmutableList<File>	getContainedFiles();
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
	
	////////////////////////////////////////////////////////
	///	\brief	Default destructor, closes mp_fileStream
	///
	////////////////////////////////////////////////////////
	~FileStream();
	
	////////////////////////////////////////////////////////
	///	\brief	Opens the FileStream
	///
	///	\return	Whether FileStream opened correctly
	////////////////////////////////////////////////////////
	bool open();
	
	////////////////////////////////////////////////////////
	///	\brief	Whether the FileStream is open
	///
	////////////////////////////////////////////////////////
	bool isOpen();
	
	////////////////////////////////////////////////////////
	///	\brief	Closes the FileStream
	///
	///	\return	Whether FileStream closed properly
	////////////////////////////////////////////////////////
	void close();
	
	////////////////////////////////////////////////////////
	///	\brief	Writes data to the FileStream
	///
	///	\param	data	The data to write to the stream
	///	\param	size	The acceptable size of the data, -1 auto-assigns the size
	///
	////////////////////////////////////////////////////////
	void write(Bytes data, Int64 size=-1);
	
	////////////////////////////////////////////////////////
	///	\brief	Reads the data from the FileStream
	///
	///	\param	data	The data to read from the stream
	///
	////////////////////////////////////////////////////////
	Int64 read(Bytes data);
	
	////////////////////////////////////////////////////////
	///	\brief	Sets the position of the writer
	///
	///	\param	position	The position to set the writer to
	///
	///	\throws	FileStream::EOSException	If position is beyond end of stream
	///
	////////////////////////////////////////////////////////
	void seekWrite(Int64 position);
	
	////////////////////////////////////////////////////////
	///	\brief	Reports the position of the writer
	///
	///	\throws	File::FileSizeFailException	If writer position can't be retrieved
	///
	////////////////////////////////////////////////////////
	UInt64 tellWrite();
	
	////////////////////////////////////////////////////////
	///	\brief	Sets the position of the reader
	///
	///	\param	position	The position to set the reader to
	///
	///	\throws	FileStream::EOSException	If position is beyond end of stream
	///
	////////////////////////////////////////////////////////
	void seekRead(Int64 position);
	
	////////////////////////////////////////////////////////
	///	\brief	Reports the position of the reader
	///
	///	\throws	File::FileSizeFailException	If writer position can't be retrieved
	///
	////////////////////////////////////////////////////////
	Int64 tellRead();
	
	////////////////////////////////////////////////////////
	///	\brief	Flushes the FileStream
	///
	///	\return	Whether flush succeeded
	////////////////////////////////////////////////////////
	bool flush();
	
	////////////////////////////////////////////////////////
	///	\brief	Retrieves the std::fstream currently active, returns NULL
	///		if isOpen is false
	///
	////////////////////////////////////////////////////////
	std::fstream* getStream();
};

template<typename T>
class FDLAPI ImmutableList
{
private:
	
	const T* mp_valuesList;
	const std::size_t m_size;
public:
	
	typedef std::iterator<std::random_access_iterator_tag, T> Iterator;

	////////////////////////////////////////////////////////
	///	\brief	Default Constructor for ImmutableList	
	///
	////////////////////////////////////////////////////////
	ImmutableList();
	
	////////////////////////////////////////////////////////
	///	\brief	Constructor for ImmutableList
	///
	///	\param	p_values	An array of values of type T
	///	\param	size	The size of the stored array
	///
	////////////////////////////////////////////////////////
	ImmutableList(const T* p_values, const size_t size);
	
	////////////////////////////////////////////////////////
	///	\brief	Constructor for ImmutableList
	///
	///	\param	values	An object of HolderT which holds type T
	///
	////////////////////////////////////////////////////////
	template<typename HolderT>
	ImmutableList(const HolderT<T> values);
	
	////////////////////////////////////////////////////////
	///	\brief	Assignment operator for ImmutableList to ImmutableList
	///
	///	\param	rhs	A reference to a constant ImmutableList to assign
	///
	////////////////////////////////////////////////////////
	ImmutableList& operator=(const ImmutableList &rhs) const;
	
	////////////////////////////////////////////////////////
	///	\brief	Retrieves an immutable group of values for the list
	///
	////////////////////////////////////////////////////////
	const T* getValues() const;
	
	////////////////////////////////////////////////////////
	///	\brief	Retrieves  constant size value of the list
	///
	////////////////////////////////////////////////////////
	const size_t getSize() const;
	
	////////////////////////////////////////////////////////
	///	\brief	Retrieves the beginning iterator
	///
	////////////////////////////////////////////////////////
	const Iterator getBegin() const;
	
	////////////////////////////////////////////////////////
	///	\brief	Retrieves the ending iterator
	///
	////////////////////////////////////////////////////////
	const Iterator getEnd() const;
};

} /* namespace FDL */

#endif /* _FDL_INCLUDE_ */
