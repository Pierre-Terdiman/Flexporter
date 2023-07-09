
echo off

rem Copy readme files
rem Source directory should be Y:\APPs\IceExporter
copy readme.txt C:\Code\CoderCornerRsc\Code\Flexporter
copy SDK_Readme.txt C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK
copy .\Novodexporter\Readme.txt C:\Code\CoderCornerRsc\Code\Flexporter\dll\FlexporterDLL

rem Copy format plug-ins
copy c:\3dsmax4\IceASCIIExporter.flx C:\Code\CoderCornerRsc\Code\Flexporter\dll\FlexporterDLL
copy c:\3dsmax4\IceZCBExporter.flx C:\Code\CoderCornerRsc\Code\Flexporter\dll\FlexporterDLL
copy c:\3dsmax4\VoidExporter.flx C:\Code\CoderCornerRsc\Code\Flexporter\dll\FlexporterDLL
copy c:\3dsmax4\Novodexporter.flx C:\Code\CoderCornerRsc\Code\Flexporter\dll\FlexporterDLL

rem Copy ICE dlls
copy y:\Dll\IceCore.dll C:\Code\CoderCornerRsc\Code\Flexporter\dll\IceDLL
copy y:\Dll\IceMaths.dll C:\Code\CoderCornerRsc\Code\Flexporter\dll\IceDLL
copy y:\Dll\IceImageWork.dll C:\Code\CoderCornerRsc\Code\Flexporter\dll\IceDLL
copy y:\Dll\Meshmerizer.dll C:\Code\CoderCornerRsc\Code\Flexporter\dll\IceDLL
copy y:\Dll\Opcode.dll C:\Code\CoderCornerRsc\Code\Flexporter\dll\IceDLL
copy y:\Dll\ZCollide.dll C:\Code\CoderCornerRsc\Code\Flexporter\dll\IceDLL
copy y:\Dll\IcePrelighter.dll C:\Code\CoderCornerRsc\Code\Flexporter\dll\IceDLL
copy y:\Dll\IceRenderer.dll C:\Code\CoderCornerRsc\Code\Flexporter\dll\IceDLL
copy y:\Dll\libbz2.dll C:\Code\CoderCornerRsc\Code\Flexporter\dll\IceDLL
copy y:\Dll\ZLib.dll C:\Code\CoderCornerRsc\Code\Flexporter\dll\IceDLL

rem Copy format source code
copy Y:\APPs\IceExporter\IceASCIIExporter\*.* C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\IceASCIIExporter
copy Y:\APPs\IceExporter\IceZCBExporter\*.* C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\IceZCBExporter
copy Y:\APPs\IceExporter\VoidExporter\*.* C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\VoidExporter

rem Copy libs
copy Y:\Lib\IceCore.lib C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\Lib
copy Y:\Lib\libbz2.lib C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\Lib
copy Y:\Lib\ZLib.lib C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\Lib

rem Copy ZCB help
copy Y:\APIs\Ice\IceRenderManager\IceZCBBreaker.cpp C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\ZCB_Help
copy Y:\APIs\Ice\IceRenderManager\IceZCBBreaker.h C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\ZCB_Help
copy Y:\APIs\Ice\IceCore\IceBuffer.cpp C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\ZCB_Help
copy Y:\APPs\IceExporter\Extra\ZCB_Help\*.* C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\ZCB_Help

copy Y:\APIs\Ice\Flexporter\SDK\*.* C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\Include
copy Y:\APIs\External\BZip2\Src\*.h C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\Include\BZip2
copy Y:\APIs\External\ZLib\*.h C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\Include\ZLib

copy Y:\APIs\Ice\#Plugins\FlexineSDK\FlexineSDK.h C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\Include\IceCore
copy Y:\APIs\Ice\IceCore\*.h C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\Include\IceCore

copy Y:\APPs\IceExporter\Extra\TestZCBBreaker C:\Code\CoderCornerRsc\Code\Flexporter\FlexporterSDK\FlexporterSDK\TestZCBBreaker

