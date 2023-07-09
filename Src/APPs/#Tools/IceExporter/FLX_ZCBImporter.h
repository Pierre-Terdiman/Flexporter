///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_ZCBIMPORTER_H
#define FLX_ZCBIMPORTER_H

/*
class SceneImport {
public:
							SceneImport() {};
	virtual					~SceneImport() {};
	virtual int				ExtCount() = 0;					// Number of extemsions supported
	virtual const TCHAR *	Ext(int n) = 0;					// Extension #n (i.e. "3DS")
	virtual const TCHAR *	LongDesc() = 0;					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	virtual const TCHAR *	ShortDesc() = 0;				// Short ASCII description (i.e. "3D Studio")
	virtual const TCHAR *	AuthorName() = 0;				// ASCII Author name
	virtual const TCHAR *	CopyrightMessage() = 0;			// ASCII Copyright message
	virtual const TCHAR *	OtherMessage1() = 0;			// Other message #1
	virtual const TCHAR *	OtherMessage2() = 0;			// Other message #2
	virtual unsigned int	Version() = 0;					// Version number * 100 (i.e. v3.01 = 301)
	virtual void			ShowAbout(HWND hWnd) = 0;		// Show DLL's "About..." box
	virtual int				DoImport(const TCHAR *name,ImpInterface *ii,Interface *i, BOOL suppressPrompts=FALSE) = 0;	// Import file
	virtual int				ZoomExtents() { return ZOOMEXT_NOT_IMPLEMENTED; }	// Override this for zoom extents control
	};
*/

	class ZCBClassDesc: public ClassDesc
	{
		public:
			int 			IsPublic();
			void *			Create(BOOL loading = FALSE);
			const TCHAR *	ClassName();
			SClass_ID		SuperClassID();
			Class_ID		ClassID();
			const TCHAR* 	Category();
	};

	ZCBClassDesc* GetZCBDesc();

	class ZCBImport : public SceneImport
	{
		public:
								ZCBImport();
		virtual					~ZCBImport();

		override(SceneImport)	int				ExtCount();					// Number of extensions supported
		override(SceneImport)	const TCHAR*	Ext(int n);					// Extension #n (i.e. "3DS")
		override(SceneImport)	const TCHAR*	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		override(SceneImport)	const TCHAR*	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		override(SceneImport)	const TCHAR*	AuthorName();				// ASCII Author name
		override(SceneImport)	const TCHAR*	CopyrightMessage();			// ASCII Copyright message
		override(SceneImport)	const TCHAR*	OtherMessage1();			// Other message #1
		override(SceneImport)	const TCHAR*	OtherMessage2();			// Other message #2
		override(SceneImport)	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		override(SceneImport)	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
		override(SceneImport)	int				DoImport(const TCHAR* name, ImpInterface* i,Interface* gi, BOOL suppressPrompts=FALSE);	// Import file
	};

#endif // FLX_ZCBIMPORTER_H
