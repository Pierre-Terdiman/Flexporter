///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains clipping code
 *	\file		IceClipping.h
 *	\author		Pierre Terdiman
 *	\date		June, 21, 1999
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICECLIPPING_H
#define ICECLIPPING_H

	struct ICERENDERER_API ClipBox
	{
		float	mXMin;
		float	mYMin;
		float	mXMax;
		float	mYMax;
	};

	ICERENDERER_API bool ClipQuad(PRT_Vertex* quad, const ClipBox& clip_box);
	ICERENDERER_API bool ClipQuad(PRD_Vertex* quad, const ClipBox& clip_box);

#endif // ICECLIPPING_H