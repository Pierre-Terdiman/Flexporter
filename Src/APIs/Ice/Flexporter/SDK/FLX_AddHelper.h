///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the helper-related exported structures.
 *	\file		FLX_AddHelper.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_ADDHELPER_H
#define FLX_ADDHELPER_H

	//! Supported helper types from MAX.
	//! NB: don't reorganize this list or it will break compatibility with some old files!
	enum HType
	{
		FLX_HTYPE_DUMMY,			//!< Standard helper: dummy object
		FLX_HTYPE_GIZMO_BOX,		//!< Atmospheric apparatus: box gizmo
		FLX_HTYPE_GIZMO_CYLINDER,	//!< Atmospheric apparatus: cylinder gizmo
		FLX_HTYPE_GIZMO_SPHERE,		//!< Atmospheric apparatus: sphere gizmo
		FLX_HTYPE_TAPE,				//!< Standard helper: tape
		FLX_HTYPE_GRID,				//!< Standard helper: grid
		FLX_HTYPE_POINT,			//!< Standard helper: point
		FLX_HTYPE_PROTRACTOR,		//!< Standard helper: protactor
		FLX_HTYPE_CROWD,			//!< Standard helper: crowd
		FLX_HTYPE_COMPASS,			//!< Standard helper: compass
		FLX_HTYPE_DELEGATE,			//!< Standard helper: delegate
		FLX_HTYPE_BILLBOARD,		//!< VRML helper: billboard

		FLX_HTYPE_UNDEFINED			= 0xffffffff
	};

	//! The helper information class. A dummy object in MAX may be used to group objects together.
	//! In such a situation the actual dummy object is invisible in the scene, and all the grouped objects are children of the dummy.
	class FLEXPORTER_API HelperDescriptor : public ObjectDescriptor
	{
		public:
		// Constructor / Destructor
									HelperDescriptor();
		virtual						~HelperDescriptor();

				HType				mHelperType;		//!< Type of helper
/*		// Gizmo information
				float				mLength;			//!< For BoxGizmo
				float				mWidth;				//!< For BoxGizmo
				float				mHeight;			//!< For BoxGizmo/CylinderGizmo
				float				mRadius;			//!< For CylinderGizmo/SphereGizmo
				bool				mHemi;				//!< For SphereGizmo*/
		// Group information
				bool				mIsGroupHead;		//!< Is the dummy a group head?
				Container*			mGroupedObjects;	//!< IDs of grouped objects (null if mIsGroupHead==false) [Flexporter 1.15]
		// Instance information
				HelperDescriptor*	mMasterHelper;		//!< Master object [Flexporter 1.15]
	};

	class FLEXPORTER_API BoxGizmoDescriptor : public HelperDescriptor
	{
		public:
		// Constructor / Destructor
									BoxGizmoDescriptor();
		virtual						~BoxGizmoDescriptor();

		// Gizmo information
				float				mLength;
				float				mWidth;
				float				mHeight;
	};

	class FLEXPORTER_API SphereGizmoDescriptor : public HelperDescriptor
	{
		public:
		// Constructor / Destructor
									SphereGizmoDescriptor();
		virtual						~SphereGizmoDescriptor();

		// Gizmo information
				float				mRadius;
				bool				mHemi;
	};

	class FLEXPORTER_API CylinderGizmoDescriptor : public HelperDescriptor
	{
		public:
		// Constructor / Destructor
									CylinderGizmoDescriptor();
		virtual						~CylinderGizmoDescriptor();

		// Gizmo information
				float				mHeight;
				float				mRadius;
	};

	class FLEXPORTER_API BillboardDescriptor : public HelperDescriptor
	{
		public:
		// Constructor / Destructor
									BillboardDescriptor();
		virtual						~BillboardDescriptor();

		// Billboard information
				float				mSize;
				float				mLength;
				bool				mScreenAlign;
	};

#endif // FLX_ADDHELPER_H
