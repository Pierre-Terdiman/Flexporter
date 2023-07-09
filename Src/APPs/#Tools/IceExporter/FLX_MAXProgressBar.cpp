///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXProgressBar::MAXProgressBar(Interface* max_interface) :
	mProgressBarValue		(0.0f),
	mProgressBarStep		(0.0f),
	mProgressBarDivider		(1),
	mProgressBarPercentage	(0)
{
	mMAXInterface = max_interface;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXProgressBar::~MAXProgressBar()
{
	EndProgressBar();
}

DWORD WINAPI fn(LPVOID arg)
{
	return TRUE;
}

BOOL MAXProgressBar::StartProgressBar(const CHAR * title, udword tick_count, udword divider)
{
	if(!mMAXInterface)	return FALSE;

	// Initialize internal data
	mProgressBarValue		= 0.0f;
	mProgressBarStep		= 100.0f / ( float(divider) * float(tick_count));
	mProgressBarPercentage	= -1;
	mProgressBarDivider		= divider;

	// This method puts up a progress bar in the status panel. The function fn is then called and is passed the argument arg.
	// This function fn does the processing work of the plug-in.
	return mMAXInterface->ProgressStart((TCHAR*)title, TRUE, fn, 0);
}

void MAXProgressBar::EndProgressBar() const
{
	if(mMAXInterface)	mMAXInterface->ProgressEnd();
}

BOOL MAXProgressBar::UpdateProgressBar(udword percentage) const
{
	if(mMAXInterface)
	{
		mMAXInterface->ProgressUpdate(percentage);
		return !mMAXInterface->GetCancel();
	}
	return TRUE;
}
