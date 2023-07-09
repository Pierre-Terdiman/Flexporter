# Microsoft Developer Studio Project File - Name="IceExporter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IceExporter - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IceExporter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IceExporter.mak" CFG="IceExporter - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IceExporter - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcd.lib" /out:"C:\3dsmax4\Plugins\IceExporter.dlu"
# SUBTRACT LINK32 /pdb:none /debug /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy C:\3dsmax4\Plugins\IceExporter.dlu C:\3dsmax5\Plugins\ /Q /Y	xcopy C:\3dsmax4\Plugins\IceExporter.dlu D:\3dsmax5\Plugins\ /Q /Y	xcopy C:\3dsmax4\Plugins\IceExporter.dlu D:\3dsmax6\Plugins\ /Q /Y
# End Special Build Tool
# Begin Target

# Name "IceExporter - Win32 Release"
# Begin Group "API"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FLX_CompilationSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_CompilationSettings.h
# End Source File
# Begin Source File

SOURCE=.\FLX_IceIncludes.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "MAX API"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FLX_Exporter.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_Exporter.h
# End Source File
# Begin Source File

SOURCE=.\FLX_MAXNodes.h
# End Source File
# Begin Source File

SOURCE=.\FLX_Panel.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_Panel.h
# End Source File
# End Group
# Begin Group "Dialog-related"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FLX_About.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_About.h
# End Source File
# Begin Source File

SOURCE=.\FLX_EditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_EditBox.h
# End Source File
# Begin Source File

SOURCE=.\FLX_ExportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_ExportDlg.h
# End Source File
# Begin Source File

SOURCE=.\FLX_Fileselect.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_Fileselect.h
# End Source File
# Begin Source File

SOURCE=.\FLX_HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\FLX_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_Message.h
# End Source File
# Begin Source File

SOURCE=.\FLX_MFC.h
# End Source File
# Begin Source File

SOURCE=.\FLX_MotionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_MotionDlg.h
# End Source File
# Begin Source File

SOURCE=.\IceOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\IceOptions.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\VertexLighterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VertexLighterDlg.h
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FLX_Ask2MAX.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_Ask2MAX.h
# End Source File
# Begin Source File

SOURCE=.\FLX_Controller.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_Controller.h
# End Source File
# Begin Source File

SOURCE=.\FLX_GlobalInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_GlobalInfo.h
# End Source File
# Begin Source File

SOURCE=.\FLX_MatrixConvert.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_MatrixConvert.h
# End Source File
# Begin Source File

SOURCE=.\FLX_MAXMeshPointer.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_MAXMeshPointer.h
# End Source File
# Begin Source File

SOURCE=.\FLX_MAXProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_MAXProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\FLX_TextureCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_TextureCopy.h
# End Source File
# Begin Source File

SOURCE=.\FLX_UserProps.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_UserProps.h
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexColors.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexColors.h
# End Source File
# End Group
# Begin Group "Character Studio"

# PROP Default_Filter ""
# Begin Group "CS SDK"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Bipexp2.h
# End Source File
# Begin Source File

SOURCE=.\Bipexp3.h
# End Source File
# Begin Source File

SOURCE=.\Bipexp4.h
# End Source File
# Begin Source File

SOURCE=.\BIPEXP6.H
# End Source File
# Begin Source File

SOURCE=.\Phyexp2.h
# End Source File
# Begin Source File

SOURCE=.\Phyexp3.h
# End Source File
# Begin Source File

SOURCE=.\Phyexp4.h
# End Source File
# Begin Source File

SOURCE=.\Phyexp6.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\FLX_CharacterStudio.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_CharacterStudio.h
# End Source File
# Begin Source File

SOURCE=.\ISkin3.h
# End Source File
# Begin Source File

SOURCE=.\ISkin4.h
# End Source File
# Begin Source File

SOURCE=.\iskin6.h
# End Source File
# Begin Source File

SOURCE=.\ISkinCodes3.h
# End Source File
# Begin Source File

SOURCE=.\ISkinCodes4.h
# End Source File
# End Group
# Begin Group "Converter"

# PROP Default_Filter ""
# Begin Group "AddMethods"

# PROP Default_Filter ""
# Begin Group "Controllers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FLX_FloatController.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_FOVController.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_MorphController.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_PointController.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_PRSController.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_VisibilityController.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\FLX_AddBPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddController.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddLight.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddMotion.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddPivot.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddPrimitive.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddPRS.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddSafeMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddShape.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddSpaceWarp.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_AddTexture.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\FLX_IK.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_MAXConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_MAXConverter.h
# End Source File
# End Group
# Begin Group "Vertex Lighter"

# PROP Default_Filter ""
# Begin Group "Old"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FLX_Lightmapper.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_Lightmapper.h
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexLighter.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexLighter.h
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexLighterBase.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexLighterBase.h
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexLighterLight.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexLighterLight.h
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexLighterMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexLighterMaterial.h
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexLighterMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexLighterMesh.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\FLX_VertexDataModifier.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexDataModifier.h
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexPrelighter.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_VertexPrelighter.h
# End Source File
# End Group
# Begin Group "Customized"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FLX_CustomAttrib.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_CustomMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_ExtendedMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_ExtendedMaterial.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Flexporter2.bmp
# End Source File
# Begin Source File

SOURCE=.\Flexporter3.bmp
# End Source File
# Begin Source File

SOURCE=.\IceExporter.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\Doxygen.cfg
# End Source File
# Begin Source File

SOURCE=.\FlexporterNotes.txt
# End Source File
# Begin Source File

SOURCE=.\FLX_ZCBImporter.cpp
# End Source File
# Begin Source File

SOURCE=.\FLX_ZCBImporter.h
# End Source File
# Begin Source File

SOURCE=.\NewReleaseMap.txt
# End Source File
# Begin Source File

SOURCE=.\NewResourceMap.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\SDK_Readme.txt
# End Source File
# Begin Source File

SOURCE=.\TODO.txt
# End Source File
# Begin Source File

SOURCE=.\Update.bat
# End Source File
# Begin Source File

SOURCE=.\UsedLibs.txt
# End Source File
# Begin Source File

SOURCE=.\VertexAlpha.txt
# End Source File
# End Target
# End Project
