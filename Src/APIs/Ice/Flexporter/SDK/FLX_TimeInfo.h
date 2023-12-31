///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains time-related structures.
 *	\file		FLX_TimeInfo.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_TIMEINFO_H
#define FLX_TIMEINFO_H

	//! Active time segment.
	struct FLEXPORTER_API MAXTimeInfo
	{
		long	mStartTime;		//!< First frame = mStartTime/mDeltaTime
		long	mEndTime;		//!< Last frame = mEndTime/mDeltaTime
		long	mDeltaTime;		//!< Ticks per frame
		long	mFrameRate;		//!< Frame rate
		long	mCurrentTime;	//!< Static frame number
		long	mNbFrames;		//!< Number of frames according to sampling rate
	};

#endif // FLX_TIMEINFO_H
