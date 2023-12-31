///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains transform code for a software pipeline.
 *	\file		IceXForm.h
 *	\author		Pierre Terdiman
 *	\date		August, 15, 1998
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEXFORM_H
#define ICEXFORM_H

	class ICERENDERER_API XForm
	{
		public:
		// Constructor/Destructor
							XForm();
							~XForm();

				bool		Transform(udword nb_verts, const Point* verts, HPoint* xformed, udword stride, bool project);
		// Settings
				void		SetWorldMatrix(const Matrix4x4& world)	{ mWorld		= world;	}
				void		SetViewMatrix(const Matrix4x4& view)	{ mView			= view;		}
				void		SetProjMatrix(const Matrix4x4& proj)	{ mProj			= proj;		}
				void		SetRenderWidth(udword width)			{ mRenderWidth	= width;	}
				void		SetRenderHeight(udword height)			{ mRenderHeight	= height;	}
		private:
		// State machine
				Matrix4x4	mWorld;
				Matrix4x4	mView;
				Matrix4x4	mProj;
				udword		mRenderWidth;
				udword		mRenderHeight;
	};

#endif // ICEXFORM_H
