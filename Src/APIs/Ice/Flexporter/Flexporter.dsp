# Microsoft Developer Studio Project File - Name="Flexporter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Flexporter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Flexporter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Flexporter.mak" CFG="Flexporter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Flexporter - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Flexporter - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Flexporter - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Z<none> /Fr
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy release\Flexporter.lib y:\lib /Q /Y
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Flexporter - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\Flexporter_D.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy debug\Flexporter_D.lib y:\lib /Q /Y
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Flexporter - Win32 Release"
# Name "Flexporter - Win32 Debug"
# Begin Group "API"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Flexporter.h
# End Source File
# Begin Source File

SOURCE=.\FlexporterAFX.h
# End Source File
# Begin Source File

SOURCE=.\FlexporterSettings.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "SDK"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sdk\FlexporterSDK.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FlexporterSDK.txt
# End Source File
# Begin Source File

SOURCE=.\Sdk\FlexporterSDK_IceCore.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FlexporterSDKVersion.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddCamera.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddController.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddHelper.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddJoint.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddLight.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddMaterial.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddMesh.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddMotion.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddPrimitive.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddPRS.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddShape.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddSpaceWarp.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_AddTexture.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_BaseFormat.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_BaseInfo.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_CropValues.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_ExportSettings.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_Keys.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_TimeInfo.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\FLX_Units.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\IceCharacterStudioNodes.h
# End Source File
# Begin Source File

SOURCE=.\Sdk\IceMeshBuilderResults.h
# End Source File
# End Group
# Begin Group "Descriptors"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FLX_BaseInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_CameraDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_ControllerDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_HelperDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_JointDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_LightDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_MaterialDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_MAXNativeMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_MeshDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_MotionDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_ShapeDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_Skeleton.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_SpaceWarpDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_TextureDescriptor.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\FLX_Converter.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_Converter.h
# End Source File
# Begin Source File

SOURCE=.\FLX_Plug.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_Plug.h
# End Source File
# Begin Source File

SOURCE=.\FLX_PlugManager.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_PlugManager.h
# End Source File
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
