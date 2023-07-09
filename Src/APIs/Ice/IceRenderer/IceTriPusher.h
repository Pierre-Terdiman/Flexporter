///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains triangle pushers.
 *	\file		IceTriPusher.h
 *	\author		Pierre Terdiman
 *	\date		January, 17, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICETRIPUSHER_H
#define ICETRIPUSHER_H

	class ICERENDERER_API TriPusher
	{
		public:
		// Constructor
								TriPusher();
		// Destructor
								~TriPusher();
		// Lifetime
				bool			Init(Renderer* rd, udword fvf, udword nbfaces=5000, udword batchsize=5000);
				bool			Release();
		// Standard use
				bool			Push(Renderer* rd, udword nbfaces, ubyte* data);
		// Batch processing
				bool			Batch(Renderer* rd, udword nbverts, ubyte* data);
				bool			FlushBatch(Renderer* rd);
		private:
				VertexBuffer*	mVB;
				udword			mVertexOffset;

			// Batch
				udword			mBatchSize;
				udword			mBatchIndex;
				ubyte*			mBatchBuffer;
	};

#endif // ICETRIPUSHER_H
