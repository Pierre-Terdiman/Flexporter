///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains mipmap-related code.
 *	\file		IceMipmap.h
 *	\author		Pierre Terdiman
 *	\date		January, 17, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEMIPMAP_H
#define ICEMIPMAP_H

	//! Mipmap generator
	class ICERENDERER_API MipmapGenerator
	{
		public:
		// Constructor / Destructor
							MipmapGenerator();
		virtual				~MipmapGenerator();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Mipmap generation method
		 *	\param		pic		[in/out] picture to reduce
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual		bool	CreateMipmap(Picture& pic)	const;
	};

#endif // ICEMIPMAP_H
