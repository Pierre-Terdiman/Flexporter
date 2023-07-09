///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains a billboard manager.
 *	\file		IceBillboardManager.h
 *	\author		Pierre Terdiman
 *	\date		April, 14, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEBILLBOARDMANAGER_H
#define ICEBILLBOARDMANAGER_H

	class ICERENDERER_API Billboard
	{
		public:

		inline_					Billboard()																							{}
		inline_					Billboard(const Point& p, float size, float deadline) : mPos(p), mSize(size), mDeadLine(deadline)	{}
		inline_					~Billboard()																						{}

				Point			mPos;
				float			mSize;
				float			mDeadLine;
	};

	class ICERENDERER_API BillboardManager
	{
		public:
								BillboardManager();
								~BillboardManager();

		inline_	void			SetWorldMatrix(const Matrix4x4& world_matrix)	{ mWorld = world_matrix;	mIdentity = mWorld.IsIdentity();	}
				void			AddBillboard(const Billboard& object);
				bool			Render(Renderer* rd, float global_time);
		inline_	void			Reset()			{ mData.Reset();	}

		private:
				Matrix4x4		mWorld;
				BOOL			mIdentity;
				udword			mVertexOffset;
				VertexBuffer*	mVB;
		// 
				Container		mData;

		// Internal methods
				void			Release();
	};

#endif	// ICEBILLBOARDMANAGER_H
