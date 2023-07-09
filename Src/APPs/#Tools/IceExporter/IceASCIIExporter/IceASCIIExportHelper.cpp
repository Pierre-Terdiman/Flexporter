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
bool ASCIIFormat::ExportHelper(const HelperDescriptor& helper)
{
	// 1) Export common information
	mHelpers.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("Helper:       ");
	ExportBasicInfo(&helper, &mHelpers);

	// 2) Export helper parameters
	// 2-1) Export helper's type
	mHelpers.StoreASCII("Helper type: ");
			if(helper.mHelperType==FLX_HTYPE_DUMMY)				mHelpers.StoreASCII("dummy object\n");
	else	if(helper.mHelperType==FLX_HTYPE_GIZMO_BOX)			mHelpers.StoreASCII("box gizmo\n");
	else	if(helper.mHelperType==FLX_HTYPE_GIZMO_SPHERE)		mHelpers.StoreASCII("sphere gizmo\n");
	else	if(helper.mHelperType==FLX_HTYPE_GIZMO_CYLINDER)	mHelpers.StoreASCII("cylinder gizmo\n");
	else	if(helper.mHelperType==FLX_HTYPE_BILLBOARD)			mHelpers.StoreASCII("billboard\n");
	else	if(helper.mHelperType==FLX_HTYPE_TAPE)				mHelpers.StoreASCII("tape helper\n");
	else	if(helper.mHelperType==FLX_HTYPE_GRID)				mHelpers.StoreASCII("grid helper\n");
	else	if(helper.mHelperType==FLX_HTYPE_POINT)				mHelpers.StoreASCII("point helper\n");
	else	if(helper.mHelperType==FLX_HTYPE_PROTRACTOR)		mHelpers.StoreASCII("protractor helper\n");
	else	if(helper.mHelperType==FLX_HTYPE_CROWD)				mHelpers.StoreASCII("crowd helper\n");
	else	if(helper.mHelperType==FLX_HTYPE_COMPASS)			mHelpers.StoreASCII("compass helper\n");
	else	if(helper.mHelperType==FLX_HTYPE_DELEGATE)			mHelpers.StoreASCII("delegate helper\n");
	else	if(helper.mHelperType==FLX_HTYPE_UNDEFINED)			mHelpers.StoreASCII("undefined helper\n");

	// 2-2) Export group data
	mHelpers.StoreASCII("Group head:  ").StoreASCII(helper.mIsGroupHead).StoreASCII("\n");
	if(helper.mIsGroupHead && helper.mGroupedObjects)
	{
		udword NbGroupedObjects = helper.mGroupedObjects->GetNbEntries();
		mHelpers.StoreASCII("Grouped objects' IDs:  ");
		for(udword i=0;i<NbGroupedObjects;i++)
		{
			mHelpers.StoreASCII("%d  ", helper.mGroupedObjects->GetEntry(i));
			if((i&63)==63)	mHelpers.StoreASCII("\n");
		}
	}

	// 2-3) Export helper data
	switch(helper.mHelperType)
	{
		case FLX_HTYPE_GIZMO_BOX:
		{
			const BoxGizmoDescriptor& Desc = (const BoxGizmoDescriptor&)helper;
			mHelpers
				.StoreASCII("Length:  ").StoreASCII(Desc.mLength).StoreASCII("\n")
				.StoreASCII("Width:   ").StoreASCII(Desc.mWidth).StoreASCII("\n")
				.StoreASCII("Height:  ").StoreASCII(Desc.mHeight).StoreASCII("\n");
		}
		break;

		case FLX_HTYPE_GIZMO_SPHERE:
		{
			const SphereGizmoDescriptor& Desc = (const SphereGizmoDescriptor&)helper;
			mHelpers
				.StoreASCII("Radius:  ").StoreASCII(Desc.mRadius).StoreASCII("\n")
				.StoreASCII("Hemi:    ").StoreASCII(Desc.mHemi).StoreASCII("\n");
		}
		break;

		case FLX_HTYPE_GIZMO_CYLINDER:
		{
			const CylinderGizmoDescriptor& Desc = (const CylinderGizmoDescriptor&)helper;
			mHelpers
				.StoreASCII("Radius:  ").StoreASCII(Desc.mRadius).StoreASCII("\n")
				.StoreASCII("Height:  ").StoreASCII(Desc.mHeight).StoreASCII("\n");
		}
		break;

		case FLX_HTYPE_BILLBOARD:
		{
			const BillboardDescriptor& Desc = (const BillboardDescriptor&)helper;
			mHelpers
				.StoreASCII("Size:        ").StoreASCII(Desc.mSize).StoreASCII("\n")
				.StoreASCII("Length:      ").StoreASCII(Desc.mLength).StoreASCII("\n")
				.StoreASCII("ScreenAlign: ").StoreASCII(Desc.mScreenAlign).StoreASCII("\n");
		}
		break;
	}
	return true;
}
