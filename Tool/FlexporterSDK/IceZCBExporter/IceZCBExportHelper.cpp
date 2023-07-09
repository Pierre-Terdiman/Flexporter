///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Helper export method.
 *	This method is called once for each exported helper.
 *	\param		helper		[in] a structure filled with current helper information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBFormat::ExportHelper(const HelperDescriptor& helper)
{
	// 1) Export common information
	ExportBasicInfo(&helper, &mHelpers);

	// 2) Export helper parameters
	// 2-1) Export helper's type
	mHelpers.Store(udword(helper.mHelperType));
	// 2-2) Export group data
	mHelpers.Store(helper.mIsGroupHead);
	// CHUNK_HELP_VER >= 3
	if(helper.mIsGroupHead)
	{
		// Dump grouped objects' IDs
		udword NbGroupedObjects = helper.mGroupedObjects ? helper.mGroupedObjects->GetNbEntries() : 0;
		mHelpers.Store(NbGroupedObjects);
		for(udword i=0;i<NbGroupedObjects;i++)	mHelpers.Store(helper.mGroupedObjects->GetEntry(i));
	}

	// 2-3) Export gizmo data
	switch(helper.mHelperType)
	{
		case FLX_HTYPE_GIZMO_BOX:
		{
			const BoxGizmoDescriptor* Desc = (const BoxGizmoDescriptor*)&helper;
			mHelpers
				.Store(Desc->mLength)
				.Store(Desc->mWidth)
				.Store(Desc->mHeight);
		}
		break;

		case FLX_HTYPE_GIZMO_SPHERE:
		{
			const SphereGizmoDescriptor* Desc = (const SphereGizmoDescriptor*)&helper;
			mHelpers
				.Store(Desc->mRadius)
				.Store(Desc->mHemi);
		}
		break;

		case FLX_HTYPE_GIZMO_CYLINDER:
		{
			const CylinderGizmoDescriptor* Desc = (const CylinderGizmoDescriptor*)&helper;
			mHelpers
				.Store(Desc->mRadius)
				.Store(Desc->mHeight);
		}
		break;

		case FLX_HTYPE_BILLBOARD:
		{
			const BillboardDescriptor* Desc = (const BillboardDescriptor*)&helper;
			mHelpers
				.Store(Desc->mSize)
				.Store(Desc->mLength)
				.Store(Desc->mScreenAlign);
		}
		break;
	}

	return true;
}
