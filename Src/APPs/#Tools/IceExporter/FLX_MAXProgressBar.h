///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_MAXPROGRESSBAR_H
#define FLX_MAXPROGRESSBAR_H

	class MAXProgressBar
	{
		public:
		// Constructor/Destructor
							MAXProgressBar(Interface* max_interface);
							~MAXProgressBar();

				BOOL		StartProgressBar(const CHAR* title, udword tick_count, udword divider = 1);
				void		EndProgressBar()						const;
				BOOL		UpdateProgressBar(udword percentage)	const;
		inline_	BOOL		UpdateProgressBar()
							{
								BOOL MustContinue = TRUE;
								udword RunningPercentage = udword(mProgressBarValue);

								if(RunningPercentage!=mProgressBarPercentage)	// only update the GUI if needed
								{
									mProgressBarPercentage = RunningPercentage;
									MustContinue = UpdateProgressBar(mProgressBarPercentage * mProgressBarDivider);
								}

								mProgressBarValue += mProgressBarStep;

								return MustContinue;
							}
		private:
				Interface*	mMAXInterface;
				float		mProgressBarValue;
				float		mProgressBarStep;
				sdword		mProgressBarDivider;
				sdword		mProgressBarPercentage;
	};

#endif // FLX_MAXPROGRESSBAR_H
