///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains file related code.
 *	\file		IceFile.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceCore;

static Container* gPaths = null;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Registers a global path, further used to retrieve various files.
 *	\fn			RegisterPath(const char* path)
 *	\param		path	[in] the pathname
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceCore::RegisterPath(const char* path)
{
	// Checkings
	if(!path)	return false;

	// Allocate container on first call
	if(!gPaths)
	{
		gPaths = new Container;
		CHECKALLOC(gPaths);
	}

	// Check that path does not already exist
	for(udword i=0;i<gPaths->GetNbEntries();i++)
	{
		// Get current path
		String* CurPath = (String*)gPaths->GetEntry(i);
		// Check
		if(CurPath && strcmp(CurPath->Get(), path)==0)	return true;
	}

	// Allocate a new path
	String* NewPath = new String;
	CHECKALLOC(NewPath);

	// Keep track of it
	gPaths->Add(udword(NewPath));

	// Setup new string
	return NewPath->Set(path);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Releases all registered paths.
 *	\fn			ReleasePaths()
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceCore::ReleasePaths()
{
	if(gPaths)
	{
		// Loop through paths
		for(udword i=0;i<gPaths->GetNbEntries();i++)
		{
			// Get current path
			String* CurPath = (String*)gPaths->GetEntry(i);
			// Release current path
			DELETESINGLE(CurPath);
		}
		DELETESINGLE(gPaths);
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Locates a file, using all registered paths.
 *	Ex: a user registered path "c:/textures", then looks for texture file "Wood01.bmp". Calling the
 *	function search the registered paths, and eventually return "c:/textures/Wood1.bmp" if such a
 *	file has been found. Please note you can use / or \ characters (even mixed).
 *
 *	\fn			FindFile(const char* filename, String& file)
 *	\param		filename	[in] the file we're looking for
 *	\param		file		[out] the file completed with a path (or null if you just want to know it has been found)
 *	\return		true if the file has been found somewhere
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceCore::FindFile(const char* filename, String* file)
{
	// Checkings
	if(!filename)	return false;

	// First thing to do is to test the root directory !
	if(FileExists(filename))
	{
		// Found in the root dir
		if(file)	*file = filename;
		return true;
	}

	// Loop through paths
	if(gPaths)
	{
		// Loop through paths
		for(udword i=0;i<gPaths->GetNbEntries();i++)
		{
			// Get current path
			String* CurPath = (String*)gPaths->GetEntry(i);

			// Work string
			String TmpFile = *CurPath;

			// Catch the string's length to access last character
			udword LastChar = TmpFile.Length();
			// Check the registered path is not whacked
			if(LastChar)
			{
				// Last char index = length - 1
				LastChar--;

				// Add a separator if there isn't one already
				if(TmpFile[LastChar]!='/' && TmpFile[LastChar]!='\\')	TmpFile+="/";

				// Append the filename
				TmpFile+=filename;

				// Check resulting path
				if(FileExists(TmpFile))
				{
					if(file)	*file = TmpFile;
					return true;
				}
			}
		}
	}
	return false;
}


/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FileSelect(const char* type, const char* destname, const char* alias, bool mode, char* filename)
{
	// Setup flags
	long Flags = mode ? OFN_PATHMUSTEXIST|OFN_HIDEREADONLY : OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY;

	// Create a fileselect dialog
	CFileDialog FileDlg(mode, type, destname, Flags, alias);

	// Setup title
	FileDlg.m_ofn.lpstrTitle = "Select file";

	// Call dialog
	if(FileDlg.DoModal() != IDOK) return false;

	// Get the filename back and copy it
	CString FName = FileDlg.GetPathName();
	if(strlen((const char*)FName)>256) return false;
	strcpy(filename, FName);

	return true;
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets the size of a given file.
 *	\fn			GetFileSize(const char* name)
 *	\param		name		[in] the filename
 *	\return		the length of the file in bytes
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword IceCore::GetFileSize(const char* name)
{
	#ifndef SEEK_END
	#define SEEK_END 2
	#endif

	FILE* File = fopen(name, "rb");
	if(!File)
	{
		SetIceError("IceCore::GetFileSize: file not found.");
		return 0;
	}
	fseek(File, 0, SEEK_END);
	udword eof_ftell = ftell(File);
	fclose(File);
	return eof_ftell;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks whether a file exists or not.
 *	\fn			FileExists(const char* filename)
 *	\param		name		[in] the filename
 *	\return		true if the file exists, else false.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceCore::FileExists(const char* filename)
{
	if(!filename)			return SetIceError("IceCore::FileExists: null filename!");
	if(!strlen(filename))	return SetIceError("IceCore::FileExists: invalid filename!");
	FILE* File = fopen(filename, "rb");
	if(!File)	return false;
	fclose(File);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Changes a file's extension.
 *	\fn			ChangeExtension(const char* filename, const char* newext)
 *	\param		filename		[in] the filename
 *	\param		newext			[in] the new extension to put
 *	\return		the new string
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* IceCore::ChangeExtension(const char* filename, const char* newext)
{
	static char NewName[256];
	for(udword i=0;i<(udword)strlen(filename);i++)
	{
		NewName[i] = tolower(filename[i]);
	}
	char* p = strrchr(NewName, '.');
	*++p='\0';
	strcat(NewName, newext);
	return(NewName);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	File class.
 *	\class		IceFile
 *	\author		Pierre Terdiman
 *	\version	1.0
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IceFile::IceFile(const String& filename, const char* access)
{
	mBufferLength	= 0;
	mBuffer			= null;
	mFp				= access ? fopen(filename, access) : fopen(filename, "rb");
	mName			= filename;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IceFile::~IceFile()
{
	if(mFp)	fclose(mFp);

	DELETEARRAY(mBuffer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A method to catch a line in an already opened text file.
 *	\param		buffer		[out] the output buffer
 *	\return		true if success, else false if no more lines.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceFile::GetLine(char* buffer)
{
	int c;
	udword i=0;
	do{
		c = fgetc(mFp);
		if(c==EOF) return false;
		if((c!=0x0d)&&(c!=0x0a))	buffer[i++]=(char)c;
	}while(c!=0x0d);
	buffer[i] = 0;
	return true;
/*
	int c;
	udword i=0;
	do{
		do{
			c = fgetc(mFp);
			if(c==EOF) return false;
			if((c!=0x0d)&&(c!=0x0a))
			{
				buffer[i++]=(char)c;
			}
		}while(c!=0x0d);

		c = fgetc(mFp);
		if(c==EOF) return false;
		if((c!=0x0d)&&(c!=0x0a))
		{
			buffer[i++]=(char)c;
		}
	}while(c!=0x0a);
	buffer[i] = 0;
	return true;*/
}

// Check file pointer
#define CHECK_FILE_PTR(err)		if(!mFp)	{ SetIceError("IceFile::Load: invalid file pointer");	return err;}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A method to load the whole file in a buffer.
 *	\param		length		[out] the buffer length
 *	\return		the buffer address, or null if failed
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ubyte* IceFile::Load(udword& length)
{
	CHECK_FILE_PTR(null)

	// Check file size
	length = GetFileSize(mName);
	if(!length)	{ SetIceError("IceFile::Load: file has zero length");	return null;}

	// Get some bytes
	DELETEARRAY(mBuffer);
	mBuffer = new ubyte[length];
	if(!mBuffer)	{ SetIceError("IceFile::Load: out of memory");	return null;}

	// Read the file
	fread(mBuffer, length, 1, mFp);

	// Close and free
	fclose(mFp);	mFp = null;

	// Return stored bytes
	return mBuffer;
}

ubyte IceFile::GetByte()
{
	CHECK_FILE_PTR(0)

	ubyte b;
	fread(&b, sizeof(ubyte), 1, mFp);
	return b;
}

uword IceFile::GetWord()
{
	CHECK_FILE_PTR(0)

	uword c;
	fread(&c, sizeof(uword), 1, mFp);
	return c;
}

udword IceFile::GetDword()
{
	CHECK_FILE_PTR(0)

	udword d;
	fread(&d, sizeof(udword), 1, mFp);
	return d;
}

float IceFile::GetFloat()
{
	CHECK_FILE_PTR(0.0f)

	float f;
	fread(&f, sizeof(float), 1, mFp);
	return f;
}

const char* IceFile::GetString()
{
	CHECK_FILE_PTR(null)

	static char Buf[1024];
	udword Offset=0;
	ubyte b;
	do
	{
		b = GetByte();
		Buf[Offset++] = b;
	}
	while(b);
	return Buf;
}

bool IceFile::GetBuffer(ubyte* dest, udword size)
{
	if(!dest || !size) return false;
	CHECK_FILE_PTR(false)

	fread(dest, size, 1, mFp);
	return true;
}

bool IceFile::Seek(udword pos)
{
	CHECK_FILE_PTR(false)

	return fseek(mFp, pos, SEEK_SET)==0;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A file finder.
 *	\class		FileFinder
 *	\author		Pierre Terdiman
 *	\version	1.0
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Construtor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input	: 
//	- filepathname, file & path name of the files to look for (wildcards are supported)
// Output	: None
// Return	: None
// Exception: None
// Remark	: The constructor find the first matching file, use the data member to get
//	the file handle (mH), the file name (mFile), the file size (mSize) and the file 
//	attributes (mAttrbs).
//	To get the next matching file, call the FindFiles::FindNext() method.
//  Check if at least one file was found by calling FindFiles::IsValid().
FileFinder::FileFinder(const char* mask)
{
	mHandle = FindFirstFile (mask, &mFindData);

	// Did we find a file ?
	if(!IsValid())
	{
		mFile		= "";
		mSize		= 0;
		mAttribs	= 0;
	}
	else
	{
		mFile		= mFindData.cFileName;
		mSize		= mFindData.nFileSizeLow;
		mAttribs	= 0;
		mAttribs	|= (mFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	? FINDFILE_DIR		: 0;
		mAttribs	|= (mFindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)		? FINDFILE_HIDDEN	: 0;
		mAttribs	|= (mFindData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)		? FINDFILE_NORMAL	: 0;
		mAttribs	|= (mFindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)	? FINDFILE_READONLY	: 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Find the next file
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input	: None
// Output	: None
// Return	: true if a file was found, false if there's no more matching files.
// Exception: None
// Remark	: Use the same data members to retrieve the file information as
//	for the constructor.
bool FileFinder::FindNext()
{
	if(!FindNextFile(mHandle, &mFindData))
	{
		mHandle = (void*)-1;
		FindClose(mHandle);
		return false;
	}

	mFile		= mFindData.cFileName;
	mSize		= mFindData.nFileSizeLow;
	mAttribs	= 0;
	mAttribs	|= (mFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	? FINDFILE_DIR		: 0;
	mAttribs	|= (mFindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)		? FINDFILE_HIDDEN	: 0;
	mAttribs	|= (mFindData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)		? FINDFILE_NORMAL	: 0;
	mAttribs	|= (mFindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)	? FINDFILE_READONLY	: 0;

	return true;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create File function
// For fileaccess and fileattrib signification, see the enum definition
// Return -1 if the function fails
udword IceCore::CreateFile(String& filepathname, udword fileaccess, FILE_CREATEATTRB fileattrib)
{
	udword W32FA = ((fileaccess&FILE_READ) ? GENERIC_READ : 0) | ((fileaccess&FILE_WRITE) ? GENERIC_WRITE : 0);
	udword W32FC;
	udword W32S = ((fileaccess&FILE_SHAREREAD)?FILE_SHARE_READ:0) | ((fileaccess&FILE_SHAREWRITE)?FILE_SHARE_WRITE:0);
	switch (fileattrib)
	{
		case FILE_CREATE_NEW:		W32FC = CREATE_NEW;			break;
		case FILE_CREATE_ALWAYS:	W32FC = CREATE_ALWAYS;		break;
		case FILE_OPEN_EXISTING:	W32FC = OPEN_EXISTING;		break;
		case FILE_OPEN_ALWAYS:		W32FC = OPEN_ALWAYS;		break;
	}
		
	return (udword)::CreateFile((LPCSTR)filepathname, W32FA, W32S, NULL, W32FC, FILE_ATTRIBUTE_NORMAL, NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Close a file
// Return false if the function fails
bool IceCore::CloseFile(udword handle)
{
	return CloseHandle((HANDLE)handle)!=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Seek the file pointer
// Return -1 if the function fails
udword IceCore::SeekFilePtr(udword handle, udword length, FILE_SEEK seek)
{
	udword W32S = ((seek&FILE_FROM_BEGIN) ? FILE_BEGIN : 0) | ((seek&FILE_FROM_CURRENT) ? FILE_CURRENT : 0) | ((seek&FILE_FROM_END) ? FILE_END : 0);
	return SetFilePointer((HANDLE)handle, length, NULL, W32S);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Return false if the function fails
bool IceCore::WriteFile(udword handle, const void* buffer, udword length, udword* writtenlength)
{
	udword written;
	return ::WriteFile((HANDLE)handle, buffer, length, (DWORD*)(writtenlength?writtenlength:&written), NULL)!=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Return false if the function fails
bool IceCore::ReadFile(udword handle, void* buffer, udword length, udword* readlength)
{
	udword read;
	return ::ReadFile((HANDLE)handle, buffer, length, (DWORD*)(readlength?readlength:&read), NULL)!=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Return -1 if the function fails
udword IceCore::GetFilePosition(udword handle)
{
	return SetFilePointer((HANDLE)handle, 0, NULL, FILE_CURRENT);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Return -1 if the function fails
//ICECORE_API udword IceCore::GetFileLength(udword handle)
//{
//	return GetFileSize((HANDLE)handle, NULL);
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Returns true if successful
bool IceCore::DeleteFile(String& filepathname)
{
	return ::DeleteFile((LPCSTR)filepathname)!=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Returns true if successful
bool IceCore::CopyFile(String& existingfile, String& newfile, bool overwrite)
{
	return ::CopyFile((LPCSTR)existingfile, (LPCSTR)newfile, !overwrite)!=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Returns true if successful
bool IceCore::MoveFile(String& existingfile, String& newfile)
{
	return ::MoveFile((LPCSTR)existingfile, (LPCSTR)newfile)!=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Returns -1 if the function fails
udword IceCore::GetFileAttributes(String& filepathname)
{
	udword W32ATR = ::GetFileAttributes((LPCSTR)filepathname);
	if (W32ATR==-1)													return -1;

	udword ATR=0;
	if (W32ATR & FILE_ATTRIBUTE_READONLY	)		ATR	|=	FILE_ATTRB_READONLY;
	if (W32ATR & FILE_ATTRIBUTE_HIDDEN		)		ATR	|=	FILE_ATTRB_HIDDEN;
	if (W32ATR & FILE_ATTRIBUTE_SYSTEM		)		ATR	|=	FILE_ATTRB_SYSTEM;
	if (W32ATR & FILE_ATTRIBUTE_DIRECTORY	)		ATR	|=	FILE_ATTRB_DIRECTORY;
	if (W32ATR & FILE_ATTRIBUTE_ARCHIVE		)		ATR	|=	FILE_ATTRB_ARCHIVE;
	if (W32ATR & FILE_ATTRIBUTE_NORMAL		)		ATR	|=	FILE_ATTRB_NORMAL;
	return	ATR;
}


void IceCore::UnixToDosName(char* name)
{
	if(!name)	return;

	for(int i=0; ;i++)
	{
		if(name[i]=='\0')	break;
		if(name[i]=='/')	name[i]='\\';
	}
}

void IceCore::DosToUnixName(char* name)
{
	if(!name)	return;

	for(int i=0; ;i++)
	{
		if(name[i]=='\0')	break;
		if(name[i]=='\\')	name[i]='/';
	}
}

const char* IceCore::GetCurrentDirectory()
{
	static char	CurrentDir[PATH_MAX];

	// Get current path
	_getcwd(CurrentDir, PATH_MAX);
	strcat(CurrentDir, "\\");

	return CurrentDir;
}

bool IceCore::RemovePath(String& filename)
{
	// Checkings
	if(!filename.IsValid())	return false;

	// Look for '\'
	char* Separator = strrchr(filename.Get(), '\\');

	// Look for '/' if not found
	if(!Separator)	Separator = strrchr(filename.Get(), '/');

	// Definitely no path ? Done!
	if(!Separator)	return false;

	// Create a temporary string....
	String tmp(++Separator);

	// Copy & return
	filename = tmp;
	return true;
}

bool IceCore::GetPath(const String& filename, String& path)
{
	// Checkings
	if(!filename.IsValid())	return false;

	// Look for '\'
	char* Separator = strrchr(filename.Get(), '\\');

	// Look for '/' if not found
	if(!Separator)	Separator = strrchr(filename.Get(), '/');

	// Definitely no path ? Done!
	if(!Separator)	return false;

	// Create a temporary string....
	udword Offset = udword(Separator - filename.Get());
	path = filename.Left(Offset+1);
	return true;
}
