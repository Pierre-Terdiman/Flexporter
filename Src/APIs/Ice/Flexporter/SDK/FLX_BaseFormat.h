///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the base class for all export formats.
 *	\file		FLX_BaseFormat.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_BASEFORMAT_H
#define FLX_BASEFORMAT_H

	class FLEXPORTER_API CustomFormat
	{
		public:
		// Constructor/Destructor
								CustomFormat()
								{
									hInstance	= null;
									hWnd		= null;
								}
		virtual					~CustomFormat()	{}

		// Frame conversion
		// These methods are called each time a frame conversion (e.g. MAX-to-D3D) is needed.
		// (Called -after- the built-in MAX-to-D3D conversion)
		virtual	void			ConvertPoint(float& x, float& y, float& z)			const	{				}
		virtual	void			ConvertQuat(float& x, float& y, float& z, float& w)	const	{				}
		virtual	void			Parity(bool& parity)								const	{				}

		// GUI overrides
		// WARNING: if you return false, Flexporter built-in versions are discarded
		virtual	bool			InitGUI()													{ return true;	}
		virtual	bool			SetStatus(const char* text)									{ return true;	}
		virtual	bool			SetExtStatus(const char* text)								{ return true;	}
		virtual	bool			Progress(int pos)											{ return true;	}
		virtual	bool			DisplayLogText(const char* text)							{ return true;	}
		virtual	bool			CloseGUI()													{ return true;	}

		// About box
		virtual	void			About()														{				}

		// Fileselect overrides
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Fileselect override
		 *	\return		true if CustomFormat::FileSelect() must be used instead of built-in version
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual	bool			UseCustomFileselect()								const	{ return false;	}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Filename override
		 *	\return		possible suggested filename
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual	const char*		SuggestedFilename()									const	{ return null;	}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Displays a standard file-select dialog.
		 *	\param		extension		[in] file extension
		 *	\param		destname		[in] default destination name
		 *	\param		alias			[in] alias string
		 *	\param		mode			[in] false (LOAD), true (SAVE)
		 *	\param		filename		[out] user-returned filename
		 *	\return		true if selected file must be processed, else false to abort
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual	bool			FileSelect(const char* extension, const char* destname, const char* alias, bool mode, String& filename){ return false; }

		// Flexporter will setup those ones
				HINSTANCE		hInstance;
				HWND			hWnd;
	};

	//! Base export format
	class FLEXPORTER_API ExportFormat : public CustomFormat
	{
		public:
		// Constructor/Destructor
								ExportFormat()	{}
		virtual					~ExportFormat()	{}

		// Exporter init
		virtual	bool			Init(bool motion)												= 0;

		// Main scene info
		virtual	bool			SetSceneInfo		(const MainDescriptor&			maininfo)	= 0;

		// Export loop
		virtual	bool			ExportCamera		(const CameraDescriptor&		camera)		= 0;
		virtual	bool			ExportController	(const ControllerDescriptor&	controller)	= 0;
		virtual	bool			ExportHelper		(const HelperDescriptor&		helper)		= 0;
		virtual	bool			ExportLight			(const LightDescriptor&			light)		= 0;
		virtual	bool			ExportMaterial		(const MaterialDescriptor&		material)	= 0;
		virtual	bool			ExportMesh			(const MeshDescriptor&			mesh)		= 0;
		virtual	bool			ExportMotion		(const MotionDescriptor&		motion)		= 0;
		virtual	bool			ExportShape			(const ShapeDescriptor&			shape)		= 0;
		virtual	bool			ExportSpaceWarp		(const SpaceWarpDescriptor&		space_warp)	= 0;
		virtual	bool			ExportTexture		(const TextureDescriptor&		texture)	= 0;

		// End of export notification
		virtual	bool			EndExport			(const StatsDescriptor&			stats)		= 0;

		// Exportation settings
		inline_	void			SetSettings(const ExportSettings& settings)					{ mSettings = settings; }
		inline_	void			GetSettings(ExportSettings& settings)				const	{ settings = mSettings; }

		// Output filename
		inline_	void			SetOutputFilename(const String& str)						{ mFilename = str;		}

		// Log-string
				void			SetLogString(const String& str)								{ mLogString = str;		}
				void			SetLogString(const char* str)								{ mLogString = str;		}
				void			AddLogString(const String& str)								{ mLogString += str;	}
				void			AddLogString(const char* str)								{ mLogString += str;	}
				void			GetLogString(String& str)							const	{ str = mLogString;		}
				void			ResetLogString()											{ mLogString.Reset();	}
		public:
				ExportSettings	mSettings;		//!< Exportation settings
				String			mFilename;		//!< Output filename
		private:
				String			mLogString;		//!< A string to add to the logfile (see below)

		// The log-string has been added so that your exporter can output information in
		// Flexporter's text window. You can use it this way:
		// - Flexporter calls your custom exporter. The log-string is always null.
		// - Put your text in the string by using SetLogString() or AddLogString().
		// - When control gets back to Flexporter, the string is automatically added to
		//	the internal text window.
		// - Then the string is reset for next call.
	};

#endif // FLX_BASEFORMAT_H
