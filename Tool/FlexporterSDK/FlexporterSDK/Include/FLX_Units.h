///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2003 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains units-related structures.
 *	\file		FLX_Units.h
 *	\author		Pierre Terdiman
 *	\date		January, 2, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __FLX_UNITS_H__
#define __FLX_UNITS_H__

	enum UnitType
	{
		FLX_UNIT_INCHES,
		FLX_UNIT_FEET,
		FLX_UNIT_MILES,
		FLX_UNIT_MILLIMETERS,
		FLX_UNIT_CENTIMETERS,
		FLX_UNIT_METERS,
		FLX_UNIT_KILOMETERS,

		FLX_UNIT_FORCE_DWORD = 0x7fffffff
	};

	enum DisplayType
	{
		FLX_UNITDISPLAY_GENERIC,
		FLX_UNITDISPLAY_METRIC,
		FLX_UNITDISPLAY_US,
		FLX_UNITDISPLAY_CUSTOM,

		FLX_UNITDISPLAY_FORCE_DWORD = 0x7fffffff
	};

	struct FLEXPORTER_API MAXDispInfo
	{
		DisplayType	mDispType;					//!< Display type
		int			mMetricDisp;				//!< Metric display option
		int			mUSDisp;					//!< US display option
		int			mUSFrac;					//!< US fraction option
		String		mCustomName;				//!< Custom unit name
		float		mCustomValue;				//!< Custom unit value
		int			mCustomUnit;				//!< Custom unit reference
	};

	//! Units data at export time
	struct FLEXPORTER_API MAXUnits
	{
		// Master scales
		double		mMasterScaleInches;			//!< The master scale in terms of the specified unit type (inches)
		double		mMasterScaleFeet;			//!< The master scale in terms of the specified unit type (feet)
		double		mMasterScaleMiles;			//!< The master scale in terms of the specified unit type (miles)
		double		mMasterScaleMillimeters;	//!< The master scale in terms of the specified unit type (millimeters)
		double		mMasterScaleCentimeters;	//!< The master scale in terms of the specified unit type (centimeters)
		double		mMasterScaleMeters;			//!< The master scale in terms of the specified unit type (meters)
		double		mMasterScaleKilometers;		//!< The master scale in terms of the specified unit type (kilometers)
		// Master unit settings in effect
		UnitType	mType;						//!< Unit type in effect
		float		mScale;						//!< The master scale setting. This is the value the user entered in the
												//!< 1 Unit = XXX field of the General Page in the Preference Settings dialog box.
		// Current unit display information
		MAXDispInfo	mDispInfo;
		// Current unit display type
		DisplayType	mDType;						//!< Might be redundant with mDispInfo [I'm not sure but better safe than sorry]
	};

#endif // __FLX_UNITS_H__
