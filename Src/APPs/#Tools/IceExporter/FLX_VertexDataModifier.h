///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the Vertex Data Modifier.
 *	\file		FLX_VertexDataModifier.h
 *	\author		Pierre Terdiman
 *	\date		April, 25, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_VERTEXDATAMODIFIER_H
#define FLX_VERTEXDATAMODIFIER_H

	#define LOCAL_VDATA_SPECULAR_X_CHANNEL			VDATA_USER + 0
	#define LOCAL_VDATA_SPECULAR_Y_CHANNEL			VDATA_USER + 1
	#define LOCAL_VDATA_SPECULAR_Z_CHANNEL			VDATA_USER + 2
	#define LOCAL_VDATA_LIGHT_DIRECTION_X_CHANNEL	VDATA_USER + 3
	#define LOCAL_VDATA_LIGHT_DIRECTION_Y_CHANNEL	VDATA_USER + 4
	#define LOCAL_VDATA_LIGHT_DIRECTION_Z_CHANNEL	VDATA_USER + 5

	#define VERTEX_DATA_MODIFIER_CLASS_ID	Class_ID(0x6446991, 0x552c5c4b)
	#define VERTEX_DATA_MODIFIER_VERSION	0x0001

	class VertexDataModifierDesc : public ClassDesc
	{
		public:
															VertexDataModifierDesc()	{}
		virtual												~VertexDataModifierDesc()	{}

		override(ClassDesc)			int						IsPublic()					{ return 0;								}
		override(ClassDesc)			const TCHAR*			ClassName()					{ return _T("Vertex Data Modifier");	}	// plug_in's name in MAX
		override(ClassDesc)			SClass_ID				SuperClassID()				{ return OSM_CLASS_ID;					}
		override(ClassDesc)			Class_ID				ClassID()					{ return VERTEX_DATA_MODIFIER_CLASS_ID;	}
		override(ClassDesc)			const TCHAR*			Category()					{ return _T("Flexporter");				}
		override(ClassDesc)			void*					Create(BOOL loading=FALSE);
	};

	VertexDataModifierDesc* GetVertexDataModifierDesc();

	class VertexDataModifier : public Modifier
	{
		public:
															VertexDataModifier();
		virtual												~VertexDataModifier();

		override(Animatable)		void					DeleteThis();
		override(Animatable)		void					GetClassName(TSTR& s);
		override(Animatable)		Class_ID				ClassID();
		override(BaseObject)		TCHAR*					GetObjectName();
		override(BaseObject)		CreateMouseCallBack*	GetCreateMouseCallBack();
		override(Animatable)		BOOL					CanCopyAnim();
		override(Modifier)			ChannelMask				ChannelsUsed();
		override(Modifier)			ChannelMask				ChannelsChanged();
		override(Modifier)			BOOL					DependOnTopology(ModContext& mc);
		override(Modifier)			Class_ID				InputType();
		override(ReferenceMaker)	int						NumRefs();
		override(ReferenceMaker)	RefTargetHandle			GetReference(int i);
		override(ReferenceMaker)	void					SetReference(int i, RefTargetHandle rtarg);
		override(Animatable)		int						NumSubs();
		override(Animatable)		Animatable*				SubAnim(int i);
		override(Animatable)		TSTR					SubAnimName(int i);
		override(Animatable)		void					BeginEditParams(IObjParam* ip, ULONG flags, Animatable* prev);
		override(Animatable)		void					EndEditParams(IObjParam* ip, ULONG flags, Animatable* next);
		override(Modifier)			void					ModifyObject(TimeValue t, ModContext& mc, ObjectState* os, INode* node);
		override(Modifier)			Interval				LocalValidity(TimeValue t);
		override(Modifier)			IOResult				Load(ILoad* iload);
		override(Modifier)			IOResult				Save(ISave* isave);
		override(ReferenceMaker)	RefResult				NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message);

									bool					SetVertexColors(IndexedSurface* color_surface, bool parity);
		private:
		// Vertex colors
									udword					mNbVerts;
									udword					mNbFaces;
									Point*					mVerts;
									IndexedTriangle*		mFaces;
		// Internal methods
									bool					ApplyVertexColors(Object* node_object);
									void					ReleaseData();
	};

#endif // FLX_VERTEXDATAMODIFIER_H
