///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains all string-related code.
 *	\file		IceString.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Simple string class.
 *	\class		String
 *	\author		Pierre Terdiman
 *	\version	1.0
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceCore;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Sorts a list of strings in lexicographical order. Default implementation uses qsort().
 *	\relates	String
 *	\fn			SortStrings(udword nb, void* list, udword stride)
 *	\param		nb		[in] number of strings to sort
 *	\param		list	[in] list of strings. Actually a list of structures whose first member is a String*.
 *	\param		stride	[in] structure's extra stride, not counting sizeof(String*). Default to 0.
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceCore::SortStrings(udword nb, void* list, udword stride)
{
	// Checkings
	if(!nb || !list)	return false;

	struct Local
	{
		static int SortCB(const void* elem1, const void* elem2)
		{
			const String* s1 = *(const String**)elem1;
			const String* s2 = *(const String**)elem2;
			return s1->Compare(*s2);
		}
	};
	qsort(list, nb, stride+sizeof(String*), Local::SortCB);
	return true;
}

// Static members
#ifdef STRING_STATS
udword String::mNbStrings = 0;
udword String::mUsedRam = 0;
#endif
//static char NullText[] = "(null string)";
//char* String::Default = NullText;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Empty constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String::String() : mText(null)
{
#ifdef STRING_STATS
	mNbStrings++;
	mUsedRam+=sizeof(String);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor from a character string.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String::String(const char* string) : mText(null)
{
#ifdef STRING_STATS
	mNbStrings++;
	mUsedRam+=sizeof(String);
#endif
	Set(string);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor from a String.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String::String(const String& str) : mText(null)
{
#ifdef STRING_STATS
	mNbStrings++;
	mUsedRam+=sizeof(String);
#endif
	Set(str.mText);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String::~String()
{
	Reset();
#ifdef STRING_STATS
	mNbStrings--;
	mUsedRam-=sizeof(String);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Reset the string.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void String::Reset()
{
	if(!mText)	return;
#ifdef STRING_STATS
	udword Length = GetLength();
	if(Length)	Length++;
	mUsedRam-=Length;
#endif
	DELETEARRAY(mText);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Set the character string held in the class.
 *	\param		string		[in] the character string.
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool String::Set(const char* string)
{
	Reset();
	if(string)
	{
		udword Length = strlen(string);
		if(Length)
		{
			mText = new char[Length+1];
			CHECKALLOC(mText);
			CopyMemory(mText, string, Length);
			mText[Length] = 0;
#ifdef STRING_STATS
			mUsedRam+=Length+1;
#endif
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Set the character string held in the class.
 *	\param		string		[in] the character string.
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool String::Set(const String& string)
{
	return Set(string.Get());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Returns the length of the string. If the string object is empty, return 0. The length doesn't include the terminate ('\0') character.
 *	\return		The length of the string.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword String::Length() const
{
	if(!mText)	return 0;
	return strlen((const char*)mText);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	String comparisons.
 *	\param		str2		[in] the string to be compared with.
 *	\return		sdword		a C-compliant comparison code.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sdword String::Compare(const String& str2) const
{
	if(!str2.mText)	return INVALID_ID;
	// Using stricmp instead of strcmp is pretty important if you want to sort your strings lexicographically
	return stricmp(mText, str2.mText);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	String comparisons.
 *	\param		str2		[in] the string to be compared with.
 *	\return		sdword		a C-compliant comparison code.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sdword String::Compare(const char* str2) const
{
	if(!str2)	return INVALID_ID;
	// Using stricmp instead of strcmp is pretty important if you want to sort your strings lexicographically
	return stricmp(mText, str2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	String fuzzy comparisons. Original code by John Ratcliff.
 *	\param		str2		[in] the string to be compared with.
 *	\return		udword		a probability between 0 (totally different strings) and 100 (exact same strings)
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword String::FuzzyCompare(const String& str2) const
{
	const char* s1 = Get();
	const char* s2 = str2.Get();

	struct Local
	{
		static int _GCsubstr(const char* st1, const char* end1, const char* st2, const char* end2)
		{
			if(end1<=st1) return 0;
			if(end2<=st2) return 0;
			if(end1==(st1+1) && end2==(st2+1)) return 0;

			const char* s1;
			const char* s2;
			int max = 0;
			const char* b1 = end1;
			const char* b2 = end2;

			for(const char* a1=st1;a1<b1;a1++)
			{
				for(const char* a2=st2;a2<b2;a2++)
				{
					if(*a1==*a2)
					{
						int i;
						for(i=1;a1[i] && (a1[i]==a2[i]);i++);

						if(i>max)
						{
							max = i; s1 = a1; s2 = a2;
							b1 = end1 - max; b2 = end2 - max;
						}
					}
				}
			}

			if(!max) return 0;

			max += _GCsubstr(s1+max, end1, s2+max, end2);
			max += _GCsubstr(st1, s1, st2, s2);

			return max;
		}
	};
	int l1 = strlen(s1);
	int l2 = strlen(s2);

	if(l1==1)
		if(l2==1)
			if(*s1==*s2)
				return(100);

	return(200 * Local::_GCsubstr(s1, s1+l1, s2, s2+l2)/(l1+l2));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Transforms a string to upper case.
 *	\return		Self-Reference.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String& String::ToUpper()
{
	if(mText)
	{
		udword L = Length();
		for(udword i=0;i<L;i++)	mText[i] = (char)toupper(mText[i]);
	}
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Transforms a string to lower case.
 *	\return		Self-Reference.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String& String::ToLower()
{
	if(mText)
	{
		udword L = Length();
		for(udword i=0;i<L;i++)	mText[i] = (char)tolower(mText[i]);
	}
	return *this;
}

bool String::IsDecimal(sdword* value)	const
{
	if(!mText)	return false;

	// Look for hexadecimal values
	if(mText[0] && mText[0]=='0' && mText[1]=='x')
	{
		// Allowed bytes: 0123456789abcdef
		udword i=2;
		while(mText[i])
		{
			if(		mText[i]!='a'
				&&	mText[i]!='b'
				&&	mText[i]!='c'
				&&	mText[i]!='d'
				&&	mText[i]!='e'
				&&	mText[i]!='f'
				&& (mText[i]<'0' || mText[i]>'9'))	return false;
			i++;
		}
	}
	else
	{
		// Allowed bytes: -0123456789
		udword i=0;
		while(mText[i])
		{
			if(mText[i]!='-' && (mText[i]<'0' || mText[i]>'9'))	return false;
			i++;
		}
	}
	if(value)	*value = ToDecimal();
	return true;
}

bool String::IsWord(sword* value)	const
{
	sdword Value32;
	if(!IsDecimal(&Value32))	return false;
	if( (Value32&0xffff0000)==0xffff0000 || (Value32&0xffff0000)==0)
	{
		if(value)	*value = Value32;
		return true;
	}
	return false;
}

bool String::IsByte(sbyte* value)	const
{
	sdword Value32;
	if(!IsDecimal(&Value32))	return false;
	if( (Value32&0xffffff00)==0xffffff00 || (Value32&0xffffff00)==0)
	{
		if(value)	*value = Value32;
		return true;
	}
	return false;
}

bool String::IsFloat(float* value)	const
{
	if(!mText)	return false;
	// Allowed bytes: -0123456789.f
	udword i=0;
	while(mText[i])
	{
		if(mText[i]=='f')
		{
			if(mText[i+1])	return false;
		}
		else if(mText[i]!='.' && mText[i]!='-' && (mText[i]<'0' || mText[i]>'9'))	return false;
		i++;
	}
	if(value)	*value = ToFloat();
	return true;
}

bool String::IsFile()	const
{
	if(!mText)	return false;
	return FileExists(mText);	
}

bool String::IsBool(bool* value)	const
{
	String Copy(*this);
	Copy.ToLower();
	if(Copy=="false")	{ if(value)	*value=false; return true; }
	if(Copy=="true")	{ if(value)	*value=true; return true; }
	return false;
}

sdword String::ToDecimal()	const
{
	sdword Result=0;

	// Look for hexadecimal values
	if(mText[0] && mText[0]=='0' && mText[1]=='x')
	{
		udword i=2;
		while(mText[i])
		{
			Result*=16;
			if(mText[i]>='a' && mText[i]<='f')	Result+=udword(mText[i]-'a'+10);
			if(mText[i]>='0' && mText[i]<='9')	Result+=udword(mText[i]-'0');
			i++;
		}
		return Result;
	}
	else
	{
		udword i=0;
		while(mText[i])
		{
			if(mText[i]!='-')
			{
				Result*=10;
				Result+=udword(mText[i]-'0');
			}
			i++;
		}
		if(mText[0]=='-')	return -Result;
		else				return Result;
	}
}

float String::ToFloat()	const
{
	udword i=0;
	float Result=0.0f;
	bool Coma = false;
	float Divider = 1.0f;
	while(mText[i])
	{
		if(mText[i]=='.')	Coma = true;
		else if(mText[i]!='-' && mText[i]!='f')
		{
			Result*=10.0f;
			Result+=float(mText[i]-'0');
			if(Coma)	Divider*=0.1f;
		}
		i++;
	}
	Result*=Divider;
	if(mText[0]=='-')	return -Result;
	else				return Result;
}

udword String::CountNbArgs() const
{
	if(!mText)	return 0;
	udword NbArgs = 0;
	for(const char* Search = mText; *Search != '\0'; Search++)
	{
		if(Search[0]=='%' && Search[1]!='%')	NbArgs++;
	}
	return NbArgs;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Replaces a character with another.
 *	\param		old_char	[in] character to find and replace
 *	\param		new_char	[in] new character
 *	\return		number of replaced characters
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword String::Replace(char old_char, char new_char)
{
	if(!mText)	return 0;
	udword i=0;
	udword Count=0;
	while(mText[i])
	{
		if(mText[i]==old_char)
		{
			mText[i] = new_char;
			Count++;
		}
		i++;
	}
	return Count;
}

// Concatenation operator
String&	String::operator+=(const String& string)
{
	udword Len = Length() + string.Length();
	char* Cat = new char[Len+1];
	if(Cat)
	{
		Cat[0] = 0;
		if(mText)			strcpy(Cat, mText);
		if(string.mText)	strcat(Cat, string.mText);
		Cat[Len] = 0;
		Set((const char*)Cat);
		DELETEARRAY(Cat);
	}
	return *this;
}

String&	String::NewLine()
{
	udword Len = Length() + 2;
	char* Cat = new char[Len+1];
	if(Cat)
	{
		Cat[0] = 0;
		if(mText)			strcpy(Cat, mText);
		Cat[Len-2] = ASCII_CARRIAGE_RETURN;
		Cat[Len-1] = ASCII_NEXT_LINE;
		Cat[Len] = 0;
		Set((const char*)Cat);
		DELETEARRAY(Cat);
	}
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets a parameter from a command line.
 *	\param		param		[out] a string where the parameter is stored.
 *	\param		curpar		[in/out] a null pointer updated by the method each time you call it
 *	\return		true if success, false if there isn't any parameter left.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool String::GetParameter(String& param, char*& curpar) const
{
	struct Local{
		// Get the next valid parameter
		static char* NextParameter(char* string, String& param)
		{
			char *p = string;

			// Skip possible spaces
			do{
				if(*p==' ')	p++;
			}while(*p==' ');

			// No more bytes? => end of parameter list
			if(*p==0)
			{
				param = "";
				return null;
			}

			// Copy current param
//			char Parameter[4096];
			CustomArray Parameter;
//			udword i=0;
			do
			{
//				Parameter[i++] = *p++;
				Parameter.Store(*p++);
			}while(*p!=' ' && *p!='\0');
//			Parameter[i]=0;
			Parameter.Store(ubyte(0));

//			param = Parameter;
			param.Set((const char*)Parameter.Collapse());

			return p;
		}
	};

	// If curpar is null, the method is called for the first time. Then, we initialize curpar with mText.
	if(!curpar)	curpar = mText;

	// If curpar is not null, it contains the address of previous parameter.
	if(curpar)	curpar = Local::NextParameter(curpar, param);

	// If curpar is null, there's no more parameter
	return curpar!=null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Computes the number of parameters included in a string.
 *	\return		number of parameters
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword String::NbParams() const
{
	String Work = *this;
	String Param;
	char* c = null;
	udword NbParams = 0;
	while(Work.GetParameter(Param, c))	NbParams++;
	return NbParams;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Retrieves a character at a given position within the current String object.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : the index of the character to retrieve within the String object
// Output   : None
// Return   : the retrieved character, or 0 (zero) if the index is meaningless.
// Comments : None
ubyte String::GetAt(udword pos)
{
	if(pos<GetLength())	return mText[pos];
	else				return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Change a character at a given position within the current String object.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : the index of the character to change within the String object, the new character.
// Output   : None
// Return   : self-reference
// Comments : None
String& String::SetAt(ubyte cara, udword pos)
{
	if(pos<GetLength())	mText[pos] = cara;
	return *this;
}


String String::Mid(udword first, udword count)	const
{
	if (count > (GetLength() - first))			count = (udword) -1;

	udword	lengthToExtract = (count == (udword) -1 ? GetLength() - first : count);
	ubyte*	temp = new ubyte[lengthToExtract + 1];
	CopyMemory(temp, mText + first, lengthToExtract);
	temp[lengthToExtract] = '\0';
	String	ret((const char*)temp);
	DELETEARRAY(temp);

	return ret;
}

//If count==-1: all the String
String String::Left(udword count)	const
{
	return Mid(0, count);
}

String String::Right(udword count)	const
{
	if (count > GetLength())	return String("");
	return Mid(GetLength() - count);
}


sdword String::Find(ubyte character, udword from, sdword to)
{
	udword	length = GetLength();
	if(from>=length)		return -1;
	if((udword)to < from)	return -1;
	if(to==-1)				to = (length ? length - 1 : 0);

	udword	pos = from;
	while((pos <= (udword) to) && (pos < length) && (mText[pos] != character))	pos++;

	return((pos == (udword) to + 1) || (pos == length) ? -1 : pos);
}

sdword String::Find(const String& stringtofind, udword from, sdword to)
{
	udword	length = GetLength();
	ubyte*	bufToCompare = (ubyte*)stringtofind.GetBuffer();
	udword	lengthToCompare = stringtofind.GetLength();

	if(from >= length)		return -1;
	if((udword) to < from)	return -1;
	if(to == -1)			to = (length ? length - 1 : 0);

	udword	pos = from;

	while ((pos <= (udword) to) && (pos < length))		{
		if (mText[pos] == bufToCompare[0])		{
			udword	cursor = 1;
			while ((mText[pos + cursor] == bufToCompare[cursor]) && (cursor<lengthToCompare) && (pos+cursor<length))			cursor++;
			if (cursor == lengthToCompare)		return pos;
		}
		pos++;
	}
	return -1;
}

sdword String::ReverseFind(ubyte character)
{
	sdword	pos = GetLength() - 1;
	while ((pos != -1) && mText[pos] != character)	pos--;
	return pos;					
}


bool String::Extract(float& f, udword from)
{
	return sscanf((const char*) GetBuffer() + from, "%f", &f)!=0;
}

bool String::Extract(udword& i, udword from)
{
	return sscanf((const char*) GetBuffer() + from, "%d", &i)!=0;
}

bool String::Extract(sdword& i, udword from)
{
	return sscanf((const char*) GetBuffer() + from, "%d", &i)!=0;
}


sdword String::NextLine(sdword from)
{
	sdword	found = Find('\n', from);
	if (found == -1)		return -1;
	return found + 1;
}

sdword String::GetEndOfLine(sdword from)
{
	sdword	found = Find('\n', from);
	if (found == -1)		return -1;
	return found;
}






#ifdef TOFIX


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Concatenates the current String object with an existing String into a third String object (the first two remains unchanged).
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : an existing String object to concatenate the current String object to.
// Output   : None
// Return   : the resulting String.
// Comments : None
String String::operator+(String& str)
{
	String	result;

	result.SetLength(GetLength() + str.GetLength());		// + 1);
	if (result.mCharBuf)		{
		if (GetLength())			::CopyMemory(result.mCharBuf, mCharBuf, GetLength());
		if (str.GetLength())		::CopyMemory(result.mCharBuf + GetLength(), str.GetBuffer(), str.GetLength());

		result.mLength = GetLength() + str.GetLength();
		result.mCharBuf[result.mLength] = 0;
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Concatenates the current String object with an existing String into a third String object (the first two remains unchanged).
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : 
// Output   : None
// Return   : the resulting String.
// Comments : None
String String::operator+(char* str)
{
	String	result;

	result.SetLength(GetLength() + GetLength((ubyte*) str));
	if (result.mCharBuf)		{
		if (GetLength())				::CopyMemory(result.mCharBuf, mCharBuf, GetLength());
		if (str && GetLength((ubyte*) str))	::CopyMemory(result.mCharBuf + GetLength(), str, GetLength((ubyte*) str));

		result.mLength = GetLength() + GetLength((ubyte*) str);
		result.mCharBuf[result.mLength] = 0;
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Append a single character to an existing String object
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : a character to append the current String object to.
// Output   : None
// Return   : the resulting String.
// Comments : None
String String::operator+(char car)
{
	String	result;

	result.SetLength(GetLength() + 1);
	if (result.mCharBuf)		{
		if (GetLength())			::CopyMemory(result.mCharBuf, mCharBuf, GetLength());
		result.mCharBuf[GetLength()] = car;

		result.mLength = GetLength() + 1;
		result.mCharBuf[result.mLength] = 0;
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Concatenates the current String object with an existing String.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : an existing String object to concatenate the current String object to.
// Output   : None
// Return   : self-reference.
// Comments : None
String& String::operator+=(String& str)
{
	if (str.GetLength())		{
		udword	oldLength = GetLength();
		SetLength(oldLength + str.GetLength() + 1);
		::CopyMemory(mCharBuf + oldLength, str.GetBuffer(), str.GetLength() + 1);
	}

	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Concatenates the current String object with an existing String.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : an existing String object to concatenate the current String object to.
// Output   : None
// Return   : self-reference.
// Comments : None
String& String::operator+=(char* str)
{
	udword	oldLength = GetLength();
	udword	appendLength = GetLength((ubyte*) str);
	if (str && appendLength)		{
		SetLength(oldLength + appendLength + 1);
		::CopyMemory(mCharBuf + oldLength, str, appendLength + 1);
	}

	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Concatenates the current String object with a single character
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : a character to append to the current String object.
// Output   : None
// Return   : self-reference.
// Comments : None
String& String::operator+=(char car)
{
	udword	oldLength = GetLength();
	SetLength(oldLength + 1);
	mCharBuf[oldLength] = car;

	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy an existing String's content to the current String object
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : an existing String object to copy.
// Output   : None
// Return   : self-reference.
// Comments : None
String& String::operator=(String& str)
{
	Reset();
	Set(str.GetBuffer());

	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy an existing string buffer into the current String object
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : an existing String object to copy.
// Output   : None
// Return   : self-reference.
// Comments : None
String& String::operator=(char* str)
{
	Reset();
	Set(str);

	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compare the current String object and an existing String.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : an existing String object to compare the current String object with.
// Output   : None
// Return   : true if the two String are identical, false otherwise.
// Comments : None
bool String::operator==(String& str)
{
	return strcmp((const char*) mCharBuf, (const char*) str.mCharBuf) == 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compare the current String object and an existing char buffer.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : an existing char buffer to compare the current String object with.
// Output   : None
// Return   : true if the two String are identical, false otherwise.
// Comments : None
bool String::operator==(char* str)
{
	return strcmp((const char*) mCharBuf, (const char*) str) == 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compare the current String object and an existing String.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : an existing String object to compare the current String object with.
// Output   : None
// Return   : true if the two String are different, false otherwise.
// Comments : None
bool String::operator!=(String& str)
{
	return strcmp((const char*) mCharBuf, (const char*) str.mCharBuf) != 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compare the current String object and an existing char buffer.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : an existing char buffer to compare the current String object with.
// Output   : None
// Return   : true if the two String are different, false otherwise.
// Comments : None
bool String::operator!=(char* str)
{
	return strcmp((const char*) mCharBuf, (const char*) str) != 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set the length of the current String object : allocate a new text buffer of the desired length, and copy the old String buffer in it. Protected method!
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input    : the new length of the current String object.
// Output   : None
// Return   : the real length of the new String object
// Comments : Set KeepOldContent to false to optimize the treatment (no copy!)
udword String::SetLength(udword count, bool KeepOldContent)
{
	if (!count)	{ Reset();	return 0;	}

	ubyte*	oldBuf = mCharBuf;
	udword	oldLength = GetLength();

	mCharBuf = new ubyte[count + 1];
	::FillMemory(mCharBuf, count, 'F');
	mCharBuf[count] = '\0';

	if (oldBuf)		{
		if (KeepOldContent && oldLength)			::CopyMemory(mCharBuf, oldBuf, MIN(count, oldLength));
		delete []oldBuf;
	}

	mLength = -1;				//The length is temporary, it'll be evaluated by the GetLength() method

	return GetLength();
}



#endif