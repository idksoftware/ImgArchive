/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#ifdef XXXXXXXXXXXXXXXXXXX
#ifndef __IDKFILE_H__
#define __IDKFILE_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif

#include "CIDKDate.h"
//#include "CIDKException.h"

/* ==================================================

  UNIX 

  The Unix function stat(char *path, struct *buf) is used to get the
  file status information.

  This structure is used to return the file status
	
	The contents of the structure pointed to by buf include  the
     following members:

          mode_t   st_mode;     * File mode
          ino_t    st_ino;      * Inode number
          dev_t    st_dev;      * ID of device containing
                                  a directory entry for this file
          dev_t    st_rdev;     * ID of device 
                                  This entry is defined only for
                                  char special or block special files
          nlink_t  st_nlink;    * Number of links
          uid_t    st_uid;      * User ID of the file's owner
          gid_t    st_gid;      * Group ID of the file's group
          off_t    st_size;     * File size in bytes
          time_t   st_atime;    * Time of last access
          time_t   st_mtime;    * Time of last data modification
          time_t   st_ctime;    * Time of last file status change
                                  Times measured in seconds since
								   00:00:00 UTC, Jan. 1, 1970

 Descriptions of structure members are as follows:

     st_mode   The mode of the file as described in mknod(2).  In
               addition  to  the  modes described in mknod(), the
               mode of a file may also be S_IFLNK if the file  is
               a  symbolic link.  S_IFLNK may only be returned by
               lstat().

     st_ino    This field uniquely identifies the file in a given
               file  system.  The pair st_ino and st_dev uniquely
               identifies regular files.

     st_dev    This field uniquely  identifies  the  file  system
               that  contains the file.  Its value may be used as
               input to the ustat() function  to  determine  more
               information  about  this  file  system.   No other
               meaning is associated with this value.

     st_rdev   This field should be used only  by  administrative
               commands.   It  is valid only for block special or
               character special files and only  has  meaning  on
               the system where the file was configured.

     st_nlink  This field should be used only  by  administrative
               commands.

     st_uid    The user ID of the file's owner.

     st_gid    The group ID of the file's group.

     st_size   For regular files, this is the address of the  end
               of  the file.  For block special or character spe-
               cial, this is not defined.  See also pipe(2).

     st_atime  Time when file data was last accessed.  Changed by
               the   following   functions:    creat(),  mknod(),
               pipe(), utime(2), and read(2).

     st_mtime  Time when data was last modified.  Changed by  the
               following  functions:   creat(),  mknod(), pipe(),
               utime(), and write(2).

     st_ctime  Time when file status was last  changed.   Changed
               by  the  following  functions:   chmod(), chown(),
               creat(),  link(2),  mknod(),  pipe(),   unlink(2),
               utime(), and write().


     The file type is specified in mode by the S_IFMT bits, which
     must be set to one of the following values:

        S_IFIFO   fifo special
        S_IFCHR   character special
        S_IFDIR   directory
        S_IFBLK   block special
        S_IFREG   ordinary file

	The file access permissions Not used in this release but are left
	here in case?

     The file access permissions are specified  in  mode  by  the
     0007777 bits, and may be constructed by an OR of the follow-
     ing values:

        S_ISUID   04000   Set user ID on execution.
        S_ISGID   020#0   Set group ID on execution if # is 7, 5, 3, or 1.
                          Enable mandatory file/record locking if # is
                          6, 4, 2, or 0.
        S_ISVTX   01000   Save text image  after execution.
        S_IRWXU   00700   Read, write, execute by owner.
        S_IRUSR   00400   Read by owner.
        S_IWUSR   00200   Write by owner.
        S_IXUSR   00100   Execute (search if a directory) by owner.
        S_IRWXG   00070   Read, write, execute by group.
        S_IRGRP   00040   Read by group.
        S_IWGRP   00020   Write by group.
        S_IXGRP   00010   Execute by group.
        S_IRWXO   00007   Read, write, execute  (search) by others.
        S_IROTH   00004   Read by others.
        S_IWOTH   00002   Write by others
        S_IXOTH   00001   Execute by others.
        S_ISVTX           On directories, restricted deletion flag.

 NT

	st_gid

	Numeric identifier of group that owns file (UNIX-specific) This field
	will always be zero on NT systems. A redirected file is classified as
	an NT file.

	st_atime

	Time of last access of file.

	st_ctime

	Time of creation of file.

	st_dev

	Drive number of the disk containing the file (same as st_rdev).

	st_ino

	Number of the information node (the inode) for the file (UNIX-specific)
	On UNIX file systems, the inode describes the file date and time stamps,
	permissions, and content. When files are hard-linked to one another,
	they share the same inode. The inode, and therefore st_ino, has no
	meaning in the FAT, HPFS, or NTFS file systems.

	st_mode

	Bit mask for file-mode information. The _S_IFDIR bit is set if path
	specifies a directory; the _S_IFREG bit is set if path specifies an
	ordinary file or a device. User read/write bits are set according to the
	file�s permission mode; user execute bits are set according
	to the filename extension.

	st_mtime

	Time of last modification of file.

	st_nlink

	Always 1 on non-NTFS file systems.

	st_rdev

	Drive number of the disk containing the file (same as st_dev).

	st_size

	Size of the file in bytes; a 64-bit integer for _stati64 and _wstati64

	st_uid

	Numeric identifier of user who owns file (UNIX-specific). This field will always be zero on NT systems. A redirected file is classified as an NT file.



================================================================== */
/*
class CFileAttrib
{
#ifndef _WIN32
	typedef enum {
		normal,
		readOnly,
		hidden,
		system,
		volume,
		directory,
		archive
	} EAttribute;
	//ERROR - This needs sorting
#endif

public:

	
private:
	unsigned int m_Attr;
public:
	CFileAttrib()
	{
		m_Attr = 0;
	}

	CFileAttrib(unsigned int attr)
	{
		m_Attr = attr;
	}
	CFileAttrib(const char*szFullName)
	{
		m_Attr = ::GetFileAttributes(szFullName);
	}

	void Set(const char*szFullName)
	{
		m_Attr = ::GetFileAttributes(szFullName);
	}

	CFileAttrib(CFileAttrib &attr)
	{
		m_Attr = attr.m_Attr;
	}

	void operator=(CFileAttrib &attr)
	{
		m_Attr = attr.m_Attr;
	}

	void operator=(long attr)
	{
		m_Attr = attr;
	}

	bool IsArchive()
	{
		return (m_Attr & FILE_ATTRIBUTE_ARCHIVE);
	}
	bool IsCompressed()
	{
		return (m_Attr & FILE_ATTRIBUTE_COMPRESSED);
	}

	bool IsDirectory()
	{
		return (m_Attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool IsEncrypted()
	{
		return (m_Attr & FILE_ATTRIBUTE_ENCRYPTED);
	}

	bool IsNormal()
	{
		return (m_Attr & FILE_ATTRIBUTE_NORMAL);
	}

	bool IsNotContentIndexed()
	{
		return (m_Attr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED);
	}

	bool IsOffLine()
	{
		return (m_Attr & FILE_ATTRIBUTE_OFFLINE);
	}

	bool IsReadOnly()
	{
		return (m_Attr & FILE_ATTRIBUTE_READONLY);
	}

	std::string Attr()
	{
		std::string l_buffer;

		if (IsArchive())
		{
			l_buffer += "A";
		}

		if (IsCompressed())
		{
			l_buffer += "C";
		}

		if (IsDirectory())
		{
			l_buffer += "D";
		}
	
		if (IsEncrypted())
		{
			l_buffer += "E";
		}

		if (IsNormal())
		{
			l_buffer += "N";
		}

		if (IsNotContentIndexed())
		{
			l_buffer += "I";
		}

		if (IsOffLine())
		{
			l_buffer += "O";
		}

		if (IsReadOnly())
		{
			l_buffer += "R";
		}
		return l_buffer;
	}

	void Print()
	{
		Print(stdout);
	}
	void Print(FILE *fp)
	{

		fprintf(fp, "Attr %s\n",Attr().c_str());
	}
	long Get()
	{
		return (long)m_Attr;
	}

	bool operator!=(CFileAttrib &attr)
	{
		return (m_Attr != attr.m_Attr);
	}
};
*/

class CIDKFileStatus
{
public:

private:
	CIDKDate m_ctime;
	CIDKDate m_mtime;
	CIDKDate m_atime;
	long m_size;
	CFileAttrib m_attr;
	std::string m_szFullName;
	std::string m_szName;
	std::string m_szExt;
	int m_iError;
	

	bool IsDirectory()
	{
		return m_attr.IsDirectory();
	}
public:

	//	Members

	// CALL THIS BEFORE ANY OF THE OTHERS!

	bool IsValid()
	{
		return (m_iError == 0);
	}

	//
	// Specifies the date and time the file was created. 
	//
	CIDKDate GetCreatedTime()
	{
		return m_ctime;
	}

	//
	// Specifies the date and time the file was last modified. 
	//
	CIDKDate GetModifiedTime()
	{
		return m_mtime;
	}
	
	//
	// Specifies the date and time the file was last accessed. 
	//
	CIDKDate GetAccessedTime()
	{
		return m_atime;
	}

	//
	// Specifies the logical size of the file in bytes, as reported by the DIR command.
	//
	long GetSize()
	{
		return m_size;
	}

	//
	// Specifies the attribute byte of the file. This is the logical OR of the following enumerated values: 
	//
	CFileAttrib GetAttribute()
	{
		return m_attr; //m_attribute;
	}

//	unsigned int GetAttrFlags()
//	{
//		return m_attr;
//	}

	//
	// The absolute filename including the path.
	//
	const char *GetFullName()
	{
		return m_szFullName.c_str();
	};
	
	//
	// The absolute filename excluding the path..
	//
	const char *GetName()
	{

		int len = m_szFullName.find_last_of('\\');
		if (len < 0)
		{
			len = m_szFullName.find_last_of('\\');
		}
		if (m_szFullName.length() - len == 1)
		{
			// remove tralling / i.e c:/somedir/
			m_szName = m_szFullName.substr(len);
			len = m_szName.find_last_of('\\');
			std::string l_szTmpName = m_szName.substr((m_szName.length() - len) - 1, m_szName.length());
			m_szName = l_szTmpName;
			return m_szName.c_str();
		}
		m_szName = m_szFullName.substr(m_szFullName.length() - len - 1, m_szName.length());
		return m_szName.c_str();
	}; 

	const char *GetExt()
	{
		int len = m_szFullName.find_last_of('.');
		m_szExt = m_szFullName.substr(m_szFullName.length() - len - 1, m_szFullName.length());
		return m_szExt.c_str();
	}; 

	CIDKFileStatus(CIDKFileStatus &rFileStatus)
	{
		m_szFullName = rFileStatus.m_szFullName;
		m_attr = rFileStatus.m_attr;
	//	m_attr.st_mode = CFileAttrib(szFullName);
        m_size = rFileStatus.m_size;		// File size in bytes
        m_atime = rFileStatus.m_atime;		// Time of last access
        m_mtime = rFileStatus.m_mtime;		// Time of last data modification
        m_ctime = rFileStatus.m_ctime;		// Time of last file status change
		m_szExt = rFileStatus.m_szExt;
		m_iError = rFileStatus.m_iError;
		m_szName = rFileStatus.m_szName;

	}


#ifdef _WIN32

	CIDKFileStatus(const char *szFullName)
	{
		char path_buffer[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];


		struct _stat l_pBuffer;
		/*
//		int count = 0;
		WIN32_FIND_DATA findFileData;

		HANDLE hFind = FindFirstFile(szFullName, &findFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			m_iError = -1;
//			count++;
//			if (count < 3)
//			{
//				Sleep(500);
//				goto TryAgain;
//			}
			return;
		}
		if (!FindClose(hFind))
		{
			m_iError = -1;
			return;
		}
*/
		if ((m_iError = _stat(szFullName, &l_pBuffer)) < 0)
		{
			m_iError = errno;
			perror("_Stat");
			return;
		}
	
		_splitpath(szFullName, drive, dir, fname, ext);
		if (dir[0] == '\0')
		{
			DWORD res = GetCurrentDirectory(_MAX_DIR,dir);
			m_szFullName = dir;
			m_szFullName += '\\';
			m_szFullName += fname;
			m_szFullName += ext;
		}
		else
		{
			m_szFullName = szFullName;
		}
		if (drive[0] == '\0')
		{
		}
		

		m_attr.Set(szFullName);		// File mode
	//	m_attr.st_mode = CFileAttrib(szFullName);
        m_size = l_pBuffer.st_size;		// File size in bytes
        m_atime = l_pBuffer.st_atime;		// Time of last access
        m_mtime = l_pBuffer.st_mtime;		// Time of last data modification
        m_ctime = l_pBuffer.st_ctime;		// Time of last file status change
								// Times measured in seconds since
								// 00:00:00 UTC, Jan. 1, 1970
	}


/*
	static bool IsDirectory(CIDKFileStatus &rFile)
	{
		return (rFile.m_attr & _S_IFDIR);
	}

	static bool IsFile(CIDKFileStatus &rFile)
	{
		return (rFile.m_attr & _S_IFREG);
	}

	static bool IsCharFile(CIDKFileStatus &rFile) // character special 
	{
		return (rFile.m_attr & _S_IFCHR);
	}

	static bool IsPipe(CIDKFileStatus &rFile) // pipe 
	{
		return (rFile.m_attr & _S_IFIFO);
	}

	static bool IsRead(CIDKFileStatus &rFile)
	{
		return (rFile.m_attr & _S_IREAD);
	}

	static bool IsWrite(CIDKFileStatus &rFile)
	{
		return (rFile.m_attr & _S_IWRITE);
	}

	static bool IsExec(CIDKFileStatus &rFile)
	{
		return (rFile.m_attr & _S_IEXEC);
	}
*/

	static bool IsArchive(CIDKFileStatus &rFile)
	{
		return (rFile.m_attr.IsArchive());
	}
	static bool IsCompressed(CIDKFileStatus &rFile)
	{
		return (rFile.m_attr.IsCompressed());
	}

	static bool IsDirectory(CIDKFileStatus &rFile)
	{
		return (rFile.m_attr.IsDirectory());
	}

	static bool IsEncrypted(CIDKFileStatus &rFile)
	{
		return (rFile.m_attr.IsEncrypted());
	}

	static bool IsNormal(CIDKFileStatus &rFile)
	{
		return (rFile.m_attr.IsNormal());
	}

	static bool IsReadOnly(CIDKFileStatus &rFile)
	{
		return (rFile.m_attr.IsReadOnly());
	}

	static bool IsFile(CIDKFileStatus &rFile)
	{
		return (!(rFile.m_attr.IsDirectory()));
	}
#else

	CIDKFileStatus(const char *szFullName)
	{
		struct stat l_pBuffer;

		if ((m_iError = stat(szFullName, &l_pBuffer)) < 0)
		{
			m_iError = errno;
			return;
		}
		
		m_szFullName = szFullName;
		m_attr = l_pBuffer.st_mode;		// File mode
        m_size = l_pBuffer.st_size;		// File size in bytes
        m_atime = l_pBuffer.st_atime;		// Time of last access
        m_mtime = l_pBuffer.st_mtime;		// Time of last data modification
        m_ctime = l_pBuffer.st_ctime;		// Time of last file status change
								// Times measured in seconds since
								// 00:00:00 UTC, Jan. 1, 1970
	}

	static bool IsDirectory(CIDKFileStatus &rFile)
	{
		return ((rFile.m_attr & S_IFDIR) != 0);
	}

	static bool IsFile(CIDKFileStatus &rFile)
	{
		return ((rFile.m_attr & S_IFREG) != 0);
	}

	static bool IsWritable(CIDKFileStatus &rFile)
	{
		return ((rFile.m_attr & S_IWRITE) != 0);
	}

#endif
	void Print()
	{
		Print(stdout);
	}
	void Print(FILE *fp)
	{
		fprintf(fp, "Create time %s", (char *)m_ctime.Print().c_str());
		fprintf(fp, "Mod time %s", (char *)m_mtime.Print().c_str());
		fprintf(fp, "Access time %s", (char *)m_atime.Print().c_str());

		fprintf(fp, "Size %d\n", m_size);
		CFileAttrib l_FileAttrib(m_attr);
		l_FileAttrib.Print(fp);
//	unsigned int m_attr;
		fprintf(fp, "FullName %s\n", (char *)m_szFullName.c_str());
		fprintf(fp, "Name %s\n", (char *)m_szName.c_str());
		fprintf(fp, "Ext %s\n", (char *)m_szExt.c_str());
	};
};

/*
 * CIDKFile	
 * 
 * This classes purpose is to wrap the operations required
 * to handle an input file.
 *
 * ###############################################################
 * ### Please note that its internal buffers are managed using ###
 * ### malloc(), realloc() and free() NOT new and delete       ###
 * ###############################################################
 */
class CIDKFile
{
public:
	enum {GROW_SIZE = 16};	// Number of bytes to increase buffer size by.

	/*
	 * Default Constructor
	 */
	CIDKFile() 
		:	m_strPath(""),	
			m_pszBuffer(0),
			m_lBuffSize(0),
			m_lBuffPos(0),
			m_pStream(0)
	{
		
	}

	/*
	 * Copy constructor
	 */
	CIDKFile(const CIDKFile &rhs)
		:	m_strPath(""),	
			m_pszBuffer(0),
			m_lBuffSize(0),
			m_lBuffPos(0),
			m_pStream(0)
	{
		*this = rhs;
	}

	/*
	 * Destructor
	 */
	virtual ~CIDKFile()
	{
		// Make sure the file is closed.
		Close();

		if ( m_pszBuffer )
		{
			free((void *)m_pszBuffer);
			m_pszBuffer = 0;
			m_lBuffPos	= 0;
			m_lBuffSize = 0;
		}
	}

	/*
	 * assignment operator
	 */
	CIDKFile &operator=(const CIDKFile &rhs)
	{
		// Make sure we dont copy ourself
		if ( this == &rhs )
			return *this;

		// if the rhs file is open...
		if ( rhs.IsOpen() )
		{
			bool bStatus = Open(rhs.m_strPath.c_str(),rhs.m_strMode.c_str());
			if ( true == bStatus )
			{
				// set the file pointer at the same position
				// within this new file
				long lOffset = ftell(rhs.m_pStream);
				fseek(m_pStream,lOffset,SEEK_SET);

			}
		}

		// ...now copy the rhs buffer
		m_pszBuffer = 0;
		m_lBuffSize = rhs.m_lBuffSize;
		if ( m_lBuffSize > 0 )
		{
			m_pszBuffer = (char *) malloc(m_lBuffSize);
			//IDK_THROW_NULLPTR_EXCEPTION1(m_pszBuffer);
			memcpy(m_pszBuffer,rhs.m_pszBuffer,m_lBuffSize);
		}

		// and finally set the buffer position pointer.
		m_lBuffPos = rhs.m_lBuffPos;

		return *this;
	}

	/*
	 * METHODS
	 */

	/*
	 * Open(const char *pszPath, const char *pszMode = "r")
	 * Opens the file specified by the pszPath argument
	 * with the mode specified by pszMode
	 */
	bool Open(const char *pszPath, const char *pszMode = "r" /* Open for reading */ )
	{
		m_strPath = pszPath;
		m_strMode = pszMode;

		Close();

		m_pStream = fopen(m_strPath.c_str(),m_strMode.c_str());

		return ( 0 == m_pStream ) ? false : true;
	}

	/*
	 * Close()
	 * Closes the file.
	 */
	void Close()
	{
		if ( m_pStream )
		{
			fclose(m_pStream);
			m_pStream = 0;
		}
	}
	
	/*
	 * IsOpen()
	 * Tells us whether the file is open (true) or not (false)
	 */
	const bool IsOpen() const
	{
		return (m_pStream == 0) ? false : true;
	}

	/*
	 * Read()
	 * Reads a line of text into the objects internal buffer
	 * Returns true if successful otherwise false.
	 */
	bool Read()
	{
		if ( false == IsOpen() )
			return false;

		// Set the buffer position 

		m_lBuffPos = 0;
		int ichar;
		bool bDone = false;
		bool bBslash = false;

		do
		{
			// Allocate memory to the buffer, if required
			Resize();

			// Now, get a character from the stream
			ichar = fgetc(m_pStream);

			switch(ichar)
			{
				case EOF  :		// Possible error
					bBslash = false;

				case '\r' :			
				case '\n' :
					if (!bBslash)	// backslash at EOL is a continuation mark
					{
						m_pszBuffer[m_lBuffPos] = '\0';
						bDone = true;
					}
					else bBslash = false;
					break;

				default :
					if (bBslash)	// we just read a backslash
					{
						m_pszBuffer[m_lBuffPos++] = '\\';	// put it back
						Resize();
						bBslash = false;
					}
					if (ichar == '\\')
					{
						bBslash = true;
					}
					else
					{
						m_pszBuffer[m_lBuffPos++] = ichar;
					}
					break;
			}

		} while (!bDone);

		return true;

	}

	/*
	 * IsEOF() 
	 * Returns true if the end of file false if not.
	 */
	const bool IsEOF() const
	{
		if ( false == IsOpen() )
			return false;

		return (0 == feof(m_pStream)) ? false : true;
	}

	/* 
	 * IsError()
	 * Returns true if there is an error on the stream
	 * otherwise false;
	 */
	const bool IsError() const
	{
		if ( false == IsOpen() )
			return false;

		return (0 == ferror(m_pStream)) ? false : true;
	}
	/*
	 * Reset()
	 * Resets the file pointer to the begining of the file
	 * assuming its open.
	 * Returns true if successful, false if not.
	 */
	const bool Reset() const
	{
		if ( false == IsOpen() )
			return false;

		return (0 == fseek(m_pStream, 0L, SEEK_SET)) ? true : false;
	}

	/*
	 * GetBuffer()
	 * Returns a pointer to the internal buffer
	 * containing any text read in from file.
	 */
	char * GetBuffer() { return m_pszBuffer; }

	operator char *()
	{
		return m_pszBuffer;
	}

protected:
	/*
	 * Resize()
	 * Re sizes the buffer m_pszBuffer if more memory is required.
	 */
	void Resize()
	{

		// If buffer has not been allocated yet, then do so.
		if ( 0 == m_pszBuffer )
		{
			// We will start off with a buffer 5 * greater than the grow size.
			m_lBuffSize = 5 * CIDKFile::GROW_SIZE;
		}
		else
		{
			// Buffer already allocated, however do we require more memory??
			if ( m_lBuffPos >= m_lBuffSize )
			{
				m_lBuffSize += CIDKFile::GROW_SIZE;
			}
			else
			{
				return;
			}
		}

		// If we've got this far, then lets allocate some memory.
		char *pszTemp;

		pszTemp = (char *) realloc((void *) m_pszBuffer, m_lBuffSize);

		//IDK_THROW_NULLPTR_EXCEPTION1(pszTemp);

		m_pszBuffer = pszTemp;

		return;

	}

private:
	std::string	m_strPath;			// Pathname of file
	std::string	m_strMode;			// Mode to open file with
	char	*m_pszBuffer;		// Buffer used to read data in from file
	long	m_lBuffSize;		// Current buffer size
	long	m_lBuffPos;			// Current character pos within m_pszBuffer;
	FILE	*m_pStream;			// File handle;
};

#endif
#endif // XXXXXXXXXX


