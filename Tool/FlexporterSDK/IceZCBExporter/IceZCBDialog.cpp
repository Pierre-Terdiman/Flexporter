///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains dialog code for the ZCB format.
 *	\file		IceZCBDialog.cpp
 *	\author		Pierre Terdiman
 *	\date		May, 27, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"
#include <commctrl.h>

static HANDLE gModule = null;

BOOL APIENTRY DllMain(HANDLE hModule, udword ul_reason_for_call, void* lpReserved)
{
	// We don't really need to implement DllMain, but it's an easy way to get the module handle back.
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			gModule = hModule;
			break;
	}

	// This returns the same as what we already have in gModule. We choose to use
	// the other method anyway since this one depends on the module's name:
	//
	// HMODULE HMod = GetModuleHandle("IceZCBExporter.flx");

	return true;
}

static ZCBFormat* gFormat = null;

/*
  virtual int value()
    {
      return SendMessage( controlWindow_->window()->hWnd(), TBM_GETPOS, 0, 0 );
    }
  virtual void setValue( int v )
    {
      LRESULT lr = SendMessage( controlWindow_->window()->hWnd(), TBM_SETPOS, TRUE, v );
    }
*/

// Slider value between 0 and 100 *included*, i.e. there are 101 possible values
// We map this to 1 <= nb_bits <= 15
static udword SliderToBits(udword slider_value)
{
							// 0 <= slider_value <= 100
	slider_value *= 14;		// 1 <= slider_value <= 1400
	slider_value /= 100;	// 0 <= slider_value <= 14
	slider_value++;			// 1 <= slider_value <= 15
	return slider_value;
}

static udword BitsToSlider(udword bits_value)
{
						// 1 <= bits_value <= 15
	bits_value--;		// 0 <= bits_value <= 14
	bits_value *=100;	// 0 <= bits_value <= 1400
	bits_value /=14;	// 0 <= bits_value <= 100
	return bits_value;
}

static BOOL CALLBACK ZCBWindowProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
    switch(message) 
    { 
		case WM_INITDIALOG:
		{
			gFormat->LoadOptions();

/*			int Min = 0;
			int Max = 100;
			if(!IceCore::SetScrollRange(IceCore::GetDlgItem(hwndDlg, IDC_QUALITY), SB_CTL, Min, Max, FALSE))
			{
				IceCore::MessageBox(null, "SetScrollRange failed", "Oops", MB_OK);
			}
//			IceCore::GetScrollRange(IceCore::GetDlgItem(hwndDlg, IDC_QUALITY), SB_CTL, &Min, &Max);
*/
			int Current = IDC_COMPRESS_NONE;	// Default
			ZCBCompression Compress = gFormat->GetCompression();
					if(Compress==ZCB_COMPRESSION_ZLIB)	Current = IDC_COMPRESS_ZLIB;
			else	if(Compress==ZCB_COMPRESSION_BZIP2)	Current = IDC_COMPRESS_BZIP2;
			IceCore::CheckRadioButton(hwndDlg, IDC_COMPRESS_NONE, IDC_COMPRESS_BZIP2, Current);

//			IceCore::SetDlgItemInt(hwndDlg, IDC_NB_BITS, gFormat->GetNbBits(), TRUE);

			IceCore::SendMessage(IceCore::GetDlgItem(hwndDlg, IDC_SLIDER1), TBM_SETPOS, TRUE, BitsToSlider(gFormat->GetNbBits()));

			IceCore::SetDlgItemText(hwndDlg, IDC_NBBITS, _F("%d bits", gFormat->GetNbBits()));

			return TRUE;
		}

		case WM_HSCROLL:
		{
			udword NbBits = SliderToBits(IceCore::SendMessage(IceCore::GetDlgItem(hwndDlg, IDC_SLIDER1), TBM_GETPOS, 0, 0));
			IceCore::SetDlgItemText(hwndDlg, IDC_NBBITS, _F("%d bits", NbBits));
/*			switch(LOWORD(wParam))
			{
				case SB_THUMBTRACK:
				{
					// Initialize SCROLLINFO structure
					SCROLLINFO si;
					ZeroMemory(&si, sizeof(si));
					si.cbSize = sizeof(si);
					si.fMask = SIF_TRACKPOS;

					// Call GetScrollInfo to get current tracking  position in si.nTrackPos
					if(!IceCore::GetScrollInfo(hwndDlg, SB_HORZ, &si))
						return 1; // GetScrollInfo failed

				IceCore::MessageBox(null, _F("%d", si.nTrackPos), "Truc", MB_OK);

					break;
				}
			}*/
		}
		break;

		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{

//BOOL Status;
//UINT Quality = IceCore::GetDlgItemInt(hwndDlg, IDC_NB_BITS, &Status, TRUE);
//IceCore::MessageBox(null, _F("%d", Quality), "Truc", MB_OK);

//HWND hh = IceCore::GetDlgItem(hwndDlg, IDC_QUALITY);

//char Text[1024];
//IceCore::GetDlgItemText(hwndDlg, IDC_QUALITY, Text, 1024);
//IceCore::MessageBox(null, Text, "Truc", MB_OK);
/*
				int Quality = IceCore::GetScrollPos(IceCore::GetDlgItem(hwndDlg, IDC_QUALITY), SB_CTL);
//				int Quality = IceCore::GetScrollPos(hwndDlg, SB_CTL);
				IceCore::MessageBox(null, _F("%d", Quality), "Truc", MB_OK);
*/



//char Text[1024];
//IceCore::GetDlgItemText(hwndDlg, IDC_COMBO2, Text, 1024);
//IceCore::MessageBox(null, Text, "Truc", MB_OK);

/*
HWND hh = IceCore::GetDlgItem(hwndDlg, IDC_SLIDER1);
int Quality = IceCore::SendMessage(hh, TBM_GETPOS, 0, 0);
IceCore::MessageBox(null, _F("%d", Quality), "Truc", MB_OK);
*/

				if(LOWORD(wParam) == IDOK)
				{
					ZCBCompression Compress;
							if(IceCore::IsDlgButtonChecked(hwndDlg, IDC_COMPRESS_ZLIB))		Compress = ZCB_COMPRESSION_ZLIB;
					else	if(IceCore::IsDlgButtonChecked(hwndDlg, IDC_COMPRESS_BZIP2))	Compress = ZCB_COMPRESSION_BZIP2;
					else																	Compress = ZCB_COMPRESSION_NONE;

					gFormat->SetCompression(Compress);

//					BOOL Status;
//					UINT NbBits = IceCore::GetDlgItemInt(hwndDlg, IDC_NB_BITS, &Status, TRUE);
//					if(Status)	gFormat->SetNbBits(NbBits);

					udword NbBits = SliderToBits(IceCore::SendMessage(IceCore::GetDlgItem(hwndDlg, IDC_SLIDER1), TBM_GETPOS, 0, 0));
					gFormat->SetNbBits(NbBits);
//					IceCore::MessageBox(null, _F("%d", NbBits), "Truc", MB_OK);

					gFormat->SaveOptions();
				}
				IceCore::EndDialog(hwndDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
    } 
    return FALSE; 
} 

// This method is called by Flexporter
void ZCBFormat::About()
{
	gFormat = this;
	IceCore::DialogBox_((HINSTANCE)gModule, MAKEINTRESOURCE(IDD_ZCB), null, ZCBWindowProc, 0);
}

bool ZCBFormat::SaveOptions()
{
	// Save to registry
	Save("Flexporter", "ZCBFormat", "Compression",	(udword)mCompression);
	Save("Flexporter", "ZCBFormat", "NbBits",		mNbBits);

	return true;
}

bool ZCBFormat::LoadOptions()
{
	// Load from registry
	udword Value;
	if(Load("Flexporter", "ZCBFormat", "Compression", Value))	mCompression = (ZCBCompression)Value;
	if(Load("Flexporter", "ZCBFormat", "NbBits", Value))		mNbBits = Value;
	return true;
}
