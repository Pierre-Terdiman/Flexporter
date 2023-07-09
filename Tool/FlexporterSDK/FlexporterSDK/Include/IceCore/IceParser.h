///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains a parser.
 *	\file		IceParser.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEPARSER_H__
#define __ICEPARSER_H__

	//
	FUNCTION ICECORE_API bool ReadScript(const char* name);

	// Parse & search
	FUNCTION ICECORE_API bool ParseAndSearch(const String& str, const String& keywords);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	User-callback, called by the parser each time a new string parameter has been found.
	 *	\param		param		[in] the string parameter
	 *	\param		user_data	[in] user-defined data
	 *	\return		true to end parsing
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef bool	(*PARSE_CALLBACK)		(ubyte* param, udword user_data);

	class ICECORE_API TextParser
	{
		public:
		// Constructor/Destructor
								TextParser();
								~TextParser();

		// Initialize separators and end symbol
		static	bool			Init();
		static	void			SetSeparator(ubyte symbol)				{ mSeparator[symbol] = true;				}
		static	void			SetDiscarded(ubyte symbol)				{ mDiscarded[symbol] = true;				}

		// Callback control
		inline_	TextParser&		SetUserData(udword data)				{ mUserData	= data;			return *this;	}
		inline_	TextParser&		SetCallback(PARSE_CALLBACK callback)	{ mCallback	= callback;		return *this;	}

		// Parsing method
				bool			Parse(const char* text);
		private:
		static	bool			mSeparator[256];	//!< Separators
		static	bool			mDiscarded[256];	//!< Discarded symbols
		static	ubyte			mEndSymbol;			//!< End marker

		// User callback
				udword			mUserData;			//!< User-defined data sent to callbacks
				PARSE_CALLBACK	mCallback;
	};

	class ParameterBlock;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	User-callback, called by the parser each time a new string parameter has been found, or when an error occurs.
	 *	\param		command		[in] original unmodified command currently parsed
	 *	\param		pb			[in] command's parameter block
	 *	\param		context		[in] callback context (e.g. error codes for error callback)
	 *	\param		user_data	[in] user-defined data
	 *	\param		cmd			[in] possible matched command's parameter block
	 *	\return		true to go ahead and continue parsing, false to stop it
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef bool	(*CMDPARSE_CALLBACK)		(const char* command, const ParameterBlock& pb, udword context, udword user_data, const ParameterBlock* cmd);

	enum BlockFixingCode
	{
		BFC_PRESERVE			= 0,			//!< Don't modify strings
		BFC_REMOVE_TABS			= (1<<0),		//!< Replace tabs with spaces
		BFC_MAKE_LOWER_CASE		= (1<<1),		//!< Make lower case
		BFC_REMOVE_SEMICOLON	= (1<<2),		//!< Remove end-of-line semicolons
		BFC_DISCARD_COMMENTS	= (1<<3),		//!< Discard comments
		BFC_DISCARD_UNKNOWNCMDS	= (1<<4),		//!< Discard unknown commands
		BFC_DISCARD_INVALIDCMDS	= (1<<5),		//!< Discard invalid commands
		BFC_DISCARD_GLOBALCMDS	= (1<<6),		//!< Discard global commands

		BFC_COMMENTS_FLIPSTATUS	= (1<<7),		//!< Internal comments/code status

		BFC_READONLY			= BFC_COMMENTS_FLIPSTATUS,

		BFC_FORCE_DWORD			= 0x7fffffff
	};

	class ICECORE_API ParameterBlock
	{
		public:
		// Constructor/Destructor
									ParameterBlock();
									ParameterBlock(const char* str);
									ParameterBlock(const String& str);
									ParameterBlock(const ParameterBlock& block);
									~ParameterBlock();
		// Initialize
				udword				Create(const String& str);
				udword				Create(const ParameterBlock& block);
		// Reset
				void				DeleteAll();
		// Operations
				bool				MakeLowerCase();
				bool				MakeUpperCase();
		// Settings
				void				BindData(CMDPARSE_CALLBACK callback, udword context, bool check_params);
				void				ForceNbParams(udword i)			{ mNbParams = i;									}
		// Data access
		inline_	udword				GetNbParams()			const	{ return mNbParams;									}
		inline_	String&				GetParam(udword i)				{ ASSERT(i>=0 && i<mNbParams); return mTable[i];	}
		inline_	const String&		GetParam(udword i)		const	{ ASSERT(i>=0 && i<mNbParams); return mTable[i];	}
		inline_	String&				operator[](udword i)	const	{ ASSERT(i>=0 && i<mNbParams); return mTable[i];	}

		inline_	CMDPARSE_CALLBACK	GetCallback()			const	{ return mCallback;									}
		inline_	udword				GetContext()			const	{ return mContext;									}
		inline_	bool				MustCheckParams()		const	{ return mCheckParams;								}

		private:
		// Parameters
				udword				mNbParams;
				String*				mTable;
		// Data
				CMDPARSE_CALLBACK	mCallback;
				udword				mContext;
				bool				mCheckParams;
	};

	class ICECORE_API CommandManager
	{
		public:
		// Constructor/Destructor
									CommandManager();
		virtual						~CommandManager();
		// Commands
				bool				FlushCommands();
				bool				RegisterCommand(const char* command, CMDPARSE_CALLBACK callback=null, udword context=0, bool check_params=true);
				bool				RegisterCommand(const String& command, CMDPARSE_CALLBACK callback=null, udword context=0, bool check_params=true);
		inline_	udword				GetNbCommands()			const	{ return mCommands.GetNbEntries();					}
		inline_	ParameterBlock*		GetCommand(udword i)	const	{ return (ParameterBlock*)mCommands.GetEntry(i);	}
		private:
		// Registered commands and associated data
				Container			mCommands;
	};

	ICECORE_API CommandManager*	GetCommandManager();

	class ICECORE_API CommandParser : public CommandManager
	{
		public:
		// Constructor/Destructor
									CommandParser();
		virtual						~CommandParser();

									DECLARE_FLAGS(BlockFixingCode, mParsingFlags, BFC_READONLY)
		// Callback control
		inline_	void				SetUserData(udword user_data)					{ mUserData			= user_data;	}
		inline_	void				SetParseCallback(CMDPARSE_CALLBACK callback)	{ mParseCallback	= callback;		}
		inline_	void				SetErrorCallback(CMDPARSE_CALLBACK callback)	{ mErrorCallback	= callback;		}

		// Parsing method
				bool				CommandParse(const char* buffer);
		// To overload
		virtual	bool				Execute(const char* source)						{ mParsingFlags&=~BFC_COMMENTS_FLIPSTATUS; return true;	}

		private:
		// User callback
				udword				mUserData;				//!< User-defined data sent to callbacks
				CMDPARSE_CALLBACK	mParseCallback;			//!< User-defined callback for each line
				CMDPARSE_CALLBACK	mErrorCallback;			//!< User-defined callback for errors
		// Current line is cut to pieces in this parameter block
				ParameterBlock		mPB;
		// Internal methods
				bool				FindCommand(const char* buffer, const CommandManager* cm, const ParameterBlock& work, bool& valid_command, ParameterBlock*& cmd);
	};

	class ICECORE_API BufferParser : public CommandParser
	{
		public:
		// Constructor/Destructor
									BufferParser();
		virtual						~BufferParser();
		// Parsing method
		virtual	bool				Execute(const char* buffer);
	};

	class ICECORE_API ScriptFile : public CommandParser
	{
		public:
		// Constructor/Destructor
									ScriptFile();
		virtual						~ScriptFile();
		// Parsing method
		virtual	bool				Execute(const char* filename);
	};

	class ICECORE_API Parsable
	{
		public:
		// Constructor/Destructor
									Parsable();
		virtual						~Parsable();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Parses a text and call the parsing callback for each line.
		 *	\param		text	[in] the text to parse
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				bool				Parse(const char* text);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	User-callback, called by the parser each time a new line has been found.
		 *	\param		line	[in] the new line
		 *	\param		pb		[in] a parameter-block made from current line
		 *	\return		true to end parsing
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual	bool				Parameter(const char* line, const ParameterBlock& pb)	{ return false;	}
	};

#endif // __ICEPARSER_H__
