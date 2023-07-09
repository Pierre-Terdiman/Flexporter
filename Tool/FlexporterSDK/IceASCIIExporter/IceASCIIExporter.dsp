# Microsoft Developer Studio Project File - Name="IceASCIIExporter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ICEASCIIEXPORTER - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IceASCIIExporter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IceASCIIExporter.mak" CFG="ICEASCIIEXPORTER - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IceASCIIExporter - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ICEASCIIEXPORTER_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ICEASCIIEXPORTER_EXPORTS" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 IceCore.lib /nologo /dll /debug /machine:I386 /out:"C:\3dsmax3_1\IceASCIIExporter.flx"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy C:\3dsmax3_1\IceASCIIExporter.flx c:\3dsmax4\ /Q /Y	xcopy C:\3dsmax3_1\IceASCIIExporter.flx c:\3dsmax5\ /Q /Y	xcopy C:\3dsmax3_1\IceASCIIExporter.flx d:\3dsmax5\ /Q /Y
# End Special Build Tool
# Begin Target

# Name "IceASCIIExporter - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Components"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IceASCIIExportCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\IceASCIIExportController.cpp
# End Source File
# Begin Source File

SOURCE=.\IceASCIIExportHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\IceASCIIExportLight.cpp
# End Source File
# Begin Source File

SOURCE=.\IceASCIIExportMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\IceASCIIExportMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\IceASCIIExportMotion.cpp
# End Source File
# Begin Source File

SOURCE=.\IceASCIIExportShape.cpp
# End Source File
# Begin Source File

SOURCE=.\IceASCIIExportSpaceWarp.cpp
# End Source File
# Begin Source File

SOURCE=.\IceASCIIExportTexture.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\IceASCIIExporter.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\IceASCIIExporter.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
