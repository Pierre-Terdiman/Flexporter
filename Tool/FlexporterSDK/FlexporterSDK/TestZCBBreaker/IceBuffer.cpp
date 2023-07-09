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

bool IceCore::Delta(void* buffer, udword nbitems, udword itemsize)
{
	switch(itemsize)
	{
		case 1:
		{
			sbyte* Buf = (sbyte*)buffer;
			sbyte Previous = Buf[0];
			for(udword i=1;i<nbitems;i++)
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
			for(udword i=1;i<nbitems;i++)
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
			for(udword i=1;i<nbitems;i++)
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

bool IceCore::UnDelta(void* buffer, udword nbitems, udword itemsize)
{
	switch(itemsize)
	{
		case 1:
		{
			sbyte* Buf = (sbyte*)buffer;
			for(udword i=1;i<nbitems;i++)	Buf[i]+=Buf[i-1];
			return true;
		}
		break;

		case 2:
		{
			sword* Buf = (sword*)buffer;
			for(udword i=1;i<nbitems;i++)	Buf[i]+=Buf[i-1];
			return true;
		}
		break;

		case 4:
		{
			sdword* Buf = (sdword*)buffer;
			for(udword i=1;i<nbitems;i++)	Buf[i]+=Buf[i-1];
			return true;
		}
		break;
	}
	return false;
}
