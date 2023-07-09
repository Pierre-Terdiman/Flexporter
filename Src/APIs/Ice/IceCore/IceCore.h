///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Main file for IceCore.dll.
 *	\file		IceCore.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICECORE_H
#define ICECORE_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compilation messages

#ifndef __cplusplus
	#error ICE needs C++ compilation...
#endif

#if defined(ICECORE_EXPORTS)
	#pragma message("Compiling ICE Core")
#elif !defined(ICECORE_EXPORTS)
	#pragma message("Using ICE Core")
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Automatic linking
	#ifndef BAN_ICECORE_AUTOLINK
		#ifdef _DEBUG
			#pragma comment(lib, "IceCore_D.lib")
		#else
			#pragma comment(lib, "IceCore.lib")
		#endif
	#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes

// Windows includes
#ifdef WIN32
	#include <windows.h>		// General purpose Win32 functions
	#include <windowsx.h>
	#include <vfw.h>			// Header File For Video For Windows
	#include <commctrl.h>		// For the toolbar
	#include <commdlg.h>		// For open/save file structures
	#include <shellapi.h>		// For drag & drop
	#include <shlobj.h>			// For browse info
#endif // WIN32

// Standard includes
#ifdef _DEBUG
	#include <crtdbg.h>			// C runtime debug functions
#endif
	#include <stdio.h>			// Standard Input/Output functions
	#include <stdlib.h>
	#include <float.h>
	#include <malloc.h>			// For _alloca
	#include <math.h>

//#define NETTLE_MEMORY

#ifdef NETTLE_MEMORY
	// Paul Nettle's memory manager
	#include <new>

	#ifndef	WIN32
	#include <unistd.h>
	#endif
#endif

	// First file to include
	#include "IcePreprocessor.h"

//#define ICECORE_TIMEINFO_INTEGER

// External guards are used to release the lib with selected headers only

#ifndef ICEQUOTE_H
	#include "IceQuote.h"
#endif
#ifdef NETTLE_MEMORY
	#include "MemoryManager/mmgr.h"
#endif

// ICE includes
#ifndef	ICEASSERT_H
	#include "IceAssert.h"
#endif
#ifndef ICETYPES_H
	#include "IceTypes.h"
#endif
#ifndef ICEMEMORYMACROS_H
	#include "IceMemoryMacros.h"
#endif
#ifndef	_H_MMGR	// Already use this
#ifndef	ICEMEMORYMANAGER_H
	#include "IceMemoryManager.h"
#endif
#ifndef	ICEOLDMM_H
	#include "IceOldMM.h"
#endif
#endif
#ifndef ICEDLL_H
	#include "IceDll.h"
#endif
#ifndef ICECODESIZE_H
	#include "IceCodeSize.h"
#endif
#ifndef ICECPUID_H
	#include "IceCPUID.h"
#endif

	namespace IceCore
	{
		#ifndef ICECORETYPES_H
			#include "IceCoreTypes.h"
		#endif
		#ifndef ICECOREERRORS_H
			#include "IceCoreErrors.h"
		#endif
		#ifndef ICEUTILS_H
			#include "IceUtils.h"
		#endif
		#ifndef	ICESTATS_H
			#include "IceStats.h"
		#endif
		#ifndef	ICERANDOM_H
			#include "IceRandom.h"
		#endif
		#ifndef	ICELOG_H
			#include "IceLog.h"
		#endif
		#ifndef	ICEFPU_H
			#include "IceFPU.h"
		#endif
		#ifndef ICEALLOCATOR_H
			#include "IceAllocator.h"
		#endif
		#ifndef	ICELINKEDLIST_H
			#include "IceLinkedList.h"
		#endif
		#ifndef	ICECONTAINER_H
			#include "IceContainer.h"
		#endif
		#ifndef	ICEPAIRS_H
			#include "IcePairs.h"
		#endif
		#ifndef	ICEPAIRMANAGER_H
			#include "IcePairManager.h"
		#endif
		#ifndef	ICESTACK_H
			#include "IceStack.h"
		#endif
		#ifndef	ICEPRIORITYQUEUE_H
			#include "IcePriorityQueue.h"
		#endif
		#ifndef	ICEBITARRAY_H
			#include "IceBitArray.h"
		#endif
		#ifndef	ICECUSTOMARRAY_H
			#include "IceCustomArray.h"
		#endif
		#ifndef	ICEXLIST_H
			#include "IceXList.h"
		#endif
		#ifndef	ICEHANDLEMANAGER_H
			#include "IceHandleManager.h"
		#endif
		#ifndef	ICESIGNATURE_H
			#include "IceSignature.h"
		#endif
		#ifndef	ICETRASHBUFFER_H
			#include "IceTrashBuffer.h"
		#endif
		#ifndef	ICEINSTRUMENTTRACE_H
			#include "IceInstrumentTrace.h"
		#endif
		#ifndef	ICESTRING_H
			#include "IceString.h"
		#endif
		#ifndef	ICESTRINGTABLE_H
			#include "IceStringTable.h"
		#endif
		#ifndef	ICECONSTANTS_H
			#include "IceConstants.h"
		#endif
		#ifndef	ICEERROR_H
			#include "IceError.h"
		#endif
		#ifndef	ICESYMBOLICLIST_H
			#include "IceSymbolicList.h"
		#endif
		#ifndef	ICEKEYBOARD_H
			#include "IceKeyboard.h"
		#endif
		#ifndef	ICEFILE_H
			#include "IceFile.h"
		#endif
		#ifndef	ICEBUFFER_H
			#include "IceBuffer.h"
		#endif
		#ifndef	ICEHISTORYBUFFER_H
			#include "IceHistoryBuffer.h"
		#endif
		#ifndef	ICECRC_H
			#include "IceCRC.h"
		#endif
		#ifndef	ICEFUNCTIONBINDING_H
			#include "IceFunctionBinding.h"
		#endif
		#ifndef	ICESYSINFO_H
			#include "IceSysInfo.h"
		#endif
		#ifndef	ICEQSORT_H
			#include "IceQSort.h"
		#endif
		#ifndef	ICENLOGLOGN_H
			#include "IceNLogLogN.h"
		#endif
		#ifndef	ICEINSERTIONSORT_H
			#include "IceInsertionSort.h"
		#endif
		#ifndef	ICERADIXSORT_H
			#include "IceRevisitedRadix.h"
		#endif
		#ifndef	ICETIMEINFO_H
			#include "IceTimeInfo.h"
		#endif
		#ifndef	ICETIMESTAMPMANAGER_H
			#include "IceTimestampManager.h"
		#endif
		#ifndef	ICELINEARDAMPER_H
			#include "IceLinearDamper.h"
		#endif
		#ifndef	ICEPROFILER_H
			#include "IceProfiler.h"
		#endif
		#ifndef	ICEPERMUTATION_H
			#include "IcePermutation.h"
		#endif
		#ifndef	ICEFIELDS_H
			#include "IceFields.h"
		#endif
		#ifndef	ICEFIELDDESCRIPTOR_H
			#include "IceFieldDescriptor.h"
		#endif
		#ifndef	ICEFIELDCONTROLLER_H
			#include "IceFieldController.h"
		#endif
		#ifndef	ICERTTI_H
			#include "IceRTTI.h"
		#endif
		#ifndef	ICEIMPORTEXPORT_H
			#include "IceImportExport.h"
		#endif
		#ifndef	ICEREFTRACKER_H
			#include "IceRefTracker.h"
		#endif
		#ifndef	ICECELL_H
			#include "IceCell.h"
		#endif
		#ifndef	ICEMSG_H
			#include "IceMsg.h"
		#endif
		#ifndef	ICEDYNABLOCK_H
			#include "IceDynaBlock.h"
		#endif
		#ifndef	ICECUSTOMLIST_H
			#include "IceCustomList.h"
		#endif
		#ifndef	ICEFLAGCONTROL_H
			#include "IceFlagControl.h"
		#endif
		#ifndef	ICEWATCHER_H
			#include "IceWatcher.h"
		#endif
		#ifndef	ICEKERNEL_H
			#include "IceKernel.h"
		#endif
		#ifndef	ICESMARTCONTAINER_H
			#include "IceSmartContainer.h"
		#endif
		#ifndef	ICEREGISTRY_H
			#include "IceRegistry.h"
		#endif
		#ifndef	ICEPARSER_H
			#include "IceParser.h"
		#endif
		#ifndef	ICETIMER_H
			#include "IceTimer.h"
		#endif
		#ifndef	ICEHNODE_H
			#include "IceHNode.h"
		#endif
		#ifndef	ICEHASHING_H
			#include "IceHashing.h"
		#endif
		#ifndef	ICENOTIFSYSTEM_H
			#include "IceNotifSystem.h"
		#endif
		#ifndef	ICEDATABINDER_H
			#include "IceDataBinder.h"
		#endif
		#ifndef	ICEPUBLISHSUBSCRIBE_H
			#include "IcePublishSubscribe.h"
		#endif

//			#define FLEXINE_API __declspec(dllimport)
			#define FLEXINE_API ICECORE_API
		#ifndef	FLEXINESDK_H
			#include "FlexineSDK.h"
		#endif
		#ifndef	ICEPLUGINMANAGER_H
			#include "IcePluginManager.h"
		#endif
		#ifndef	ICECALLBACKS_H
			#include "IceCallbacks.h"
		#endif
		#ifndef	ICEREQUEST_H
			#include "IceRequest.h"
		#endif
		#ifndef	ICEAPI_H
			#include "IceAPI.h"
		#endif
		#ifndef	ICEAPP_H
			#include "IceAPP.h"
		#endif
		#ifndef	ICEGDIAPP_H
			#include "IceGDIApp.h"
		#endif
		#ifndef	ICEHUFFMAN_H
			#include "IceHuffman.h"
		#endif
		#ifndef	ICEMOUSE_H
			#include "IceMouse.h"
		#endif
		#ifndef	ICETHREAD_H
			#include "IceThread.h"
		#endif
		#ifndef	ICEAVI_H
			#include "IceAVI.h"
		#endif
		#ifndef	ICEARRAYREDUCER_H
			#include "IceArrayReducer.h"
		#endif

		#ifndef	ICECOREAPI_H
			#include "IceCoreAPI.h"
		#endif
	}

#endif // ICECORE_H

