///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Includes needed files from ICE.
 *	\file		FLX_IceIncludes.h
 *	\author		Pierre Terdiman
 *	\date		May, 4, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define FLEXPORTER_SUPPORT_PRELIGHTER

// Here is the savage part : undef some things already defined in MAX
	#undef PI
	#undef HALFPI
	#undef TWOPI
// End of the savage part...

// Define this to discard unnecessary includes (else include all headers)
//#define FLX_MINIMAL_INCLUDES

// Ice includes :

//#define ICE_NO_DLL
//#define BAN_ICECORE_AUTOLINK

//////[ IceCore ]////////////////////////////////////////////////////////////////////////////////////////////

	#define DEFAULT_NEWDELETE		// Don't use our new/delete as the MFC already override them

#ifdef FLX_MINIMAL_INCLUDES
	#define ICEQUOTE_H
//	#define ICETYPES_H
	#define ICECORETYPES_H
	#define ICECOREERRORS_H
//	#define ICEUTILS_H
	#define ICECODESIZE_H
	#define ICECPUID_H
//	#define ICEMEMORYMACROS_H
	#define	ICEMEMORYMANAGER_H
	#define ICEDLL_H
	#define	ICEASSERT_H
	#define	ICESTATS_H
	#define	ICERANDOM_H
//	#define	ICELOG_H
//	#define	ICEFPU_H
	#define	ICELINKEDLIST_H
//	#define	ICECONTAINER_H
	#define	ICESTACK_H
	#define	ICEBITARRAY_H
//	#define	ICECUSTOMARRAY_H
	#define	ICEXLIST_H
	#define	ICETRASHBUFFER_H
	#define	ICEINSTRUMENTTRACE_H
//	#define	ICESTRING_H
	#define	ICESTRINGTABLE_H
	#define	ICECONSTANTS_H
	#define	ICEMSG_H
	#define	ICEREQUEST_H
//	#define	ICEERROR_H
	#define	ICEKEYBOARD_H
//	#define	ICEFILE_H
	#define	ICEBUFFER_H
	#define	ICEHISTORYBUFFER_H
	#define	ICECRC_H
	#define	ICEFUNCTIONBINDING_H
	#define	ICESYSINFO_H
	#define	ICEQSORT_H
	#define	ICENLOGLOGN_H
	#define	ICEINSERTIONSORT_H
//	#define	ICERADIXSORT_H
//	#define	ICETIMEINFO_H
	#define	ICEPROFILER_H
	#define	ICEPERMUTATION_H
	#define	ICEFIELDS_H
//	#define	ICEFIELDDESCRIPTOR_H
//	#define	ICEFIELDCONTROLLER_H
	#define	ICEIMPORTEXPORT_H
	#define	ICEREFTRACKER_H
//	#define	ICECELL_H
	#define	ICEDYNABLOCK_H
	#define	ICESYMBOLICLIST_H
	#define	ICECUSTOMLIST_H
	#define	ICEFLAGCONTROL_H
	#define	ICEWATCHER_H
	#define	ICEKERNEL_H
	#define	ICESMARTCONTAINER_H
//	#define	ICEREGISTRY_H
	#define	ICEPARSER_H
	#define	ICETIMER_H
	#define	ICEHNODE_H
	#define	ICEHASHING_H
	#define	ICENOTIFSYSTEM_H
	#define	ICEDATABINDER_H
	#define	ICEPUBLISHSUBSCRIBE_H
//	#define	FLEXINESDK_H
//	#define	ICEPLUGINMANAGER_H
	#define	ICECALLBACKS_H
	#define	ICEAPI_H
	#define	ICEAPP_H
#endif

	#include <IceCore/IceCore.h>
	using namespace IceCore;

//////[ IceMaths ]////////////////////////////////////////////////////////////////////////////////////////////

#ifdef FLX_MINIMAL_INCLUDES
	#define	ICEAPPROX_H
//	#define	ICEPOINT_H
//	#define	ICEHPOINT_H
//	#define	ICEMATRIX3X3_H
//	#define	ICEMATRIX4X4_H
	#define	ICEMATRIXNXN_H
//	#define	ICEQUAT_H
	#define	ICEANGLEAXIS_H
	#define	ICEPRS_H
	#define	ICEBASIS_H
//	#define	ICEPLANE_H
//	#define	ICERAY_H
	#define	ICERECTANGLE3_H
//	#define	ICEVERTICES_H
	#define	ICEMATRIXPALETTE_H
	#define	ICEEIGEN_H
	#define	ICEFFT_H
//	#define	ICEVOLUMEINTEGRATION_H
	#define	ICEINERTIATENSOR_H
//	#define	ICENEURALNETQUANTIZER_H
	#define	ICESPLINE_H
	#define	ICEZCURVE_H
	#define	ICEIK_H
	#define	ICEWHITEGAUSSIANNOISE_H
	#define	ICEPERLINNOISE_H
	#define	ICEFRACTALBROWNIANMOTION_H
	#define	ICERIDGEDFRACTAL_H
	#define	ICEWORLEYNOISE_H
	#define	ICEPHARRNOISE_H
	#define	ICESKINNERNOISE_H
#endif

	#include <IceMaths/IceMaths.h>
	using namespace IceMaths;

//////[ IceImageWork ]////////////////////////////////////////////////////////////////////////////////////////////

#ifdef FLX_MINIMAL_INCLUDES
//	#define	IML_H
//	#define	ICECOLOR_H
	#define	ICEPALETTE_H
//	#define	ICEPICTURE_H
	#define	ICEFLOATPICTURE_H
	#define	ICEDDS_H
	#define	ICECROPVALUES_H
	#define	ICETEXTURESYNTHESIS_H
	#define	ICEPICLOADER_H
//	#define	ICEBMP_H
//	#define	ICEPCX_H
//	#define	ICETGA_H
//	#define IML_H
#endif

	#include <IceImageWork/IceImageWork.h>
	using namespace IceImageWork;

//////[ Meshmerizer ]////////////////////////////////////////////////////////////////////////////////////////////

#ifdef FLX_MINIMAL_INCLUDES
	// Remove unnecessary includes
	#define	ICECASSINIEGG_H
	#define	ICEMINIBALL_H
//	#define	ICEAABB_H
//	#define	ICEOBB_H
	#define	ICELSS_H
	#define	ICERSS_H
//	#define	ICEBOUNDINGSPHERE_H
//	#define	ICECONVEXHULL_H
	#define	ICECOLLISIONHULL_H
	#define	ICECULLINGHULL_H
	#define	ICEFRUSTUM_H
	#define	ICECONTACT_H
//	#define	ICECUBEINDEX_H
//	#define	ICEVERTEXCLOUD_H
	#define	ICEMOMENT_H
//	#define	ICETRIANGLE_H
	#define	ICESTABBING_H
	#define	ICEQUAD_H
	#define	ICEPOLYGON_H
	#define	ICEEDGELIST_H
	#define	ICEADJACENCIES_H
//	#define	ICEVALENCY_H
	#define	ICEHEIGHTFIELD_H
	#define	ICETRISURFACE_H
//	#define	ICEINDEXEDSURFACE_H
	#define	ICERENDERSURFACE_H
	#define	ICEVERTEXTOFACES_H
	#define	ICEBSP_H
	#define	ICESHAPE_H
	#define	ICETSS_H
	#define	ICECUBE_H
	#define	ICESPHERE_H
	#define	ICEBUNNY_H
	#define	ICEROOM_H
	#define	ICEUVCREATOR_H
//	#define	ICENORMALS_H
	#define	ICELISTOPTIMIZER_H
	#define	ICESTRIPER_H
//	#define	ICEMESHBUILDER2_H
	#define	ICEQUANTIZE_H
	#define	ICEBOXPOINTDIST_H
	#define	ICETRIPOINTDIST_H
	#define	ICEEDGEEDGEDIST_H
	#define	ICETRITRIDIST_H
	#define	ICESEGMENTSEGMENTDIST_H
	#define	ICESEGMENTTRIDIST_H
	#define	ICESEGMENTBOXDIST_H
	#define	ICEPOINTRECTANGLEDIST_H
	#define	ICESEGMENTRECTANGLEDIST_H
	#define	ICETRIANGLERECTANGLEDIST_H
	#define	ICEBOXBOXDIST_H
//	#define	ICERAYAABB_H
	#define	ICERAYSPHERE_H
	#define	ICERAYPLANE_H
//	#define	ICERAYTRI_H
	#define	ICETRITRI_H
	#define	ICEPLANEAABB_H
	#define	ICEOBBOBB_H
	#define	ICETRIAABB_H
	#define	ICESPHERESPHERE_H
	#define	ICESPHEREAABB_H
	#define	ICESPHEREPLANE_H
	#define	ICESPHEREPOINT_H
	#define	ICESPHEREOBB_H
	#define	ICESPHERECONE_H
	#define	ICEPOINTOBB_H
	#define	ICETRISPHERE_H
	#define	ICETRICYLINDER_H
	#define	ICEBOXPRUNING_H
	#define	ICEINTERVAL_H
	#define	ICELODRUNNER_H
	#define	PROGRESSIVE_MESH_H
	#define	ICESUBDIVISIONHISTORY_H
	#define	ICESUBDIVISION_H
	#define	ICEBUTTERFLY_H
	#define	ICEMESHPACKER_H
	#define	ICEMESHPACKER2_H
	#define	ICEGRIDBUTTERFLY_H
	#define	ICESILHOUETTE_H
	#define	ICENORMALMASK_H
	#define	ICEHILLCLIMBING_H
	#define	ICEDECAL_H
	#define	ICEPROGRESSIVEEIGENBOXES_H
	#define	ICEINNERBOXES_H
#endif

	#define Ray IceMaths::Ray		// Resolve ambiguous symbols
	#define CTCCONTINUOUS_H
	#include <Meshmerizer/Meshmerizer.h>
	using namespace Meshmerizer;

	#define Interval ::Interval		// Resolve ambiguous symbols

#ifdef FLEXPORTER_SUPPORT_PRELIGHTER

//	#include <Opcode/Opcode.h>
//	using namespace Opcode;

	#define Quat IceMaths::Quat		// Resolve ambiguous symbols
	#define Edge Meshmerizer::Edge	// Resolve ambiguous symbols
	#undef SHADE_PHONG

	#undef	GetControlInterface
	#include <IcePrelighter/IcePrelighterAFX.h>
	using namespace IcePrelighter;

	#undef Quat
	#undef Edge
#endif