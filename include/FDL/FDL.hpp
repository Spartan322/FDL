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
EXCEPT_NAME(FDL::String message="") : EXTEND_NAME(message) {} \
~EXCEPT_NAME() { ~EXTEND_NAME(); } \
} \

#define FDL_EXCEPTION_CREATE(EXCEPT_NAME) FDL_EXCEPTION_CREATE_EXTEND(EXCEPT_NAME, FDL::Exception)

namespace FDL {

class Exception;
class String;
class File;
class Directory;
class FileStream;
class StreamHandler;
template<typename T>
class ImmutableList;

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

////////////////////////////////////////////////////////
///	\brief	Determines whether this function is being run on Windows
///
////////////////////////////////////////////////////////
bool FDLAPI isWindows();
////////////////////////////////////////////////////////
///	\brief	Determines whether this function is being run on a POSIX system
///
////////////////////////////////////////////////////////
bool FDLAPI isPosix();

////////////////////////////////////////////////////////
///	\brief	Creates a file according to the path
///
///	\note	Does not throw
///
///	\warn	Unsafe, does not check path, avoid use
///
///	\param	path	The path of the file
///	\param	recursive	Whether recursive creation is done
///
///	\return	Whether file creation succeeded
////////////////////////////////////////////////////////
bool FDLAPI createFileNS(String path, bool recursive=true);

////////////////////////////////////////////////////////
///	\brief	Deletes a file according to the path
///
///	\note	Does not throw
///
///	\warn	Unsafe, does not check path, avoid use
///
///	\param	path	The path to of the file to delete
///
///	\return	Whether file deletion succeeded
////////////////////////////////////////////////////////
bool FDLAPI deleteFileNS(String path);

////////////////////////////////////////////////////////
///	\brief	Converts a string to an appropriate string
///
///	Shall replace all instances of escaped '\' with '/'
///
///	\note	Returns NULL if path can not be converted
///
///	\param	originalStr	The original string to convert
///
////////////////////////////////////////////////////////
String FDLAPI convertString(String originalStr);

////////////////////////////////////////////////////////
///	\brief	A base exception class for FDL
///
////////////////////////////////////////////////////////
class FDLAPI Exception : public std::exception
{
protected:

	String m_message;
public:

	////////////////////////////////////////////////////////
	///	\brief	Constructor for an Exception
	///
	///	\param	message	The message to assign
	///
	////////////////////////////////////////////////////////
	virtual Exception(String message="");

	////////////////////////////////////////////////////////
	///	\brief	The Default Destructor
	///
	////////////////////////////////////////////////////////
	virtual ~Exception();

	////////////////////////////////////////////////////////
	///	\brief	return the message
	///
	////////////////////////////////////////////////////////
	String what();

	////////////////////////////////////////////////////////
	///	\brief	Casts the exception to return the message into a new string
	///
	////////////////////////////////////////////////////////
	String operator String();

	////////////////////////////////////////////////////////
	///	\brief	Assigns a message
	///
	///	\param	message	The message to assign
	///
	////////////////////////////////////////////////////////
	Exception& operator=(String message);
};

FDL_EXCEPTION_CREATE(UnsupportedException);
FDL_EXCEPTION_CREATE(BadPathException);

////////////////////////////////////////////////////////
///	\brief	A small and simple class for handling character strings
///
////////////////////////////////////////////////////////
class FDLAPI String
{
private:

	std::size_t m_size;
	const char* m_str;
public:

	///	\brief	A consistent null string
	static const String null_str = String(NULL);

	////////////////////////////////////////////////////////
	///	\brief	Constructor for a String
	///
	///	\param	strings	The string to set
	///
	////////////////////////////////////////////////////////
	String(char* string = "");

	////////////////////////////////////////////////////////
	///	\brief	Constructor for a String
	///
	///	\param	strings	The string to set
	///	\param	size	The max characters to hold in the string
	///
	////////////////////////////////////////////////////////
	String(char* string, std::size_t size);

	////////////////////////////////////////////////////////
	///	\brief	Constructor for a String
	///
	///	\param	strings	The string to set
	///
	////////////////////////////////////////////////////////
	String(const char* string);

	////////////////////////////////////////////////////////
	///	\brief	Constructor for a String
	///
	///	\param	strings	The string to set
	///	\param	size	The max characters to hold in the string
	///
	////////////////////////////////////////////////////////
	String(const char* string, std::size_t size);

	////////////////////////////////////////////////////////
	///	\brief	Copy Constructor for a String
	///
	///	\param	strings	The string to copy
	///
	////////////////////////////////////////////////////////
	String(const String& string);

	////////////////////////////////////////////////////////
	///	\brief	Destructor for a String
	///
	////////////////////////////////////////////////////////
	~String();

	////////////////////////////////////////////////////////
	///	\brief	An equalivent operator for character strings
	///
	///	\param	string	The string to assign to this String
	///
	////////////////////////////////////////////////////////
	String& operator=(char* string);

	////////////////////////////////////////////////////////
	///	\brief	An equalivent operator for constant character strings
	///
	///	\param	string	The string to assign to this String
	///
	////////////////////////////////////////////////////////
	String& operator=(const char* string);

	////////////////////////////////////////////////////////
	///	\brief	A cast operator for character strings
	///
	////////////////////////////////////////////////////////
	operator char*() const;

	////////////////////////////////////////////////////////
	///	\brief	A cast operator for constant character strings
	///
	////////////////////////////////////////////////////////
	operator const char*() const;

	////////////////////////////////////////////////////////
	///	\brief	An bracket index operator for treatment like array
	///
	///	\param	position	The position being searched for
	///
	///	\throws	std::out_of_range	If position is beyond size
	///
	////////////////////////////////////////////////////////
	char* operator[](int position);

	////////////////////////////////////////////////////////
	///	\brief	Retrieves the constant character string held within
	///
	///	\note	For compatibility with std classes
	///
	////////////////////////////////////////////////////////
	const char* c_str() const;

	////////////////////////////////////////////////////////
	///	\brief	Retrieves the size of the character string within
	///
	///	\note	For compatibility with std classes
	///
	////////////////////////////////////////////////////////
	std::size_t size() const;

	////////////////////////////////////////////////////////
	///	\brief	Determines if the string is a null string
	///
	///	\return	true if is not equal to NULL, otherwise false
	///
	////////////////////////////////////////////////////////
	bool isNullStr() const;
};

////////////////////////////////////////////////////////
///	\brief	The simpliest file managing object
///
////////////////////////////////////////////////////////
class FDLAPI File
{
protected:

	Str m_fullPath;
public:

	FDL_EXCEPTION_CREATE(FileFailException);
	FDL_EXCEPTION_CREATE_EXTEND(FileMissingException, FileFailException);
	FDL_EXCEPTION_CREATE_EXTEND(FileSizeFailureException, FileFailException);

	////////////////////////////////////////////////////////
	///	\brief	Constructor for a File
	///
	///	\param	path	The path File points to, relative or absoulte
	///
	///	\throws	BadPathException	If path is invalid
	///
	////////////////////////////////////////////////////////
	File(String path);

	////////////////////////////////////////////////////////
	///	\brief	Constructor for a File
	///
	///	\param	root	The root to start in
	///	\param	path	The path File points to according to root
	///
	///	\throws	BadPathException	If path is invalid or could not be formatted
	///
	////////////////////////////////////////////////////////
	File(String root, String path);

	////////////////////////////////////////////////////////
	///	\brief	Constructor for a File
	///
	///	\param	root	The root to start in
	///	\param	path	The path File points to according to root
	///
	///	\throws	BadPathException	If path is invalid or could not be formatted
	///
	////////////////////////////////////////////////////////
	File(File root, String path="");

	////////////////////////////////////////////////////////
	///	\brief	Default destructor
	///
	////////////////////////////////////////////////////////
	~File();

	////////////////////////////////////////////////////////
	///	\brief	Retrieves the full path of the file
	///
	////////////////////////////////////////////////////////
	String getFullPath() const;

	////////////////////////////////////////////////////////
	///	\brief	Retrieves the extension of the file
	///
	///	\note	return NULL if isDirectory is true
	///
	////////////////////////////////////////////////////////
	String getExtension() const;

	////////////////////////////////////////////////////////
	///	\brief Retrieves the root path of the file
	///
	////////////////////////////////////////////////////////
	String getRootPath() const;

	////////////////////////////////////////////////////////
	///	\brief	Retrieves the file name
	///
	////////////////////////////////////////////////////////
	String getFileName() const;

	////////////////////////////////////////////////////////
	///	\brief	Retrieves the file name and extension
	///
	////////////////////////////////////////////////////////
	String getFullName() const;

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
	///	\throws	File::FileMissingException	If file couldn't be found
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
	String toNativePath() const;
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
	Directory(String path="");

	////////////////////////////////////////////////////////
	///	\brief	Constructor for a File
	///
	///	\param	root	The root to start in
	///	\param	path	The path File points to
	///
	////////////////////////////////////////////////////////
	Directory(String root, String path);

	////////////////////////////////////////////////////////
	///	\brief	Constructor for a File
	///
	///	\param	root	The root to start in
	///	\param	path	The path File points to
	///
	////////////////////////////////////////////////////////
	Directory(File root, String path="");

	////////////////////////////////////////////////////////
	///	\brief	Opens a File in the Directory
	///
	///	\param	path	A path to search for File
	///
	////////////////////////////////////////////////////////
	File open(String path);

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
