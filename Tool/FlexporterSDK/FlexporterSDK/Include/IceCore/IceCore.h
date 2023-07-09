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
#ifndef __ICECORE_H__
#define __ICECORE_H__

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

#ifndef __ICEQUOTE_H__
	#include "IceQuote.h"
#endif
#ifdef NETTLE_MEMORY
	#include "MemoryManager/mmgr.h"
#endif

// ICE includes
#ifndef	__ICEASSERT_H__
	#include "IceAssert.h"
#endif
#ifndef __ICETYPES_H__
	#include "IceTypes.h"
#endif
#ifndef __ICEMEMORYMACROS_H__
	#include "IceMemoryMacros.h"
#endif
#ifndef	_H_MMGR	// Already use this
#ifndef	__ICEMEMORYMANAGER_H__
	#include "IceMemoryManager.h"
#endif
#ifndef	__ICEOLDMM_H__
	#include "IceOldMM.h"
#endif
#endif
#ifndef __ICEDLL_H__
	#include "IceDll.h"
#endif
#ifndef __ICECODESIZE_H__
	#include "IceCodeSize.h"
#endif
#ifndef __ICECPUID_H__
	#include "IceCPUID.h"
#endif

	namespace IceCore
	{
		#ifndef __ICECORETYPES_H__
			#include "IceCoreTypes.h"
		#endif
		#ifndef __ICECOREERRORS_H__
			#include "IceCoreErrors.h"
		#endif
		#ifndef __ICEUTILS_H__
			#include "IceUtils.h"
		#endif
		#ifndef	__ICESTATS_H__
			#include "IceStats.h"
		#endif
		#ifndef	__ICERANDOM_H__
			#include "IceRandom.h"
		#endif
		#ifndef	__ICELOG_H__
			#include "IceLog.h"
		#endif
		#ifndef	__ICEFPU_H__
			#include "IceFPU.h"
		#endif
		#ifndef	__ICELINKEDLIST_H__
			#include "IceLinkedList.h"
		#endif
		#ifndef	__ICECONTAINER_H__
			#include "IceContainer.h"
		#endif
		#ifndef	__ICEPAIRS_H__
			#include "IcePairs.h"
		#endif
		#ifndef	__ICESTACK_H__
			#include "IceStack.h"
		#endif
		#ifndef	__ICEPRIORITYQUEUE_H__
			#include "IcePriorityQueue.h"
		#endif
		#ifndef	__ICEBITARRAY_H__
			#include "IceBitArray.h"
		#endif
		#ifndef	__ICECUSTOMARRAY_H__
			#include "IceCustomArray.h"
		#endif
		#ifndef	__ICEXLIST_H__
			#include "IceXList.h"
		#endif
		#ifndef	__ICEHANDLEMANAGER_H__
			#include "IceHandleManager.h"
		#endif
		#ifndef	__ICESIGNATURE_H__
			#include "IceSignature.h"
		#endif
		#ifndef	__ICETRASHBUFFER_H__
			#include "IceTrashBuffer.h"
		#endif
		#ifndef	__ICEINSTRUMENTTRACE_H__
			#include "IceInstrumentTrace.h"
		#endif
		#ifndef	__ICESTRING_H__
			#include "IceString.h"
		#endif
		#ifndef	__ICESTRINGTABLE_H__
			#include "IceStringTable.h"
		#endif
		#ifndef	__ICECONSTANTS_H__
			#include "IceConstants.h"
		#endif
		#ifndef	__ICEERROR_H__
			#include "IceError.h"
		#endif
		#ifndef	__ICESYMBOLICLIST_H__
			#include "IceSymbolicList.h"
		#endif
		#ifndef	__ICEKEYBOARD_H__
			#include "IceKeyboard.h"
		#endif
		#ifndef	__ICEFILE_H__
			#include "IceFile.h"
		#endif
		#ifndef	__ICEBUFFER_H__
			#include "IceBuffer.h"
		#endif
		#ifndef	__ICEHISTORYBUFFER_H__
			#include "IceHistoryBuffer.h"
		#endif
		#ifndef	__ICECRC_H__
			#include "IceCRC.h"
		#endif
		#ifndef	__ICEFUNCTIONBINDING_H__
			#include "IceFunctionBinding.h"
		#endif
		#ifndef	__ICESYSINFO_H__
			#include "IceSysInfo.h"
		#endif
		#ifndef	__ICEQSORT_H__
			#include "IceQSort.h"
		#endif
		#ifndef	__ICENLOGLOGN_H__
			#include "IceNLogLogN.h"
		#endif
		#ifndef	__ICEINSERTIONSORT_H__
			#include "IceInsertionSort.h"
		#endif
		#ifndef	__ICERADIXSORT_H__
			#include "IceRevisitedRadix.h"
		#endif
		#ifndef	__ICETIMEINFO_H__
			#include "IceTimeInfo.h"
		#endif
		#ifndef	__ICELINEARDAMPER_H__
			#include "IceLinearDamper.h"
		#endif
		#ifndef	__ICEPROFILER_H__
			#include "IceProfiler.h"
		#endif
		#ifndef	__ICEPERMUTATION_H__
			#include "IcePermutation.h"
		#endif
		#ifndef	__ICEFIELDS_H__
			#include "IceFields.h"
		#endif
		#ifndef	__ICEFIELDDESCRIPTOR_H__
			#include "IceFieldDescriptor.h"
		#endif
		#ifndef	__ICEFIELDCONTROLLER_H__
			#include "IceFieldController.h"
		#endif
		#ifndef	__ICERTTI_H__
			#include "IceRTTI.h"
		#endif
		#ifndef	__ICEIMPORTEXPORT_H__
			#include "IceImportExport.h"
		#endif
		#ifndef	__ICEREFTRACKER_H__
			#include "IceRefTracker.h"
		#endif
		#ifndef	__ICECELL_H__
			#include "IceCell.h"
		#endif
		#ifndef	__ICEMSG_H__
			#include "IceMsg.h"
		#endif
		#ifndef	__ICEDYNABLOCK_H__
			#include "IceDynaBlock.h"
		#endif
		#ifndef	__ICECUSTOMLIST_H__
			#include "IceCustomList.h"
		#endif
		#ifndef	__ICEFLAGCONTROL_H__
			#include "IceFlagControl.h"
		#endif
		#ifndef	__ICEWATCHER_H__
			#include "IceWatcher.h"
		#endif
		#ifndef	__ICEKERNEL_H__
			#include "IceKernel.h"
		#endif
		#ifndef	__ICESMARTCONTAINER_H__
			#include "IceSmartContainer.h"
		#endif
		#ifndef	__ICEREGISTRY_H__
			#include "IceRegistry.h"
		#endif
		#ifndef	__ICEPARSER_H__
			#include "IceParser.h"
		#endif
		#ifndef	__ICETIMER_H__
			#include "IceTimer.h"
		#endif
		#ifndef	__ICEHNODE_H__
			#include "IceHNode.h"
		#endif
		#ifndef	__ICEHASHING_H__
			#include "IceHashing.h"
		#endif
		#ifndef	__ICENOTIFSYSTEM_H__
			#include "IceNotifSystem.h"
		#endif
		#ifndef	__ICEDATABINDER_H__
			#include "IceDataBinder.h"
		#endif
		#ifndef	__ICEPUBLISHSUBSCRIBE_H__
			#include "IcePublishSubscribe.h"
		#endif

//			#define FLEXINE_API __declspec(dllimport)
			#define FLEXINE_API ICECORE_API
		#ifndef	__FLEXINESDK_H__
			#include "FlexineSDK.h"
		#endif
		#ifndef	__ICEPLUGINMANAGER_H__
			#include "IcePluginManager.h"
		#endif
		#ifndef	__ICECALLBACKS_H__
			#include "IceCallbacks.h"
		#endif
		#ifndef	__ICEREQUEST_H__
			#include "IceRequest.h"
		#endif
		#ifndef	__ICEAPI_H__
			#include "IceAPI.h"
		#endif
		#ifndef	__ICEAPP_H__
			#include "IceAPP.h"
		#endif
		#ifndef	__ICEGDIAPP_H__
			#include "IceGDIApp.h"
		#endif
		#ifndef	__ICEHUFFMAN_H__
			#include "IceHuffman.h"
		#endif
		#ifndef	__ICEMOUSE_H__
			#include "IceMouse.h"
		#endif
		#ifndef	__ICETHREAD_H__
			#include "IceThread.h"
		#endif
		#ifndef	__ICEAVI_H__
			#include "IceAVI.h"
		#endif
		#ifndef	__ICEARRAYREDUCER_H__
			#include "IceArrayReducer.h"
		#endif

		#ifndef	__ICECOREAPI_H__
			#include "IceCoreAPI.h"
		#endif
	}

#endif // __ICECORE_H__

