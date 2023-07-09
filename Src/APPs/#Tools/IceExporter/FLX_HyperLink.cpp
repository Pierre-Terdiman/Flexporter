///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "stdafx.h"

using namespace IceExporter;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOLTIP_ID 1

/////////////////////////////////////////////////////////////////////////////
// FLX_HyperLink

FLX_HyperLink::FLX_HyperLink()
{
	m_hLinkCursor		= NULL;                 // No cursor as yet
	m_crLinkColor		= RGB(  0,   0, 238);   // Blue
	m_crVisitedColor	= RGB( 85,  26, 139);   // Purple
	m_crHoverColor		= ::GetSysColor(COLOR_HIGHLIGHT);
	m_bOverControl		= FALSE;                // Cursor not yet over control
	m_bVisited			= FALSE;                // Hasn't been visited yet.
	m_bUnderline		= TRUE;                 // Underline the link?
	m_bAdjustToFit		= TRUE;                 // Resize the window to fit the text?
	m_strURL.Empty();
}

FLX_HyperLink::~FLX_HyperLink()
{
    m_Font.DeleteObject();
	if (m_hLinkCursor) {
		DestroyCursor(m_hLinkCursor);
	}
}


BEGIN_MESSAGE_MAP(FLX_HyperLink, CStatic)
	//{{AFX_MSG_MAP(FLX_HyperLink)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FLX_HyperLink message handlers

BOOL FLX_HyperLink::PreTranslateMessage(MSG* pMsg) 
{
    m_ToolTip.RelayEvent(pMsg);
    return CStatic::PreTranslateMessage(pMsg);
}

void FLX_HyperLink::PreSubclassWindow() 
{
    // We want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
    
    // Set the URL as the window text
    if (m_strURL.IsEmpty())
        GetWindowText(m_strURL);

    // Check that the window text isn't empty. If it is, set it as the URL.
    CString strWndText;
    GetWindowText(strWndText);
    if (strWndText.IsEmpty()) {
        ASSERT(!m_strURL.IsEmpty());    // Window and URL both NULL. DUH!
        SetWindowText(m_strURL);
    }

    // Create the font
    LOGFONT lf;
    GetFont()->GetLogFont(&lf);
    lf.lfUnderline = (char)m_bUnderline;
    m_Font.CreateFontIndirect(&lf);
    SetFont(&m_Font);

    PositionWindow();        // Adjust size of window to fit URL if necessary
    SetDefaultCursor();      // Try and load up a "hand" cursor

    // Create the tooltip
    CRect rect; 
    GetClientRect(rect);
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(this, m_strURL, rect, TOOLTIP_ID);

    CStatic::PreSubclassWindow();
}

void FLX_HyperLink::OnMouseMove(UINT nFlags, CPoint point) 
{
    CStatic::OnMouseMove(nFlags, point);

    if (m_bOverControl)        // Cursor is currently over control
    {
        CRect rect;
        GetClientRect(rect);

        if (!rect.PtInRect(point))
        {
            m_bOverControl = FALSE;
            ::ReleaseCapture();
            RedrawWindow();
            return;
        }
    }
    else                      // Cursor has just moved over control
    {
        m_bOverControl = TRUE;
        RedrawWindow();
        SetCapture();
    }
}

BOOL FLX_HyperLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if (m_hLinkCursor)
    {
        ::SetCursor(m_hLinkCursor);
        return TRUE;
    }
    return FALSE;
}

HBRUSH FLX_HyperLink::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
#ifdef _DEBUG
	ASSERT(nCtlColor == CTLCOLOR_STATIC);
#else
	UNUSED_ALWAYS( nCtlColor );
#endif
    
    if (m_bOverControl)
        pDC->SetTextColor(m_crHoverColor);
    else if (m_bVisited)
        pDC->SetTextColor(m_crVisitedColor);
    else
        pDC->SetTextColor(m_crLinkColor);

    // transparent text.
    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH)IceCore::GetStockObject(NULL_BRUSH);
}

void FLX_HyperLink::OnClicked() 
{
    int result = (int)GotoURL(m_strURL, SW_SHOW);
    m_bVisited = (result > HINSTANCE_ERROR);
    if (!m_bVisited) {
        MessageBeep(MB_ICONEXCLAMATION);     // Unable to follow link
        ReportError(result);
    } else 
        SetVisited();                        // Repaint to show visited Color
}
















void FLX_HyperLink::SetURL(CString strURL)
{
    m_strURL = strURL;

    if (::IsWindow(GetSafeHwnd())) {
        PositionWindow();
        m_ToolTip.UpdateTipText(strURL, this, TOOLTIP_ID);
    }
}

CString FLX_HyperLink::GetURL() const
{ 
    return m_strURL;   
}

void FLX_HyperLink::SetColors(COLORREF crLinkColor, COLORREF crVisitedColor,
                            COLORREF crHoverColor /* = -1 */) 
{ 
    m_crLinkColor    = crLinkColor; 
    m_crVisitedColor = crVisitedColor;

	if (crHoverColor == -1)
		m_crHoverColor = ::GetSysColor(COLOR_HIGHLIGHT);
	else
		m_crHoverColor = crHoverColor;

    if (::IsWindow(m_hWnd))
        Invalidate(); 
}

COLORREF FLX_HyperLink::GetLinkColor() const
{ 
    return m_crLinkColor; 
}

COLORREF FLX_HyperLink::GetVisitedColor() const
{
    return m_crVisitedColor; 
}

COLORREF FLX_HyperLink::GetHoverColor() const
{
    return m_crHoverColor;
}

void FLX_HyperLink::SetVisited(BOOL bVisited /* = TRUE */) 
{ 
    m_bVisited = bVisited; 

    if (::IsWindow(GetSafeHwnd()))
        Invalidate(); 
}

BOOL FLX_HyperLink::GetVisited() const
{ 
    return m_bVisited; 
}

void FLX_HyperLink::SetLinkCursor(HCURSOR hCursor)
{ 
    m_hLinkCursor = hCursor;
    if (m_hLinkCursor == NULL)
        SetDefaultCursor();
}

HCURSOR FLX_HyperLink::GetLinkCursor() const
{
    return m_hLinkCursor;
}

void FLX_HyperLink::SetUnderline(BOOL bUnderline /* = TRUE */)
{
    m_bUnderline = bUnderline;

    if (::IsWindow(GetSafeHwnd()))
    {
        LOGFONT lf;
        GetFont()->GetLogFont(&lf);
        lf.lfUnderline = (char)m_bUnderline;

        m_Font.DeleteObject();
        m_Font.CreateFontIndirect(&lf);
        SetFont(&m_Font);

        Invalidate(); 
    }
}

BOOL FLX_HyperLink::GetUnderline() const
{ 
    return m_bUnderline; 
}

void FLX_HyperLink::SetAutoSize(BOOL bAutoSize /* = TRUE */)
{
    m_bAdjustToFit = bAutoSize;

    if (::IsWindow(GetSafeHwnd()))
        PositionWindow();
}

BOOL FLX_HyperLink::GetAutoSize() const
{ 
    return m_bAdjustToFit; 
}

void FLX_HyperLink::PositionWindow()
{
    if (!::IsWindow(GetSafeHwnd()) || !m_bAdjustToFit) 
        return;

    // Get the current window position
    CRect rect;
    GetWindowRect(rect);

    CWnd* pParent = GetParent();
    if (pParent)
        pParent->ScreenToClient(rect);

    // Get the size of the window text
    CString strWndText;
    GetWindowText(strWndText);

    CDC* pDC = GetDC();
    CFont* pOldFont = pDC->SelectObject(&m_Font);
    CSize Extent = pDC->GetTextExtent(strWndText);
    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);

    // Get the text justification via the window style
    DWORD dwStyle = GetStyle();

    // Recalc the window size and position based on the text justification
    if (dwStyle & SS_CENTERIMAGE)
        rect.DeflateRect(0, (rect.Height() - Extent.cy)/2);
    else
        rect.bottom = rect.top + Extent.cy;

    if (dwStyle & SS_CENTER)   
        rect.DeflateRect((rect.Width() - Extent.cx)/2, 0);
    else if (dwStyle & SS_RIGHT) 
        rect.left  = rect.right - Extent.cx;
    else // SS_LEFT = 0, so we can't test for it explicitly 
        rect.right = rect.left + Extent.cx;

    // Move the window
    SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
}

/////////////////////////////////////////////////////////////////////////////
// CFOHyperLink implementation

// The following appeared in Paul DiLascia's Jan 1998 MSJ articles.
// It loads a "hand" cursor from the winhlp32.exe module
void FLX_HyperLink::SetDefaultCursor()
{
    if (m_hLinkCursor == NULL)                // No cursor handle - load our own
    {
        // Get the windows directory
        CString strWndDir;
        GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
        strWndDir.ReleaseBuffer();

        strWndDir += _T("\\winhlp32.exe");
        // This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
        HMODULE hModule = LoadLibrary(strWndDir);
        if (hModule) {
            HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
            if (hHandCursor)
                m_hLinkCursor = CopyCursor(hHandCursor);
        }
        FreeLibrary(hModule);
    }
}

LONG FLX_HyperLink::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = ::RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        _tcscpy(retdata,data);
        RegCloseKey(hkey);
    }

    return retval;
}

void FLX_HyperLink::ReportError(int nError)
{
    CString str;
    switch (nError) {
        case 0:                       str = _T("The operating system is out\nof memory or resources."); break;
        case SE_ERR_PNF:              str = _T("The specified path was not found."); break;
        case SE_ERR_FNF:              str = _T("The specified file was not found."); break;
        case ERROR_BAD_FORMAT:        str = _T("The .EXE file is invalid\n(non-Win32 .EXE or error in .EXE image)."); break;
        case SE_ERR_ACCESSDENIED:     str = _T("The operating system denied\naccess to the specified file."); break;
        case SE_ERR_ASSOCINCOMPLETE:  str = _T("The filename association is\nincomplete or invalid."); break;
        case SE_ERR_DDEBUSY:          str = _T("The DDE transaction could not\nbe completed because other DDE transactions\nwere being processed."); break;
        case SE_ERR_DDEFAIL:          str = _T("The DDE transaction failed."); break;
        case SE_ERR_DDETIMEOUT:       str = _T("The DDE transaction could not\nbe completed because the request timed out."); break;
        case SE_ERR_DLLNOTFOUND:      str = _T("The specified dynamic-link library was not found."); break;
        case SE_ERR_NOASSOC:          str = _T("There is no application associated\nwith the given filename extension."); break;
        case SE_ERR_OOM:              str = _T("There was not enough memory to complete the operation."); break;
        case SE_ERR_SHARE:            str = _T("A sharing violation occurred. ");
        default:                      str.Format(_T("Unknown Error (%d) occurred."), nError); break;
    }
    str = _T("Unable to open hyperlink:\n\n") + str;
    AfxMessageBox(str, MB_ICONEXCLAMATION | MB_OK);
}

HINSTANCE FLX_HyperLink::GotoURL( LPCTSTR url, int showcmd )
{
    TCHAR key[MAX_PATH + MAX_PATH];

    // First try ShellExecute()
    HINSTANCE result = ShellExecute( NULL, _T("open"), url, NULL, NULL, showcmd );

    // If it failed, get the .htm regkey and lookup the program
    if((UINT)result <= HINSTANCE_ERROR)
	{
        if( GetRegKey( HKEY_CLASSES_ROOT, _T(".htm"), key ) == ERROR_SUCCESS )
		{
            lstrcat( key, _T("\\shell\\open\\command"));

            if( GetRegKey( HKEY_CLASSES_ROOT, key, key ) == ERROR_SUCCESS )
			{
                LPTSTR pos;

                pos = _tcsstr(key, _T("\"%1\""));

                if( pos == NULL )						// No quotes found
				{
                    pos = _tcsstr( key, _T("%1"));		// Check for %1, without quotes 
                    
					if( pos == NULL )					// No parameter at all...
					{
                        pos = key + _tcslen( key ) - 1;
					}
                    else
					{
                        *pos = _T('\0');				// Remove the parameter
					}
                }
                else
				{
                    *pos = _T('\0');					// Remove the parameter
				}

                lstrcat( pos, _T(" "));
                lstrcat( pos, url );

				PROCESS_INFORMATION pi;
		
				STARTUPINFO			si;

				::ZeroMemory( &si, sizeof( si )); 
		
				si.cb = sizeof( si ); 

				if( ::CreateProcess( NULL,
									 pos,
									 NULL,
									 NULL,
									 FALSE,
									 0,
									 NULL,
									 NULL,
									 &si,
									&pi ) == FALSE )
				{
					return NULL;
				}

                result = (HINSTANCE) pi.hProcess;
            }
        }
    }

    return result;
}

