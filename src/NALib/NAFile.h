
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_FILE_INCLUDED
#define NA_FILE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This file contains the definitions for very basic file and directory
// handling.
//
// If you are looking for more powerful reading and writing functions, have
// a look at NABuffer.

#include "NAString.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

// File Input and output is handeled slightly differently on different
// operating systems. In the header files of each system, there are different
// definitions of standard C functions and hence, NALib is trying to map
// everything to one definition.
//
// New types and macros introduced:
// NAFileSize:          The file size integer. Guaranteed to be signed. May be
//                      32 or 64 bits depending on system compiled for. This is
//                      different to the NABufInt type which is 64 bits always.
//                      It is (regarding the currently supported systems) the
//                      same as NAInt but we make the distinction anyway, as
//                      some systems may define disk access with a greater
//                      number of bits than addresses in memory.
// NA_FILESIZE_BITS and NA_FILESIZE_MAX     Two additional macros
//                      storing the bits needed for a file offset.
// NAFileMode:          The filesystem permission mode. On Windows corresponds
//                      to the flag combination like _S_IREAD, and on Unix
//                      corresponds to the octal premission flags like 0644.
//
// NA_FILEMODE_DEFAULT: The default permission mode to create a file.
// NA_DIRMODE_DEFAULT:  The default permission mode to create a directory. Not
//                      needed on Windows.
//
// The following macros define the NALib standard file opening flags. They
// denote combinations with flags like O_RDONLY and are needed for the naOpen
// function. You may choose to set the flags manually or use one of these three
// macros, whereas note that all three macros open files in BINARY format:
//
// NA_FILE_OPEN_FLAGS_READ   Read-Only, Cursor placed at byte 0
// NA_FILE_OPEN_FLAGS_WRITE  Write-Only, File truncated. Cursor placed at byte 0
// NA_FILE_OPEN_FLAGS_APPEND Write-Only, Cursor placed at end of file

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <windows.h>
  #include <direct.h>
  #include <io.h>
  #include <share.h>
  #include <sys/stat.h>
  #if NA_SYSTEM_ADDRESS_BITS == 64
    typedef __int64 NAFileSize; // Is signed (Important for negative offsets)
    #define NA_FILESIZE_BITS 64
    #define NA_FILESIZE_MAX NA_INT64_MAX
  #elif NA_SYSTEM_ADDRESS_BITS == 32
    typedef long NAFileSize;    // Is signed (Important for negative offsets)
  #define NA_FILESIZE_BITS 32
  #define NA_FILESIZE_MAX NA_INT32_MAX
#endif
  typedef int NAFileMode;
  #define NA_FILEMODE_DEFAULT (_S_IREAD | _S_IWRITE)
  #define NA_DIRMODE_DEFAULT 0 // No directory modes on windows needed.
  #define NA_FILE_OPEN_FLAGS_READ (O_RDONLY | O_BINARY)
  #define NA_FILE_OPEN_FLAGS_WRITE (O_WRONLY | O_CREAT | O_TRUNC | O_BINARY)
  #define NA_FILE_OPEN_FLAGS_APPEND (O_WRONLY | O_CREAT | O_APPEND | O_BINARY)
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <unistd.h>
  #include <dirent.h>
  #include <copyfile.h>
  typedef off_t NAFileSize;     // Is signed (Important for negative offsets)
  #define NA_FILESIZE_BITS 64
  #define NA_FILESIZE_MAX NA_INT64_MAX
  typedef int NAFileMode;
  #define NA_FILEMODE_DEFAULT 0644
  #define NA_DIRMODE_DEFAULT 0755
  #define NA_FILE_OPEN_FLAGS_READ (O_RDONLY) // There is no binary flag in Unix.
  #define NA_FILE_OPEN_FLAGS_WRITE (O_WRONLY | O_CREAT | O_TRUNC)
  #define NA_FILE_OPEN_FLAGS_APPEND (O_WRONLY | O_CREAT | O_APPEND)
#endif


// Mapping of standard library functions. They can be different depending on
// the system compiled. Note that only the types which are different depending
// on the system are mapped to NALib types. All other types are corresponding
// to the standards.

NA_IAPI NAFileSize  naLseek   (int fd, NAFileSize offset, int origin);
NA_IAPI int         naOpen    (const char* path, int flags, int mode);
NA_IAPI int         naClose   (int fd);
NA_IAPI NAFileSize  naRead    (int fd, void* buf, NAFileSize count);
NA_IAPI NAFileSize  naWrite   (int fd, const void* buf, NAFileSize count);
NA_IAPI int         naMkDir   (const char* path, int mode);
NA_IAPI int         naChDir   (const char* path);
NA_IAPI NABool      naExists  (const char* path);
NA_IAPI NABool      naIsDir   (const char* path);
NA_IAPI NABool      naIsHidden(const char* path);
NA_IAPI int         naRemove  (const char* path);
NA_IAPI NABool      naCopyFile(const char* dstpath, const char* srcpath);
NA_IAPI NABool      naAccess  (const char* path, NABool exists, NABool read,
                               NABool write, NABool execute);
NA_IAPI NAUTF8Char* naGetCwd  (NAUTF8Char* buf, NAInt bufsize);




// The full type definition is in the file "NAFileII.h"
//
// This is the definition of NAFile. It is designed to be a POD (plain old data)
// struct and in fact currently is nothing else but a struct container for a
// unix file descriptor. Again, if you look for more elaborate functions, see
// NABuffer.
//
// An NAFile can either be a Read-File or a Write-File. Never both.
typedef struct NAFile NAFile;

// Opens the file.
// Reading:   Opens an existing file and places the read pointer to the first
//            byte of the stream.
// Writing:   Creates a new file or overwrites and truncates an existing file
//            and places the write pointer to the first byte of the stream.
// Appending: Creates a new file or opens an existing file for appending. The
//            write pointer is set to the end of the stream.
//
// If you don't know, what the mode argument means, use NA_FILEMODE_DEFAULT.

NA_IAPI NAFile  naMakeFileReadingFilename   (const char* filename);
NA_IAPI NAFile  naMakeFileWritingFilename   (const char* filename,
                                              NAFileMode mode);
NA_IAPI NAFile  naMakeFileAppendingFilename (const char* filename,
                                              NAFileMode mode);

// Makes an NAFile struct wrapped around the standard channels.
// Stdin:     Standard input (file descriptor 0) as a reading file.
// Stdout:    Standard output (file descriptor 1) as an appending file.
// Stderr:    Standard error (file descriptor 2) as an appending file.
NA_IAPI NAFile naMakeFileReadingStdin();
NA_IAPI NAFile naMakeFileWritingStdout();
NA_IAPI NAFile naMakeFileWritingStderr();

// Note that NAFile is designed to be a POD (plain old data) struct and
// therefore, creating an NAFile is done using the naMake... functions.
// But for any other function call below, the NAFile struct must be provided
// as a pointer.

// Closes the file. Note: Can close stdin, stdout and stderr!
NA_IAPI void naCloseFile(NAFile* file);

// Computes the filesize (from first to last byte).
NA_IAPI NAFileSize naComputeFileSize(const NAFile* file);

// Tests if a file has been opened.
NA_IAPI NABool naIsFileOpen(const NAFile* file);

// Re-adjusts the internal file pointer to the given offset.
NA_IAPI void naSeekFileAbsolute(NAFile* file, NAFileSize offset);
NA_IAPI void naSeekFileRelative(NAFile* file, NAFileSize offset);

// Reads the given number of bytes and stores it without further manipulation
// in buf. The buffer must be big enough, no overflow check is made. This is
// basically just an encapsulating method for naRead(). Have a look at the
// NABuffer structure which gives you way more power for reading and writing
// files or with any kind of buffers.
//
// Returns the number of bytes read.
NA_IAPI NAFileSize naReadFileBytes(NAFile* file, void* buf, NAFileSize count);

// Writes the given number of bytes from ptr to the file without further
// manipulation. The buffer must be big enough, no overflow check is made.
// This is basically just an encapsulating method for naWrite(). Have a look
// at the NABuffer structure which gives you way more power for reading and
// writing files with any kind of buffers.
//
// Returns the number of bytes written.
NA_IAPI NAFileSize naWriteFileBytes(NAFile* file, const void* ptr, NAInt count);


// Creates or fills an NAByteArray or an NAString by reading the given number
// of bytes from the file directly into a newly created array or string.
//
// Note: If you want the whole file as an NAByteArray or NAString, use the
// naInitByteArrayWithFileContents or naNewStringWithFileContents function
// instead.
NA_IAPI NAByteArray* naInitByteArrayFromFile(NAByteArray* array,
                                                   NAFile* file,
                                                NAFileSize count);
NA_IAPI NAString*    naNewStringFromFile(           NAFile* file,
                                                NAFileSize bytecount);


// //////////////////////////
// General input and output methods
//

// Reads a full file and returns it as a new String.
// Note that this is the preferred way to handle text files in NALib: Just
// read the whole file into a string and then use the parsing functions of
// the NAString type.
NA_IAPI NAString* naNewStringWithFileContents(const char* filename);

// Reads a full file and returns it as a new NAByteArray.
// Warning: This function is only useful if you read or store raw data! The 
// NAByteArray struct is not endianness-aware. Use the reading and writing
// functions of NABuffer when handling multi-byte values instead.
NA_IAPI NAByteArray* naInitByteArrayWithFileContents( NAByteArray* array,
                                                       const char* filename);

// Scans a Decimal of the type NAInt from the console.
NA_IAPI NAInt naScanDecimal();

// Returns a string containing the current working directory
NA_IAPI NAString* naNewStringWithCurrentWorkingDirectory();




// Inline implementations are in a separate file:
#include "NACore/NAFileII.h"



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_FILE_INCLUDED

// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
