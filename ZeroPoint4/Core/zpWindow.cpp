#include "zpCore.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ShellAPI.h>

#define WINDOW_STYLE        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE
#define WINDOW_PTR_INDEX    GWLP_USERDATA
#define WINDOW_CLASS_NAME    "zpWindowClass"

LRESULT CALLBACK __windowProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
    zpWindow* window = (zpWindow*)GetWindowLongPtr( hWnd, WINDOW_PTR_INDEX );

    if( window )
    {
        window->windowProc( uMessage, wParam, lParam );
    }

    return DefWindowProc( hWnd, uMessage, wParam, lParam );
}

DWORD __zpStyleToWS( zp_uint style )
{
    DWORD windowStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
    if( style & ZP_WINDOW_STYLE_MINIMIZE )
    {
        windowStyle |= WS_MINIMIZEBOX;
    }
    if( style & ZP_WINDOW_STYLE_MAXIMIZE )
    {
        windowStyle |= WS_MAXIMIZEBOX;
    }
    if( style & ZP_WINDOW_STYLE_NO_RESIZE )
    {
        windowStyle |= WS_BORDER;
    }
    if( style & ZP_WINDOW_STYLE_RESIZE )
    {
        windowStyle |= WS_THICKFRAME;
    }
    if( style & ZP_WINDOW_STYLE_BORDERLESS )
    {
        windowStyle &= ~( WS_CAPTION | WS_SYSMENU );
        windowStyle |= WS_POPUP;
    }

    return windowStyle;
}

zpWindow::zpWindow()
    : m_position()
    , m_screenSize( 640, 480 )
    , m_windowSize()
    , m_style( ZP_WINDOW_STYLE_DEFAULT )
    , m_hWnd( ZP_NULL )
    , m_hInstance( ZP_NULL )
    , m_title( "" )
{}
zpWindow::~zpWindow()
{}

void zpWindow::setTitle( const zpString& title )
{
    m_title = title;
    if( m_hWnd ) SetWindowText( (HWND)m_hWnd, m_title.str() );
}
void zpWindow::setTitle( const zp_char* title )
{
    m_title = title;
    if( m_hWnd ) SetWindowText( (HWND)m_hWnd, m_title.str() );
}

const zpString& zpWindow::getTitle() const
{
    return m_title;
}

void zpWindow::setPosition( const zpVector2i& position )
{
    m_position = position;
    moveResize();
}
const zpVector2i& zpWindow::getPosition() const
{
    return m_position;
}

void zpWindow::setScreenSize( const zpVector2i& size )
{
    m_screenSize = size;
    resizeWindow();
    moveResize();
}
const zpVector2i& zpWindow::getScreenSize() const
{
    return m_screenSize;
}

const zpVector2i& zpWindow::getWindowSize() const
{
    return m_windowSize;
}

void zpWindow::setStyle( zp_uint style )
{
    m_style = style;
    if( m_hWnd )
    {
        SetWindowLong( (HWND)m_hWnd, GWL_STYLE, __zpStyleToWS( m_style ) );
    }
}
zp_uint zpWindow::getStyle() const
{
    return m_style;
}

zp_handle zpWindow::getWindowHandle() const
{
    return m_hWnd;
}
zp_handle zpWindow::getInstanceHandle() const
{
    return m_hInstance;
}

void zpWindow::create()
{
    if( m_hWnd ) return;
    
    m_hInstance = (zp_handle)GetModuleHandle( NULL );

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc = __windowProc;
    wc.hInstance = (HINSTANCE)m_hInstance;
    wc.hbrBackground = (HBRUSH)GetStockObject( LTGRAY_BRUSH );
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIcon = LoadIcon( NULL, MAKEINTRESOURCE( IDI_APPLICATION ) );
    wc.hIconSm = (HICON)LoadImage( NULL, MAKEINTRESOURCE( IDI_APPLICATION ), IMAGE_ICON, 16, 16, 0 );
    
    if( !RegisterClassEx( &wc ) )
    {
        ZP_ASSERT( false, "Unable to register window class" );
        return;
    }

    resizeWindow();

    m_hWnd = (zp_handle)CreateWindowEx(
        WS_EX_ACCEPTFILES,
        WINDOW_CLASS_NAME,
        m_title.str(),
        __zpStyleToWS( m_style ),
        m_position.getX(),
        m_position.getY(),
        m_windowSize.getX(),
        m_windowSize.getY(),
        NULL,
        NULL,
        (HINSTANCE)m_hInstance,
        NULL
    );

    if( !m_hWnd )
    {
        ZP_ASSERT( false, "Unable to create window. Error #%X", GetLastError() );
        return;
    }

    SetWindowLongPtr( (HWND)m_hWnd, WINDOW_PTR_INDEX, (LONG)this );

    ShowWindow( (HWND)m_hWnd,  SW_SHOW );
    UpdateWindow( (HWND)m_hWnd );
}
void zpWindow::destroy()
{
    if( !m_hWnd ) return;

    DestroyWindow( (HWND)m_hWnd );
    UnregisterClass(  WINDOW_CLASS_NAME, (HINSTANCE)m_hInstance );

    m_hInstance = ZP_NULL;
    m_hWnd = ZP_NULL;

    removeAllFocusListeners();
    removeAllProcListeners();
}

zp_bool zpWindow::isCreated() const
{
    return m_hWnd != ZP_NULL;
}

zp_bool zpWindow::processMessages()
{
    MSG message;
    if( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) ) 
    {
        if( message.message == WM_QUIT )
        {
            return false;
        }

        TranslateMessage( &message );
        DispatchMessage( &message );
    }

    return true;
}

void zpWindow::windowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam )
{
    switch( uMessage ) {
    case WM_CLOSE:
    case WM_QUIT:
        {
            PostQuitMessage( 0 );
        }
        break;
    case WM_KILLFOCUS:
        {
            m_focusListeners.foreach( []( zpWindowFocusListener* listener ) {
                listener->onFocusLost();
            });
        }
        break;
    case WM_SETFOCUS:
        {
            m_focusListeners.foreach( []( zpWindowFocusListener* listener ) {
                listener->onFocusGained();
            });
        }
        break;
    case WM_MOVE:
        {
            m_position.set( LOWORD( lParam ), HIWORD( lParam ) );
        }
        break;
    case WM_SETTEXT:
        {
            if( lParam )
            {
                setTitle( (zp_char*)lParam );
            }
        }
        break;
    case WM_STYLECHANGED:
        {
            resizeWindow();
            moveResize();
        }
        break;
    case WM_DROPFILES:
        {
            HDROP drop = (HDROP)wParam;
            
            POINT pnt;
            if( DragQueryPoint( drop, &pnt ) )
            {
                zp_uint dropCount = DragQueryFile( drop, 0xFFFFFFFF, ZP_NULL, 0 );
                if( dropCount > 0 )
                {
                    zp_uint len;
                    zp_char buff[ 256 ];
                    for( zp_uint i = 0; i < dropCount; ++i )
                    {
                        len = DragQueryFile( drop, i, buff, 256 );
                        buff[ len ] = '\0';

                        m_dragDropListeners.foreach( [ &pnt, &buff ]( zpWindowDragDropListener* listener )
                        {
                            listener->onDragDrop( buff, pnt.x, pnt.y );
                        } );
                    }
                }
            }
        }
        break;
    default:
        {
            m_procListeners.foreach( [ &uMessage, &wParam, &lParam ]( zpWindowProcListener* listener ) {
                listener->onWindowProc( uMessage, wParam, lParam );
            });
        }
        break;
    }
}

void zpWindow::addFocusListener( zpWindowFocusListener* listener )
{
    m_focusListeners.pushBack( listener );
}
void zpWindow::removeFocusListener( zpWindowFocusListener* listener )
{
    m_focusListeners.eraseAll( listener );
}
void zpWindow::removeAllFocusListeners()
{
    m_focusListeners.clear();
}

void zpWindow::addProcListener( zpWindowProcListener* listener )
{
    m_procListeners.pushBack( listener );
}
void zpWindow::removeProcListener( zpWindowProcListener* listener )
{
    m_procListeners.eraseAll( listener );
}
void zpWindow::removeAllProcListeners()
{
    m_procListeners.clear();
}

void zpWindow::addDragDropListener( zpWindowDragDropListener* listener )
{
    m_dragDropListeners.pushBack( listener );
}
void zpWindow::removeDragDropListener( zpWindowDragDropListener* listener )
{
    m_dragDropListeners.eraseAll( listener );
}
void zpWindow::removeAllDragDropListeners()
{
    m_dragDropListeners.clear();
}

void zpWindow::moveResize()
{
    if( m_hWnd )
    {
        MoveWindow( (HWND)m_hWnd, m_position.getX(), m_position.getY(), m_windowSize.getX(), m_windowSize.getY(), true );
    }
}
void zpWindow::resizeWindow()
{
    RECT rc = { 0, 0, m_screenSize.getX(), m_screenSize.getY() };
    AdjustWindowRectEx( &rc, __zpStyleToWS( m_style ), false, 0 );
    m_windowSize.set( rc.right - rc.left, rc.bottom - rc.top );
}
