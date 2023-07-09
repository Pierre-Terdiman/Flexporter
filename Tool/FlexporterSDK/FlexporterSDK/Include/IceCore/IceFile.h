///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains file related code.
 *	\file		IceFile.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEFILE_H__
#define __ICEFILE_H__

	#define FILE_SAVE	false		//!< Fileselect parameter.
	#define FILE_LOAD	true		//!< Fileselect parameter.

	// File access
	#define FILE_READ				0x1
	#define FILE_WRITE				0x2
	#define	FILE_SHAREREAD			0x4
	#define	FILE_SHAREWRITE			0x8

	// File attrib
	#define FILE_ATTRB_READONLY		0x00000001  
	#define FILE_ATTRB_HIDDEN		0x00000002  
	#define FILE_ATTRB_SYSTEM		0x00000004  
	#define FILE_ATTRB_DIRECTORY	0x00000008  
	#define FILE_ATTRB_ARCHIVE		0x00000010  
	#define FILE_ATTRB_NORMAL		0x00000020  

	// File open flags
	enum FILE_CREATEATTRB
	{
		FILE_CREATE_NEW				= 1,							// Create the file, fails if the same filename already exists
		FILE_CREATE_ALWAYS			= 2,							// Create or open an existing file (with truncation to zero)
		FILE_OPEN_EXISTING			= 3,							// Open an existing file, fails if the file doesn't exist
		FILE_OPEN_ALWAYS			= 4,							// Open an existing file, or create one if it doens't exist

		FILE_OPFORCEDW				= 0x7fffffff
	};

	// File seek flags
	enum FILE_SEEK
	{
		FILE_FROM_BEGIN				= 1,
		FILE_FROM_CURRENT			= 2, 
		FILE_FROM_END				= 4,

		FILE_FROM_FORCE				= 0x7fffffff
	};

	// Misc functions

	//ICECORE_API	bool		FileSelect(const char* type, const char* destname, const char* alias, bool mode, char* filename);

	FUNCTION ICECORE_API	udword		GetFileSize(const char* name);
	FUNCTION ICECORE_API	bool		FileExists(const char* filename);
	FUNCTION ICECORE_API	const char*	ChangeExtension(const char* filename, const char* newext);
	FUNCTION ICECORE_API	bool		RemoveExtension(String& filename);
	FUNCTION ICECORE_API	const char*	GetExtension(const String& filename, String* extension=null);

	FUNCTION ICECORE_API	udword		CreateFile(String& filepathname, udword fileaccess, FILE_CREATEATTRB fileattrib);
	FUNCTION ICECORE_API	bool		CloseFile(udword handle);
	FUNCTION ICECORE_API	udword		SeekFilePtr(udword handle, udword length, FILE_SEEK seek);
	FUNCTION ICECORE_API	bool		WriteFile(udword handle, const void* buffer, udword length, udword* writtenlength=null);
	FUNCTION ICECORE_API	bool		ReadFile(udword handle, void* buffer, udword length, udword* readlength);
	FUNCTION ICECORE_API	udword		GetFilePosition(udword handle);
	//FUNCTION ICECORE_API	udword		GetFileLength(udword handle);
	FUNCTION ICECORE_API	bool		DeleteFile(String& filepathname);
	FUNCTION ICECORE_API	bool		CopyFile(String& existingfile, String& newfile, bool overwrite);
	FUNCTION ICECORE_API	bool		MoveFile(String& existingfile, String& newfile);
	FUNCTION ICECORE_API	udword		GetFileAttributes(String& filepathname);

	#define	PATH_MAX	_MAX_PATH

	// Paths
	FUNCTION ICECORE_API	bool		RegisterPath(const char* path);
	FUNCTION ICECORE_API	bool		ReleasePaths();
	FUNCTION ICECORE_API	bool		FindFile(const char* filename, String* file=null);
	FUNCTION ICECORE_API	const char* GetCurrentDirectory();
	FUNCTION ICECORE_API	bool		RemovePath(String& filename);
	FUNCTION ICECORE_API	bool		GetPath(const String& filename, String& path);

	#define	FINDFILE_DIR			0x01
	#define	FINDFILE_HIDDEN			0x02
	#define	FINDFILE_NORMAL			0x04
	#define	FINDFILE_READONLY		0x08

	class ICECORE_API IceFile
	{
		public:
		// Constructor/Destructor
									IceFile(const char* filename, const char* access);
									~IceFile();

		inline_	bool				IsValid()		const	{ return mFp || mBuffer;	}

				ubyte				GetByte();
				uword				GetWord();
				udword				GetDword();
				float				GetFloat();
				const char*			GetString();
				bool				GetBuffer(void* dest, udword size);
				bool				Seek(udword pos);

				bool				GetLine(char* buffer);
				ubyte*				Load(udword& length);
		private:
				String				mName;
				FILE*				mFp;

				ubyte*				mBuffer;
				udword				mBufferLength;
	};

	class ICECORE_API FileFinder
	{
		public:
		// Constructor/Destructor
									FileFinder(const char* mask);
				bool				FindNext();
				bool				IsValid()	const	{ return (mHandle==(void*)-1)==0; }
		public:
				String				mFile;
				udword				mAttribs;
				udword				mSize;
		private:
				WIN32_FIND_DATA		mFindData;
				HANDLE				mHandle;
	};

	#define FILEFOUND		__ffice__.mFile;

	#define STARTFINDFILES(x)				\
	FileFinder	__ffice__(x);				\
	while(__ffice__.IsValid())				\
	{

	#define ENDFINDFILES					\
		__ffice__.FindNext();				\
	}

	FUNCTION ICECORE_API void UnixToDosName(char* name);
	FUNCTION ICECORE_API void DosToUnixName(char* name);

#endif // __ICEFILE_H__


