///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains FPU related code.
 *	\file		IceFPU.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceCore;

static uword gFPUCW_Save;

void IceCore::SaveFPU()
{
	_asm	fstcw	gFPUCW_Save
}

void IceCore::RestoreFPU()
{
	_asm	fldcw	gFPUCW_Save
}

void IceCore::SetFPUFloorMode()
{
	uword FPUCW_Save, FPUCW;
	_asm
	{
		push	eax
		fstcw	FPUCW_Save
		mov		ax, FPUCW_Save
		and		ax,01111000000111111b	; 32 bits precision
		or		ax,00C00h				; Chop
		mov		FPUCW,ax
		fldcw	FPUCW
		pop		eax
	}
}

void IceCore::SetFPUCeilMode()
{
	uword FPUCW_Save, FPUCW;
	_asm
	{
		push	eax
		fstcw	FPUCW_Save
		mov		ax, FPUCW_Save
		and		ax,01111000000111111b	; 32 bits precision
		or		ax,00800h				; Up
		mov		FPUCW,ax
		fldcw	FPUCW
		pop		eax
	}
}

void IceCore::SetFPUBestMode()
{
	uword FPUCW_Save, FPUCW;
	_asm
	{
		push	eax
		fstcw	FPUCW_Save
		mov		ax, FPUCW_Save
		and		ax,01111000000111111b	; 32 bits precision
		or		ax,00000h				; Near
		mov		FPUCW,ax
		fldcw	FPUCW
		pop		eax
	}
}

FPUMode	IceCore::GetFPUMode()
{
	// Test rounding mode
	float f1 = 1.1f;
	float f2 = 1.9f;
	udword d1, d2;
	_asm	fld		f1
	_asm	fistp	d1
	_asm	fld		f2
	_asm	fistp	d2
			if(d1==1 && d2==1)	return FPU_FLOOR;
	else	if(d1==2 && d2==2)	return FPU_CEIL;
	else	if(d1==1 && d2==2)	return FPU_BEST;
	else						return FPU_FORCE_DWORD;
}

void IceCore::SetFPUPrecision24()	{	_controlfp(_PC_24,		_MCW_PC);	}
void IceCore::SetFPUPrecision53()	{	_controlfp(_PC_53,		_MCW_PC);	}
void IceCore::SetFPUPrecision64()	{	_controlfp(_PC_64,		_MCW_PC);	}
void IceCore::SetFPURoundingChop()	{	_controlfp(_RC_CHOP,	_MCW_RC);	}
void IceCore::SetFPURoundingUp()	{	_controlfp(_RC_UP,		_MCW_RC);	}
void IceCore::SetFPURoundingDown()	{	_controlfp(_RC_DOWN,	_MCW_RC);	}
void IceCore::SetFPURoundingNear()	{	_controlfp(_RC_NEAR,	_MCW_RC);	}

int IceCore::intChop(const float& f)
{ 
	INT32 a			= *reinterpret_cast<const INT32*>(&f);			// take bit pattern of float into a register
	INT32 sign		= (a>>31);										// sign = 0xFFFFFFFF if original value is negative, 0 if positive
	INT32 mantissa	= (a&((1<<23)-1))|(1<<23);						// extract mantissa and add the hidden bit
	INT32 exponent	= ((a&0x7fffffff)>>23)-127;						// extract the exponent
	INT32 r			= ((UINT32)(mantissa)<<8)>>(31-exponent);		// ((1<<exponent)*mantissa)>>24 -- (we know that mantissa > (1<<24))
	return ((r ^ (sign)) - sign ) &~ (exponent>>31);				// add original sign. If exponent was negative, make return value 0.
}

int IceCore::intFloor(const float& f)
{ 
	INT32 a			= *reinterpret_cast<const INT32*>(&f);									// take bit pattern of float into a register
	INT32 sign		= (a>>31);																// sign = 0xFFFFFFFF if original value is negative, 0 if positive
	a&=0x7fffffff;																			// we don't need the sign any more
	INT32 exponent	= (a>>23)-127;															// extract the exponent
	INT32 expsign   = ~(exponent>>31);														// 0xFFFFFFFF if exponent is positive, 0 otherwise
	INT32 imask		= ( (1<<(31-(exponent))))-1;											// mask for true integer values
	INT32 mantissa	= (a&((1<<23)-1));														// extract mantissa (without the hidden bit)
	INT32 r			= ((UINT32)(mantissa|(1<<23))<<8)>>(31-exponent);						// ((1<<exponent)*(mantissa|hidden bit))>>24 -- (we know that mantissa > (1<<24))
	r = ((r & expsign) ^ (sign)) + ((!((mantissa<<8)&imask)&(expsign^((a-1)>>31)))&sign);	// if (fabs(value)<1.0) value = 0; copy sign; if (value < 0 && value==(int)(value)) value++; 
	return r;
}

int IceCore::intCeil(const float& f)
{ 
	INT32 a			= *reinterpret_cast<const INT32*>(&f) ^ 0x80000000;						// take bit pattern of float into a register
	INT32 sign		= (a>>31);																// sign = 0xFFFFFFFF if original value is negative, 0 if positive
	a&=0x7fffffff;																			// we don't need the sign any more
	INT32 exponent	= (a>>23)-127;															// extract the exponent
	INT32 expsign   = ~(exponent>>31);														// 0xFFFFFFFF if exponent is positive, 0 otherwise
	INT32 imask		= ( (1<<(31-(exponent))))-1;											// mask for true integer values
	INT32 mantissa	= (a&((1<<23)-1));														// extract mantissa (without the hidden bit)
	INT32 r			= ((UINT32)(mantissa|(1<<23))<<8)>>(31-exponent);						// ((1<<exponent)*(mantissa|hidden bit))>>24 -- (we know that mantissa > (1<<24))
	r = ((r & expsign) ^ (sign)) + ((!((mantissa<<8)&imask)&(expsign^((a-1)>>31)))&sign);	// if (fabs(value)<1.0) value = 0; copy sign; if (value < 0 && value==(int)(value)) value++; 
	return -r;
}


#ifdef OLD

// ------------------------------------------------------------------------------------------------------------------------
// Project Name: Coder Corner
// File Name: ASMDEFINE.H
// Author: Pierre Terdiman [Zappy]
// Date: 08/15/98
// Abstract: C definitions for the asm code to match my Tasm way of coding
// ------------------------------------------------------------------------------------------------------------------------

#ifndef __ASMDEFINE_H__
#define __ASMDEFINE_H__

#ifdef WIN32

#define OFST OFFSET
#define BPTR BYTE PTR
#define WPTR WORD PTR
#define DPTR DWORD PTR
#define QPTR QWORD PTR
#define FPTR FWORD PTR
#define o OFFSET
#define O OFFSET
#define b BYTE PTR
#define B BYTE PTR
#define w WORD PTR
#define W WORD PTR
#define d DWORD PTR
#define D DWORD PTR
#define f FWORD PTR
#define F FWORD PTR
#define q QWORD PTR
#define Q QWORD PTR
#define s short
#define S short

#endif // WIN32

#endif // __ASMDEFINE_H__



// ------------------------------------------------------------------------------------------------------------------------
// Project Name: Coder Corner
// File Name: FPU.C
// Author: Pierre Terdiman [Zappy]
// Date: 08/15/98
// Abstract: FPU Control
// ------------------------------------------------------------------------------------------------------------------------

#include "types.h"
#include "asmdefine.h"
#include "pragma.h"
#include "fpu.h"

static Float32 Deg2Rad=180.0f;

// ------------------------------------------------------------------------------------------------------------------------
// FPU rounding modes.
// ------------------------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------------------------

/*
;	mov	ax,[FPUCW_Save]
;            ;and     ax,01111110011111111b	;24 bits precision
;            ;and     ax,01111001111111111b
;	;and	ax,0F03Fh	; switch on
;	and	ax,01111000000111111b	; 32 bits precision
;	or	ax,00C00h	; Chop
;;	or	ax,00800h	; Up
;;	or	ax,00000h	; Near
;	mov	[FPUCW],ax
;	fldcw	[FPUCW]
*/

#endif