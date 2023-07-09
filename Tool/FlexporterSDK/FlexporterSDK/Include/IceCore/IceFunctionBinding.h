///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code to lazy-bind system functions on-the-fly.
 *	\file		IceFunctionBinding.h
 *	\author		Pierre Terdiman
 *	\date		February, 3, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEFUNCTIONBINDING_H__
#define __ICEFUNCTIONBINDING_H__

	FUNCTION ICECORE_API HWND		CreateWindowEx(DWORD ex_style, LPCSTR class_name, LPCSTR window_name, DWORD style, int x, int y, int width, int height, HWND wnd_parent, HMENU menu, HINSTANCE hinstance, LPVOID param);
	FUNCTION ICECORE_API ATOM		RegisterClassEx(CONST WNDCLASSEXA* class_data);
	FUNCTION ICECORE_API BOOL		UnregisterClass(LPCSTR class_name, HINSTANCE instance);
	FUNCTION ICECORE_API int		GetDeviceCaps(HDC hdc, int index);
	FUNCTION ICECORE_API BOOL		ExtTextOut(HDC hdc, int x, int y, UINT options, CONST RECT* rect, LPCSTR str, UINT count, CONST INT* dx);
	FUNCTION ICECORE_API int		SetBkMode(HDC hdc, int mode);
	FUNCTION ICECORE_API COLORREF	SetTextColor(HDC hdc, COLORREF color);
	FUNCTION ICECORE_API BOOL		SetPixelFormat(HDC hdc, int pf, CONST PIXELFORMATDESCRIPTOR* ppfd);
	FUNCTION ICECORE_API int		ChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR* ppfd);
	FUNCTION ICECORE_API int		DescribePixelFormat(HDC hdc, int pixel_format, UINT bytes, LPPIXELFORMATDESCRIPTOR ppfd);
	FUNCTION ICECORE_API BOOL		SwapBuffers(HDC hdc);
	FUNCTION ICECORE_API int		SetDIBitsToDevice(HDC hdc, int x_dest, int y_dest, DWORD width, DWORD height, int x_src, int y_src, UINT start_scan, UINT scanlines, CONST VOID* bits, CONST BITMAPINFO* bmi, UINT color_use);
	FUNCTION ICECORE_API HGDIOBJ	SelectObject(HDC hdc, HGDIOBJ hgdiobj);
	FUNCTION ICECORE_API HBITMAP	CreateDIBSection(HDC hdc, CONST BITMAPINFO* pbmi, UINT usage, VOID** bit_values, HANDLE section, DWORD offset);
	FUNCTION ICECORE_API HDC		CreateCompatibleDC(HDC hdc);
	FUNCTION ICECORE_API HFONT		CreateFont(int height, int width, int escapement, int orientation, int weight, DWORD italic, DWORD underline, DWORD strike_out, DWORD char_set, DWORD output_precision, DWORD clip_precision, DWORD quality, DWORD pitch_and_family, LPCTSTR face);
	FUNCTION ICECORE_API BOOL		DeleteObject(HGDIOBJ hgdiobj);
	FUNCTION ICECORE_API udword		TimeGetTime();
	FUNCTION ICECORE_API SHORT		GetKeyState(int key);
	FUNCTION ICECORE_API SHORT		GetAsyncKeyState(int key);
	FUNCTION ICECORE_API BOOL		GetKeyboardState(PBYTE key_state);
	FUNCTION ICECORE_API HDC		GetDeviceContext(HWND hwnd);
	FUNCTION ICECORE_API int		ReleaseDeviceContext(HWND hwnd, HDC dc);
	FUNCTION ICECORE_API BOOL		GetWindowRect(HWND hwnd, LPRECT rect);
	FUNCTION ICECORE_API BOOL		SetRect(LPRECT rect, int x_left, int y_top, int x_right, int y_bottom);
	FUNCTION ICECORE_API BOOL		ClientToScreen(HWND hwnd, LPPOINT point);
	FUNCTION ICECORE_API BOOL		GetClientRect(HWND hwnd, LPRECT rect);
	FUNCTION ICECORE_API BOOL		ValidateRect(HWND hwnd, CONST RECT* rect);
	FUNCTION ICECORE_API BOOL		AdjustWindowRect(LPRECT rect, DWORD style, BOOL menu);
	FUNCTION ICECORE_API void		PostQuitMessage(int exitcode);
	FUNCTION ICECORE_API BOOL		PostMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	FUNCTION ICECORE_API BOOL		DestroyWindow(HWND hwnd);
	FUNCTION ICECORE_API HWND		SetFocus(HWND hwnd);
	FUNCTION ICECORE_API BOOL		UpdateWindow(HWND hwnd);
	FUNCTION ICECORE_API BOOL		ShowWindow(HWND hwnd, int cmdshow);
	FUNCTION ICECORE_API LRESULT	DefWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	FUNCTION ICECORE_API int		GetSystemMetrics(int index);
	FUNCTION ICECORE_API BOOL		TranslateMessage(CONST MSG* msg);
	FUNCTION ICECORE_API BOOL		PeekMessage(LPMSG msg, HWND hwnd, UINT msg_filter_min, UINT msg_filter_max, UINT remove_msg);
	FUNCTION ICECORE_API LONG		DispatchMessage(CONST MSG* msg);
	FUNCTION ICECORE_API LRESULT	SendMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	FUNCTION ICECORE_API LONG		SetWindowLong(HWND hwnd, int index, LONG new_long);
	FUNCTION ICECORE_API LONG		GetWindowLong(HWND hwnd, int index);
	FUNCTION ICECORE_API BOOL		SetWindowPos(HWND hwnd, HWND hwnd_insert_after, int x, int y, int cx, int cy, UINT flags);
	FUNCTION ICECORE_API HWND		CreateDialog_(HINSTANCE hinstance, LPCSTR template_name, HWND hwnd_parent, DLGPROC dialog_func);
	FUNCTION ICECORE_API HWND		GetDlgItem(HWND dlg, int dlg_item);
	FUNCTION ICECORE_API UINT		GetDlgItemInt(HWND dlg, int dlg_item, BOOL* translated, BOOL is_signed);
	FUNCTION ICECORE_API BOOL		SetDlgItemInt(HWND dlg, int dlg_item, UINT value, BOOL is_signed);
	FUNCTION ICECORE_API BOOL		SetDlgItemText(HWND dlg, int dlg_item, LPCSTR string);
	FUNCTION ICECORE_API UINT		GetDlgItemText(HWND dlg, int dlg_item, LPSTR string, int max_count);
	FUNCTION ICECORE_API int		DialogBox_(HINSTANCE hinstance, LPCSTR template_name, HWND hwnd_parent, DLGPROC dialog_func, LPARAM init_param);
	FUNCTION ICECORE_API BOOL		EndDialog(HWND dlg, int result);
	FUNCTION ICECORE_API BOOL		CheckDlgButton(HWND dlg, int button, UINT check);
	FUNCTION ICECORE_API BOOL		CheckRadioButton(HWND dlg, int first_button, int last_button, int check_button);
	FUNCTION ICECORE_API UINT		IsDlgButtonChecked(HWND dlg, int button);
	FUNCTION ICECORE_API int		SetScrollPos(HWND hwnd, int bar, int pos, BOOL redraw);
	FUNCTION ICECORE_API int		GetScrollPos(HWND hwnd, int bar);
	FUNCTION ICECORE_API BOOL		SetScrollRange(HWND hwnd, int bar, int min_pos, int max_pos, BOOL redraw);
	FUNCTION ICECORE_API BOOL		GetScrollRange(HWND hwnd, int bar, LPINT min_pos, LPINT max_pos);
	FUNCTION ICECORE_API BOOL		GetScrollInfo(HWND hwnd, int bar, LPSCROLLINFO scroll_info);
	FUNCTION ICECORE_API BOOL		ScreenToClient(HWND hwnd, LPPOINT point);
	FUNCTION ICECORE_API int		ShowCursor(BOOL show);
	FUNCTION ICECORE_API BOOL		SetCursorPos(int x, int y);
	FUNCTION ICECORE_API BOOL		GetCursorPos(LPPOINT pt);
	FUNCTION ICECORE_API HCURSOR	SetCursor(HCURSOR cursor);
	FUNCTION ICECORE_API HCURSOR	LoadCursor(HINSTANCE hinstance, LPCSTR cursor_name);
	FUNCTION ICECORE_API HWND		SetCapture(HWND hwnd);
	FUNCTION ICECORE_API BOOL		ReleaseCapture();
	FUNCTION ICECORE_API BOOL		DestroyMenu(HMENU menu);
	FUNCTION ICECORE_API HMENU		GetMenu(HWND hwnd);
	FUNCTION ICECORE_API BOOL		EnumDisplaySettings(LPCSTR devicename, DWORD modenum, LPDEVMODEA devmode);
	FUNCTION ICECORE_API LONG		ChangeDisplaySettings(LPDEVMODEA devmode, DWORD flags);
	FUNCTION ICECORE_API int		MessageBox(HWND hwnd, LPCSTR text, LPCSTR caption, UINT type);
	FUNCTION ICECORE_API LONG		RegOpenKeyEx(HKEY key, LPCSTR subkey, DWORD options, REGSAM desired, PHKEY result);
	FUNCTION ICECORE_API LONG		RegCreateKeyEx(HKEY key, LPCSTR subkey, DWORD reserved, LPSTR lpclass, DWORD options, REGSAM desired, LPSECURITY_ATTRIBUTES attr, PHKEY result, LPDWORD disp);
	FUNCTION ICECORE_API LONG		RegSetValueEx(HKEY key, LPCSTR valuename, DWORD reserved, DWORD type, CONST BYTE* data, DWORD cbdata);
	FUNCTION ICECORE_API LONG		RegEnumKeyEx(HKEY key, DWORD index, LPSTR name, LPDWORD cbname, LPDWORD reserved, LPSTR lpclass, LPDWORD cbclass, PFILETIME lastwritetime);
	FUNCTION ICECORE_API LONG		RegDeleteKey(HKEY key, LPCSTR subkey);
	FUNCTION ICECORE_API LONG		RegDeleteValue(HKEY key, LPCSTR valuename);
	FUNCTION ICECORE_API LONG		RegQueryValueEx(HKEY key, LPCSTR valuename, LPDWORD reserved, LPDWORD type, LPBYTE data, LPDWORD cbdata);

	// AVI player
	FUNCTION ICECORE_API HRESULT	AVIStreamOpenFromFile(PAVISTREAM FAR* avi, LPCSTR file, DWORD type, LONG param, UINT mode, CLSID FAR* handler);
	FUNCTION ICECORE_API PGETFRAME	AVIStreamGetFrameOpen(PAVISTREAM avi, LPBITMAPINFOHEADER wanted);
	FUNCTION ICECORE_API HRESULT	AVIStreamGetFrameClose(PGETFRAME pg);
	FUNCTION ICECORE_API LPVOID		AVIStreamGetFrame(PGETFRAME pg, LONG pos);
	FUNCTION ICECORE_API LONG		AVIStreamLength(PAVISTREAM avi);
	FUNCTION ICECORE_API LONG		AVIStreamSampleToTime(PAVISTREAM avi, LONG sample);
	FUNCTION ICECORE_API ULONG		AVIStreamRelease(PAVISTREAM avi);
	FUNCTION ICECORE_API HRESULT	AVIStreamInfo(PAVISTREAM avi, LPAVISTREAMINFOA psi, LONG size);
	FUNCTION ICECORE_API void		AVIFileInit();
	FUNCTION ICECORE_API void		AVIFileExit();
	FUNCTION ICECORE_API HDRAWDIB	DrawDibOpen();
	FUNCTION ICECORE_API BOOL		DrawDibDraw(HDRAWDIB hdd, HDC hdc, int xDst, int yDst, int dxDst, int dyDst, LPBITMAPINFOHEADER lpbi, LPVOID lpBits, int xSrc, int ySrc, int dxSrc, int dySrc, UINT wFlags);
	FUNCTION ICECORE_API BOOL		DrawDibClose(HDRAWDIB hdd);



	// DEBUG ONLY - DON'T USE
	FUNCTION ICECORE_API bool		BindFunctions();
	FUNCTION ICECORE_API bool		UnbindFunctions();


	struct ICECORE_API ScreenPoint
	{
		sdword	x;
		sdword	y;
	};

	struct ICECORE_API ScreenRect
	{
		void Reset()
		{
			mLeft	= 0;
			mTop	= 0;
			mRight	= 0;
			mBottom	= 0;
		}

		bool IsEmpty()	const
		{
			return ((mLeft==mRight) || (mBottom==mTop));
		}

		void Reorganize()
		{
			if(mLeft>mRight)	TSwap(mLeft, mRight);
			if(mTop>mBottom)	TSwap(mTop, mBottom);
		}

		sdword	mLeft;
		sdword	mTop;
		sdword	mRight;
		sdword	mBottom;
	};

	// Window stuff
	FUNCTION ICECORE_API void	GetWindowCenter(HWND handle, udword render_width, udword render_height, ScreenPoint& pt);

	#define CHECKPOINT(text)	IceCore::MessageBox(null, text, "CheckPoint", MB_OK);

#endif // __ICEFUNCTIONBINDING_H__

