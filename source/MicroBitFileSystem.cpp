/*
The MIT License (MIT)
Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.
Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "MicroBitFileSystem.h"
#include "MicroBitFlash.h"
#include "MicroBitStorage.h"
#include "CodalFS.h"

MicroBitFileSystem* MicroBitFileSystem::defaultFileSystem = new MicroBitFileSystem(0, 0);

/**
  * Constructor. Creates an instance of a MicroBitFileSystem.
  */
MicroBitFileSystem::MicroBitFileSystem(uint32_t flashStart, int flashPages)
{
    //TODO temporary compat for MicroBitFileSystem -> CodalFS
    CodalFS::defaultFileSystem->init();
}

/**
  * Creates a new directory with the given name and location
  *
  * @param name The fully qualified name of the new directory.
  * @return MICROBIT_OK on success, MICROBIT_INVALID_PARAMETER if the path is invalid, or MICROBT_NO_RESOURCES if the FileSystem is full.
  */
int MicroBitFileSystem::createDirectory(char const *name)
{
    //TODO temporary compat for MicroBitFileSystem -> CodalFS
    return CodalFS::defaultFileSystem->createDirectory(name);
}


/**
  * Open a new file, and obtain a new file handle (int) to
  * read/write/seek the file. The flags are:
  *  - MB_READ : read from the file.
  *  - MB_WRITE : write to the file.
  *  - MB_CREAT : create a new file, if it doesn't already exist.
  *
  * If a file is opened that doesn't exist, and MB_CREAT isn't passed,
  * an error is returned, otherwise the file is created.
  *
  * @param filename name of the file to open, must contain only printable characters.
  * @param flags One or more of MB_READ, MB_WRITE or MB_CREAT. 
  * @return return the file handle,MICROBIT_NOT_SUPPORTED if the file system has
  *         not been initialised MICROBIT_INVALID_PARAMETER if the filename is
  *         too large, MICROBIT_NO_RESOURCES if the file system is full.
  *
  * @code
  * MicroBitFileSystem f();
  * int fd = f.open("test.txt", MB_WRITE|MB_CREAT);
  * if(fd<0)
  *    print("file open error");
  * @endcode
  */
int MicroBitFileSystem::open(char const * filename, uint32_t flags)
{
    //TODO temporary compat for MicroBitFileSystem -> CodalFS
    return CodalFS::defaultFileSystem->open(filename, flags);
}


/**
  * Writes back all state associated with the given file to FLASH memory, 
  * leaving the file open.
  *
  * @param fd file descriptor - obtained with open().
  * @return MICROBIT_OK on success, MICROBIT_NOT_SUPPORTED if the file system has not
  *         been initialised, MICROBIT_INVALID_PARAMETER if the given file handle
  *         is invalid.
  *
  * @code
  * MicroBitFileSystem f();
  * int fd = f.open("test.txt", MB_READ);
  *
  * ...
  *
  * f.flush(fd);
  * @endcode
  */
int MicroBitFileSystem::flush(int fd)
{
    //TODO temporary compat for MicroBitFileSystem -> CodalFS
    return CodalFS::defaultFileSystem->flush(fd);
}

/**
  * Close the specified file handle.
  * File handle resources are then made available for future open() calls.
  *
  * close() must be called at some point to ensure the filesize in the
  * FT is synced with the cached value in the FD.
  *
  * @warning if close() is not called, the FT may not be correct,
  * leading to data loss.
  *
  * @param fd file descriptor - obtained with open().
  * @return MICROBIT_OK on success, MICROBIT_NOT_SUPPORTED if the file system has not
  *         been initialised, MICROBIT_INVALID_PARAMETER if the given file handle
  *         is invalid.
  *
  * @code
  * MicroBitFileSystem f();
  * int fd = f.open("test.txt", MB_READ);
  * if(!f.close(fd))
  *    print("error closing file.");
  * @endcode
  */
int MicroBitFileSystem::close(int fd)
{
    //TODO temporary compat for MicroBitFileSystem -> CodalFS
    return CodalFS::defaultFileSystem->close(fd);
}

/**
  * Move the current position of a file handle, to be used for
  * subsequent read/write calls.
  *
  * The offset modifier can be:
  *  - MB_SEEK_SET set the absolute seek position.
  *  - MB_SEEK_CUR set the seek position based on the current offset.
  *  - MB_SEEK_END set the seek position from the end of the file.
  * E.g. to seek to 2nd-to-last byte, use offset=-1.
  *
  * @param fd file handle, obtained with open()
  * @param offset new offset, can be positive/negative.
  * @param flags
  * @return new offset position on success, MICROBIT_NOT_SUPPORTED if the file system
  *         is not intiialised, MICROBIT_INVALID_PARAMETER if the flag given is invalid
  *         or the file handle is invalid.
  *
  * @code
  * MicroBitFileSystem f;
  * int fd = f.open("test.txt", MB_READ);
  * f.seek(fd, -100, MB_SEEK_END); //100 bytes before end of file.
  * @endcode
  */
int MicroBitFileSystem::seek(int fd, int offset, uint8_t flags)
{
    //TODO temporary compat for MicroBitFileSystem -> CodalFS
    return CodalFS::defaultFileSystem->seek(fd, offset, flags);
}

/**
  * Read data from the file.
  *
  * Read len bytes from the current seek position in the file, into the
  * buffer. On each invocation to read, the seek position of the file
  * handle is incremented atomically, by the number of bytes returned.
  *
  * @param fd File handle, obtained with open()
  * @param buffer to store data
  * @param size number of bytes to read
  * @return number of bytes read on success, MICROBIT_NOT_SUPPORTED if the file
  *         system is not initialised, or this file was not opened with the
  *         MB_READ flag set, MICROBIT_INVALID_PARAMETER if the given file handle
  *         is invalid.
  *
  * @code
  * MicroBitFileSystem f;
  * int fd = f.open("read.txt", MB_READ);
  * if(f.read(fd, buffer, 100) != 100)
  *    print("read error");
  * @endcode
  */
int MicroBitFileSystem::read(int fd, uint8_t* buffer, int size)
{
    //TODO temporary compat for MicroBitFileSystem -> CodalFS
    return CodalFS::defaultFileSystem->read(fd, buffer, size);
}

/**
  * Write data to the file.
  *
  * Write from buffer, len bytes to the current seek position.
  * On each invocation to write, the seek position of the file handle
  * is incremented atomically, by the number of bytes returned.
  *
  * The cached filesize in the FD is updated on this call. Also, the
  * FT file size is updated only if a new page(s) has been written too,
  * to reduce the number of FT writes.
  *
  * @param fd File handle
  * @param buffer the buffer from which to write data
  * @param len number of bytes to write
  * @return number of bytes written on success, MICROBIT_NO_RESOURCES if data did
  *         not get written to flash or the file system has not been initialised,
  *         or this file was not opened with the MB_WRITE flag set, MICROBIT_INVALID_PARAMETER
  *         if the given file handle is invalid.
  *
  * @code
  * MicroBitFileSystem f();
  * int fd = f.open("test.txt", MB_WRITE);
  * if(f.write(fd, "hello!", 7) != 7)
  *    print("error writing");
  * @endcode
  */
int MicroBitFileSystem::write(int fd, uint8_t* buffer, int size)
{
    //TODO temporary compat for MicroBitFileSystem -> CodalFS
    return CodalFS::defaultFileSystem->write(fd, buffer, size);
}

/**
  * Remove a file from the system, and free allocated assets
  * (including assigned blocks which are returned for use by other files).
  *
  * @param filename null-terminated name of the file to remove.
  * @return MICROBIT_OK on success, MICROBIT_INVALID_PARAMETER if the given filename
  *         does not exist, MICROBIT_CANCELLED if something went wrong.
  *
  * @code
  * MicroBitFileSystem f;
  * if(!f.remove("file.txt"))
  *     print("file could not be removed")
  * @endcode
  */
int MicroBitFileSystem::remove(char const * filename)
{
    //TODO temporary compat for MicroBitFileSystem -> CodalFS
    return CodalFS::defaultFileSystem->remove(filename);
}

