
// Cheap templates inspired from MESA

#ifdef TEMPLATED_CALLBACK
#ifdef TEMPLATED_CONTAINER
#ifdef TEMPLATED_MANAGER
#ifdef TEMPLATED_CODE
#ifdef TEMPLATED_PARAM
#ifdef TEMPLATED_SIZE

TEMPLATED_MANAGER::~TEMPLATED_MANAGER()
{
	// Loop through collections
	for(udword i=0;i<TEMPLATED_SIZE;i++)
	{
		// Release current collection
		DELETESINGLE(mCollections[i]);
	}
}

// Stores a callback in a collection
bool TEMPLATED_MANAGER::AddCallback(TEMPLATED_CODE code, TEMPLATED_CALLBACK callback)
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
bool TEMPLATED_MANAGER::RemoveCallback(TEMPLATED_CODE code, TEMPLATED_CALLBACK callback)
{
	// Check the collection exists
	if(!mCollections[code])	return false;

	// Try removing the callback from the collection
	return mCollections[code]->RemoveCallback(callback);
}

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
