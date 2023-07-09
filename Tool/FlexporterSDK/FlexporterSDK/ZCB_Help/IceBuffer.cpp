///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains misc buffer related code.
 *	\file		IceBuffer.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceCore;

bool IceCore::Delta(void* buffer, udword nb_items, udword item_size)
{
	switch(item_size)
	{
		case 1:
		{
			sbyte* Buf = (sbyte*)buffer;
			sbyte Previous = Buf[0];
			for(udword i=1;i<nb_items;i++)
			{
				sbyte Saved = Buf[i];
				Buf[i]-=Previous;
				Previous = Saved;
			}
			return true;
		}
		break;

		case 2:
		{
			sword* Buf = (sword*)buffer;
			sword Previous = Buf[0];
			for(udword i=1;i<nb_items;i++)
			{
				sword Saved = Buf[i];
				Buf[i]-=Previous;
				Previous = Saved;
			}
			return true;
		}
		break;

		case 4:
		{
			sdword* Buf = (sdword*)buffer;
			sdword Previous = Buf[0];
			for(udword i=1;i<nb_items;i++)
			{
				sdword Saved = Buf[i];
				Buf[i]-=Previous;
				Previous = Saved;
			}
			return true;
		}
		break;
	}
	return false;
}

bool IceCore::UnDelta(void* buffer, udword nb_items, udword item_size)
{
	switch(item_size)
	{
		case 1:
		{
			sbyte* Buf = (sbyte*)buffer;
			for(udword i=1;i<nb_items;i++)	Buf[i]+=Buf[i-1];
			return true;
		}
		break;

		case 2:
		{
			sword* Buf = (sword*)buffer;
			for(udword i=1;i<nb_items;i++)	Buf[i]+=Buf[i-1];
			return true;
		}
		break;

		case 4:
		{
			sdword* Buf = (sdword*)buffer;
			for(udword i=1;i<nb_items;i++)	Buf[i]+=Buf[i-1];
			return true;
		}
		break;
	}
	return false;
}



/*

  Input buffer is made of "nbentries" whose size is "stride".

*/
bool IceCore::DisruptBuffer(void* buffer, udword nb_entries, udword item_size, udword stride)
{
	// Create a local copy
	ubyte* Copy = new ubyte[nb_entries*stride];
	CHECKALLOC(Copy);
	CopyMemory(Copy, buffer, nb_entries*stride*sizeof(ubyte));

	ubyte* dest = (ubyte*)buffer;
	udword NbCol = stride / item_size;
	for(udword j=0;j<NbCol;j++)
	{
		ubyte* src = Copy;
		src += j*item_size;
		for(udword i=0;i<nb_entries;i++)
		{
			CopyMemory(dest, src, item_size*sizeof(ubyte));
			src += stride;
			dest += item_size;
		}
	}

	return true;
}

bool IceCore::SaveAsSource(const char* filename, const char* label, const void* buffer, udword length, udword original_size, CompressionCode code)
{
	// Save original length
	udword TrueLength = length;

	// Compute number of extra bytes
	udword NbExtraBytes = 0;
	while(length&3)
	{
		NbExtraBytes++;
		length++;
	}

	// Create a new buffer for safety (avoid reading trash memory)
	ubyte* TmpBuffer = new ubyte[length];
	CHECKALLOC(TmpBuffer);
	ZeroMemory(TmpBuffer, length);
	CopyMemory(TmpBuffer, buffer, TrueLength);

	// Compute number of dwords
	length>>=2;

	CustomArray Array;

	// Output namespace
//	Array.StoreASCII("namespace %s;\n\n", label);

	// Output compression code
	switch(code)
	{
		case PACK_NONE:		Array.StoreASCII("CompressionCode %s_CompressionMethod = PACK_NONE;\n\n", label);		break;
		case PACK_ZLIB:		Array.StoreASCII("CompressionCode %s_CompressionMethod = PACK_ZLIB;\n\n", label);		break;
		case PACK_BZIP2:	Array.StoreASCII("CompressionCode %s_CompressionMethod = PACK_BZIP2;\n\n", label);		break;
		default:			Array.StoreASCII("CompressionCode %s_CompressionMethod = PACK_UNKNOWN;\n\n", label);	break;
	}

	// Output original size
	Array.StoreASCII("udword %s_OriginalSize = %d;\n\n", label, original_size);

	// Output length
	Array.StoreASCII("udword %s_CompressedSize = %d;\n\n", label, TrueLength);

	// Output dwords
	Array.StoreASCII("udword %s_Data[]={\n", label);

	ubyte ConvTable[]= { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };

	ubyte* Buf = TmpBuffer;
	for(udword i=0;i<length;i++)
	{
		char Text[32];
		ubyte c3 = Buf[i*4+0];
		ubyte c2 = Buf[i*4+1];
		ubyte c1 = Buf[i*4+2];
		ubyte c0 = Buf[i*4+3];

		ubyte o=0;
		Text[o++] = ConvTable[c0>>4];
		Text[o++] = ConvTable[c0&15];

		Text[o++] = ConvTable[c1>>4];
		Text[o++] = ConvTable[c1&15];

		Text[o++] = ConvTable[c2>>4];
		Text[o++] = ConvTable[c2&15];

		Text[o++] = ConvTable[c3>>4];
		Text[o++] = ConvTable[c3&15];

		Text[o++] = 0;

		if(c0 || c1 || c2 || c3)	Array.StoreASCII("0x").StoreASCII(Text);
		else						Array.StoreASCII("0");

		if(i!=length-1)				Array.StoreASCII(", ");
		if(!(i&15))					Array.StoreASCII("\n");
	}

	Array.StoreASCII("\n};");
	Array.ExportToDisk(filename);

	DELETEARRAY(TmpBuffer);
	return true;
}

udword IceCore::FindRank(udword* sorted_list, udword list_size, udword value)
{
	// Checkings
	if(!sorted_list)	return INVALID_ID;

	udword Min = 0;
	udword Max = list_size-1;

	if(sorted_list[Min]>value)	return INVALID_ID;
	if(sorted_list[Max]<value)	return INVALID_ID;

	while((Max-Min)>1)
	{
		udword Mid = (Max + Min)>>1;

				if(value==sorted_list[Mid])	return Mid;
		else	if(value<sorted_list[Mid])	Max = Mid;
		else								Min = Mid;
	}

	if(value==sorted_list[Min])	return Min;
	if(value==sorted_list[Max])	return Max;
	return INVALID_ID;
}

udword IceCore::SortAndReduce(udword& nb, udword* list, udword* dest, Container* cnt)
{
	// Checkings
	if(!nb || !list) return 0;

	// We have a list of N dwords in input, in arbitrary order. The goal is to get rid of
	// redundant ones and provide a minimal list of sorted dwords. (i.e. a reduced list)

	// 1) So we first create a radix sort and use it
	RadixSort RS;
	const udword* Sorted = RS.Sort(list, nb, RADIX_UNSIGNED).GetRanks();

	// 2) Now we must reduce the list
	// 2-1) Initialize with anything but the first sorted dword
	udword PreviousData = list[Sorted[0]] + 1;
	udword RunningIndex = 0;

	Container* Reduced = null;
	if(!dest && !cnt)	Reduced = new Container;

	// 2-2) Loop through dwords
	for(udword i=0;i<nb;i++)
	{
		// Get sorted index
		udword SortedIndex = Sorted[i];
		// Get sorted dword
		udword SortedData = list[SortedIndex];
		// Discard redundant ones
		if(SortedData!=PreviousData)
		{
			if(dest)	dest[RunningIndex++] = SortedData;
			if(cnt)		cnt->Add(SortedData);
			if(Reduced)	Reduced->Add(SortedData);
			PreviousData = SortedData;
		}
	}

	// 3) Replace old list if needed
	if(dest)	return RunningIndex;
	if(cnt)		return cnt->GetNbEntries();

	nb = Reduced->GetNbEntries();
	CopyMemory(list, Reduced->GetEntries(), nb*sizeof(udword));
	DELETESINGLE(Reduced);
	return nb;
}
