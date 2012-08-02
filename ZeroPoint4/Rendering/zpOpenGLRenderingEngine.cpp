#include "zpRendering.h"
#include "zpOpenGL.h"

zpOpenGLRenderingEngine::zpOpenGLRenderingEngine() {}
zpOpenGLRenderingEngine::~zpOpenGLRenderingEngine() {}

zp_bool zpOpenGLRenderingEngine::create() {
	HWND hwnd = (HWND)m_window->getWindowHandle();

	HDC hdc = GetDC( hwnd );

	PIXELFORMATDESCRIPTOR pfd;
	zp_zero_memory( &pfd );
	
	pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	// choose a pixel format
	zp_int pixelFormatIndex = ChoosePixelFormat( hdc, &pfd );
	if( pixelFormatIndex == 0 ) return false;

	// set the pixel format
	zp_bool formatSet = SetPixelFormat( hdc, pixelFormatIndex, &pfd ) == 1;
	if( !formatSet ) return false;

	// create an OpenGL context
	HGLRC context = wglCreateContext( hdc );
	wglMakeCurrent( hdc, context );

	// initialize GLEW
	GLenum gle = glewInit();
	if( gle != GLEW_OK ) {
		zp_printfln( "Unable to initialize GLEW" );
		return false;
	}

	if( WGLEW_ARB_create_context ) {
		zp_int attr[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,	3,
			WGL_CONTEXT_MINOR_VERSION_ARB,	1,
#if ZP_DEBUG
			WGL_CONTEXT_FLAGS_ARB,			WGL_CONTEXT_DEBUG_BIT_ARB,
#else
			WGL_CONTEXT_FLAGS_ARB,			0,
#endif
			0
		};
		
		HGLRC rc = wglCreateContextAttribsARB( hdc, 0, attr );
		wglMakeCurrent( ZP_NULL, ZP_NULL );
		wglDeleteContext( context );
		wglMakeCurrent( hdc, rc );
		context = rc;
	}
	const zp_byte* vv = glGetString( GL_VERSION );

	m_immediateContext = new zpOpenGLRenderingContext( context, hdc, "immediate" );

	return true;
}
void zpOpenGLRenderingEngine::destroy() {}

zpRenderingEngineType zpOpenGLRenderingEngine::getEngineType() const {
	return m_engineType;
}

zp_uint zpOpenGLRenderingEngine::enumerateDisplayModes( zpDisplayFormat displayFormat, zpArrayList<zpDisplayMode>* outDisplayModes ) { return 0; }
zp_bool zpOpenGLRenderingEngine::findClosestDisplayMode( const zpDisplayMode& displayMode, zpDisplayMode* outDisplayMode ) { return false; }

void zpOpenGLRenderingEngine::setDisplayMode( const zpDisplayMode& mode ) {
	m_displayMode = mode;
}
const zpDisplayMode& zpOpenGLRenderingEngine::getDisplayMode() const {
	return m_displayMode;
}

void zpOpenGLRenderingEngine::setScreenMode( zpScreenMode mode ) {
	m_screenMode = mode;
}
zpScreenMode zpOpenGLRenderingEngine::getScreenMode() const { 
	return m_screenMode;
}

void zpOpenGLRenderingEngine::setWindow( zpWindow* window ) {
	m_window = window;
}
zpWindow* zpOpenGLRenderingEngine::getWindow() const {
	return m_window;
}

void zpOpenGLRenderingEngine::setVSyncEnabled( zp_bool enabled ) {
	m_vsyncEnabled = enabled;
}

void zpOpenGLRenderingEngine::present() {}

zpRenderingContext* zpOpenGLRenderingEngine::createRenderingContext( const zpString& name ) { return ZP_NULL; }
zp_bool zpOpenGLRenderingEngine::removeRenderingContext( const zpString& name ) { return false; }
zpRenderingContext* zpOpenGLRenderingEngine::getRenderingContextByIndex( zp_uint index ) const { return ZP_NULL; }
zpRenderingContext* zpOpenGLRenderingEngine::getRenderingContext( const zpString& name ) const { return ZP_NULL; }
zp_uint zpOpenGLRenderingEngine::getRenderingContextCount() const { return 0; }
zpRenderingContext* zpOpenGLRenderingEngine::getImmediateRenderingContext() const {
	return m_immediateContext;
}

zpBuffer* zpOpenGLRenderingEngine::createBuffer() { return ZP_NULL; }

zpTextureResource* zpOpenGLRenderingEngine::createTextureResource() { return ZP_NULL; }
zpShaderResource* zpOpenGLRenderingEngine::createShaderResource() { return ZP_NULL; }

zpRenderTarget* zpOpenGLRenderingEngine::createRenderTarget( zpDisplayFormat format, zp_uint width, zp_uint height ) { return ZP_NULL; }
zpRenderTarget* zpOpenGLRenderingEngine::createMultiRenderTarget( zp_uint targetCount, zpDisplayFormat* formats, zp_uint width, zp_uint height ) { return ZP_NULL; }
zpDepthStencilBuffer* zpOpenGLRenderingEngine::createDepthBuffer( zpDisplayFormat format, zp_uint width, zp_uint height ) { return ZP_NULL; }

zpVertexLayout* zpOpenGLRenderingEngine::createVertexLayout( const zpString& desc ) { return ZP_NULL; }

zp_bool zpOpenGLRenderingEngine::initialize() {
	return true;
}
void zpOpenGLRenderingEngine::shutdown() {}
