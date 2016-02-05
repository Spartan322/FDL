# FDL
File Directory Library is a cross-platform file and directory management library which makes dealing with the filesystem simple, easy, and efficient.

## Building
When CMake is implemented.

To build with CMake could be done like:
```
mkdir build	# Create an output directory
cd build
cmake <path/of/FDL>	# Generate native builds
```

## Usage
To access a File:
```cpp
FDL::File file("file/thing.txt");
```
This will retrieve basic file information on "file\thing.txt" relative to the root directory if the drive is omitted in the path

To read a file:
```cpp
FDL::File file("example.txt");
FDL::FileStream stream = file.open();
```
This will create a file stream object with a C++ standard fstream object which allows direct manipulation, or controlled manipulation. This can fail with a FileFailException or FileMissingException. The fstream is not created if the stream fails

To access a Directory:
```cpp
FDL::Directory dir("directory");
```
This will retrieve a directory which can contain directory information, can be iterator'ed through, and can be used to open File objects instead of using File. The root directory can be retrieved if the drive is omitted in the path

Check out the examples and documentation for more explanations

### Path Syntax
It is recommended to use the forward slash as backslashes require escaping, but either is allowed

Since Windows and POSIX absolute paths are drastically different (Windows often being more consistent, and POSIX isn't), you can use either OS form call, but they may throw FileFailException if the converted path does not work.

#### Converted Paths
Converted paths are processed as such:
- C: is the standard root filesystem for Windows
- /dev/sda is the standard root filesystem for Unix

Either one of them will be converted to the other but this is not a guaranteed conversion and could throw a FFE.

## License
FDL is distrbuted under the  [Apache License](https://github.com/Spartan322/FDL/blob/master/LICENSE)
