///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Main file for Meshmerizer.dll.
 *	\file		Meshmerizer.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef MESHMERIZER_H
#define MESHMERIZER_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compilation messages
#if defined(MESHMERIZER_EXPORTS)
	#pragma message("Compiling ICE Meshmerizer")
#elif !defined(MESHMERIZER_EXPORTS)
	#pragma message("Using ICE Meshmerizer")
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Automatic linking
	#ifndef BAN_MESHMERIZER_AUTOLINK
		#ifdef _DEBUG
			#pragma comment(lib, "Meshmerizer_D.lib")
			#pragma comment(lib, "Contact_D.lib")
		#else
			#pragma comment(lib, "Meshmerizer.lib")
			#pragma comment(lib, "Contact.lib")
		#endif
	#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Preprocessor
#ifndef ICE_NO_DLL
	#ifdef MESHMERIZER_EXPORTS
		#define MESHMERIZER_API __declspec(dllexport)
	#else
		#define MESHMERIZER_API __declspec(dllimport)
	#endif
#else
		#define MESHMERIZER_API
#endif

	#include "MeshmerizerSettings.h"

	// Contact
	#ifndef CONTACT_H
//		#define CONTACT_API	MESHMERIZER_API

		// Removes multiple defines...
		#define CONTACT_API

		// Removes: Y:\APIS\ICE\IcePhysics\IceContactDetector.h(7) : warning C4275: non dll-interface class 'Ctc::Contact' used as base for dll-interface class 'IcePhysics::RigidContact'
//		#define CONTACT_API	__declspec(dllexport)

		#include <Contact\Contact.h>
		using namespace Ctc;
	#endif

	namespace Meshmerizer
	{
		#ifndef	MESHMERIZERERRORS_H
			#include "MeshmerizerErrors.h"
		#endif

		// Default surfaces

		#ifndef	ICEOBJECTINTERFACE_H
			#include "IceObjectInterface.h"
		#endif


//		#include "IceBunny.h"
//		#ifndef	ICECASSINIEGG_H
//			#include "IceCassiniEgg.h"
//		#endif

		// Bounding volumes
#ifndef	ICEEDGELIST_H
	#include "IceEdgeList.h"
#endif
		#ifndef	ICECONVEXHULL_H
			#include "IceConvexHull.h"
		#endif
#ifndef	ICEHILLCLIMBING_H
	#include "IceHillClimbing.h"
#endif
		#ifndef	ICECOLLISIONHULL_H
			#include "IceCollisionHull.h"
		#endif
		#ifndef	ICECULLINGHULL_H
			#include "IceCullingHull.h"
		#endif
		#ifndef	ICEFRUSTUM_H
			#include "IceFrustum.h"
		#endif
		#ifndef	ICEEIGENANALYSIS_H
			#include "IceEigenAnalysis.h"
		#endif

		// Data structures
		#ifndef	ICESURFACEINTERFACE_H
			#include "IceSurfaceInterface.h"
		#endif
		#ifndef	ICEVERTEXCLOUD_H
			#include "IceVertexCloud.h"
		#endif
		#ifndef	ICEPOINTSHELL_H
			#include "IcePointShell.h"
		#endif
		#ifndef	ICESTABBING_H
			#include "IceStabbing.h"
		#endif
		#ifndef	ICEACTIVEEDGES_H
			#include "IceActiveEdges.h"
		#endif
		#ifndef	ICEADJACENCIES_H
			#include "IceAdjacencies.h"
		#endif
		#ifndef	ICEVALENCY_H
			#include "IceValency.h"
		#endif
		#ifndef	ICEHEIGHTFIELD_H
			#include "IceHeightfield.h"
		#endif
		#ifndef	ICETRISURFACE_H
			#include "IceTriSurface.h"
		#endif
		#ifndef	ICEINDEXEDSURFACE_H
			#include "IceIndexedSurface.h"
		#endif
		#ifndef	ICERENDERSURFACE_H
			#include "IceMultiSurface.h"
		#endif
		#ifndef	ICEVERTEXTOFACES_H
			#include "IceVertexToFaces.h"
		#endif
		#ifndef	ICEBSP_H
			#include "IceBSP.h"
		#endif
		#ifndef	ICESHAPE_H
			#include "IceShape.h"
		#endif

		#ifndef	ICEROOM_H
	#include "IceRoom.h"
		#endif

		// Mesh consolidation
		#ifndef	ICEUVCREATOR_H
			#include "IceUVCreator.h"
		#endif
		#ifndef	ICENORMALS_H
			#include "IceNormals.h"
		#endif
		#ifndef	ICELISTOPTIMIZER_H
			#include "IceListOptimizer.h"
		#endif
		#ifndef	ICESTRIPER_H
			#include "IceStriper.h"
		#endif
		#ifndef	ICEMESHBUILDER2_H
			#include "IceMeshBuilderResults.h"
			#include "IceMeshBuilder2.h"
		#endif
		#ifndef	ICEQUANTIZE_H
			#include "IceQuantize.h"
		#endif
		#ifndef	ICEMANIFOLDMESH_H
			#include "IceManifoldMesh.h"
		#endif

		// Intersections
		#ifndef	ICEBOXPRUNING_H
			#include "IceBoxPruning.h"
		#endif

		// Techniques
		#ifndef	ICELODRUNNER_H
			#include "IceLODRunner.h"
		#endif
		#ifndef	PROGRESSIVE_MESH_H
			#include "IceLODRunner2.h"
		#endif
		#ifndef	ICESUBDIVISIONHISTORY_H
			#include "IceSubdivisionHistory.h"
		#endif
		#ifndef	ICESUBDIVISION_H
			#include "IceSubdivision.h"
		#endif
		#ifndef	ICELOOP_H
			#include "IceLoop.h"
		#endif
		#ifndef	ICEBUTTERFLY_H
			#include "IceButterfly.h"
		#endif
		#ifndef	ICEMESHPACKER_H
			#include "IceMeshPacker.h"
		#endif
		#ifndef	ICEMESHPACKER2_H
			#include "IceMeshPacker2.h"
		#endif
		#ifndef	ICEGRIDBUTTERFLY_H
			#include "IceGridButterfly.h"
		#endif
		#ifndef	ICEDUALSILHOUETTE_H
			#include "IceDualSilhouette.h"
		#endif
		#ifndef	ICESILHOUETTE_H
			#include "IceSilhouette.h"
		#endif
		#ifndef	ICENORMALMASK_H
			#include "IceNormalMask.h"
		#endif
		#ifndef	ICEDECAL_H
			#include "IceDecal.h"
		#endif
		#ifndef	ICEPROGRESSIVEEIGENBOXES_H
			#include "IceProgressiveEigenBoxes.h"
		#endif

		#ifndef	ICEINNERBOXES_H
	#include "IceInnerBoxes.h"
		#endif

		#ifndef	MESHMERIZERAPI_H
			#include "MeshmerizerAPI.h"
		#endif
	}

#endif // MESHMERIZER_H
