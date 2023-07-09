
// Cheap templates inspired from MESA

#ifdef TEMPLATED_CALLBACK
#ifdef TEMPLATED_CONTAINER
#ifdef TEMPLATED_MANAGER
#ifdef TEMPLATED_CODE
#ifdef TEMPLATED_PARAM
#ifdef TEMPLATED_SIZE

	// This class is a collection of callbacks
	class TEMPLATED_EXPORT TEMPLATED_CONTAINER : public Container
	{
		public:
		// Constructor / Destructor
		inline_								TEMPLATED_CONTAINER()						{}
		inline_								~TEMPLATED_CONTAINER()						{}

		// Returns the number of callbacks in the container
		inline_			udword				GetNbCallbacks()					const	{ return GetNbEntries();						}
		// Returns the whole list of callbacks
		inline_			TEMPLATED_CALLBACK*	GetCallbacks()						const	{ return (TEMPLATED_CALLBACK*)GetEntries();		}
		// Returns the ith callback
		inline_			TEMPLATED_CALLBACK	GetCallback(udword i)				const	{ return (TEMPLATED_CALLBACK)GetEntry(i);		}

		// Adds a callback to the container
		inline_			void				AddCallback(TEMPLATED_CALLBACK callback)	{ Add(udword(callback));						}
		// Remove a callback from the container, otherwise preserving the insertion order
		inline_			bool				RemoveCallback(TEMPLATED_CALLBACK callback)	{ return DeleteKeepingOrder(udword(callback));	}

		// Executes all callbacks in order
		inline_			void				ExecCallbacks(TEMPLATED_PARAM param, udword context, udword user_data)
											{
												// Loop through callbacks
												udword NbCallbacks = GetNbEntries();
												TEMPLATED_CALLBACK* Callbacks = (TEMPLATED_CALLBACK*)GetEntries();
												while(NbCallbacks--)
												{
													// Get current callback
													TEMPLATED_CALLBACK cb = *Callbacks++;
													// Execute callback
													if(cb)	(cb)(param, context, user_data);
												}
											}
	};

	// This class manages a collection of collections of callbacks
	class TEMPLATED_EXPORT TEMPLATED_MANAGER
	{
		public:
		// Constructor / Destructor
												TEMPLATED_MANAGER() : mUserData(0)
												{
													// Clear collection pointers
													ZeroMemory(mCollections, TEMPLATED_SIZE*sizeof(TEMPLATED_CONTAINER*));
												}

												~TEMPLATED_MANAGER()
												{
													// Loop through collections
													for(udword i=0;i<TEMPLATED_SIZE;i++)
													{
														// Release current collection
														DELETESINGLE(mCollections[i]);
													}
												}

		// Setup user-defined data
		inline_			void					SetUserData(udword data)				{ mUserData = data;		}
		// Get user-defined data
		inline_			udword					GetUserData()					const	{ return mUserData;		}

		// Stores a callback in a collection
						bool					AddCallback(TEMPLATED_CODE code, TEMPLATED_CALLBACK callback)
												{
													// Allocate a collection on first call
													if(!mCollections[code])
													{
														mCollections[code] = new TEMPLATED_CONTAINER;
														CHECKALLOC(mCollections[code]);
													}

													// Keep track of callback
													mCollections[code]->AddCallback(callback);
													return true;
												}

		// Removes a callback from a collection
						bool					RemoveCallback(TEMPLATED_CODE code, TEMPLATED_CALLBACK callback)
												{
													// Check the collection exists
													if(!mCollections[code])	return false;

													// Try removing the callback from the collection
													return mCollections[code]->RemoveCallback(callback);
												}

		// Executes callbacks from a particular collection
		inline_			void					ExecCallbacks(TEMPLATED_CODE code, TEMPLATED_PARAM param, udword context)
												{
													// Check the collection exists, then execute callbacks
													if(mCollections[code])	mCollections[code]->ExecCallbacks(param, context, mUserData);
												}
		private:
						udword					mUserData;
						TEMPLATED_CONTAINER*	mCollections[TEMPLATED_SIZE];
	};

#endif
#endif
#endif
#endif
#endif
#endif

#undef TEMPLATED_EXPORT
#undef TEMPLATED_CALLBACK
#undef TEMPLATED_CONTAINER
#undef TEMPLATED_MANAGER
#undef TEMPLATED_CODE
#undef TEMPLATED_PARAM
#undef TEMPLATED_SIZE
