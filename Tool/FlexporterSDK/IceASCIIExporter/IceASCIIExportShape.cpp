///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Shape export method.
 *	This method is called once for each exported shape.
 *	\param		shape		[in] a structure filled with current shape information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ASCIIFormat::ExportShape(const ShapeDescriptor& shape)
{
	// Export common information
	mShapes.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("Shape:       ");
	ExportBasicInfo(&shape, &mShapes);

	// Export shape parameters

	// Type
	mShapes.StoreASCII("Type: %d\n", shape.mType);

	// Number of lines
	mShapes.StoreASCII("Number of lines: %d\n", shape.mNbLines);

	// Material ID
	mShapes.StoreASCII("Material ID: ").StoreASCII(shape.mMatID).StoreASCII("\n");

	// Export all lines
	udword p=0;
	for(udword i=0;i<shape.mNbLines;i++)
	{
		udword NbVerts = shape.mNbVerts[i];
		bool Closed = shape.mClosed[i];
		mShapes.StoreASCII("Line ").StoreASCII(i).StoreASCII(": (").StoreASCII(NbVerts).StoreASCII(" vertices)").StoreASCII("\n");
		if(Closed)	mShapes.StoreASCII("(closed curve)\n");
		else		mShapes.StoreASCII("(open curve)\n");
		for(udword j=0;j<NbVerts;j++)
		{
			mShapes.StoreASCII(shape.mVerts[p].x).StoreASCII(" ");
			mShapes.StoreASCII(shape.mVerts[p].y).StoreASCII(" ");
			mShapes.StoreASCII(shape.mVerts[p].z).StoreASCII(" ");
			if((p&63)==63)	mShapes.StoreASCII("\n");
			p++;
		}
		mShapes.StoreASCII("\n");
	}

	return true;
}
