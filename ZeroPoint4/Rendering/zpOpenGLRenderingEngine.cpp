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
		zpLog::critical() << "Unable to initialize GLEW" << zpLog::endl;
		return false;
	}

	if( WGLEW_ARB_create_context && WGLEW_ARB_pixel_format ) {
		const zp_int pixelAttr[] = {
			WGL_DRAW_TO_WINDOW_ARB,	GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,	GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
			WGL_PIXEL_TYPE_ARB,		WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,		32,
			WGL_DEPTH_BITS_ARB,		24,
			WGL_STENCIL_BITS_ARB,	8,
			0
		};
		const zp_int contextAttr[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,	3,
			WGL_CONTEXT_MINOR_VERSION_ARB,	1,
#if ZP_DEBUG
			WGL_CONTEXT_FLAGS_ARB,			WGL_CONTEXT_DEBUG_BIT_ARB,
#else
			WGL_CONTEXT_FLAGS_ARB,			0,
#endif
			0
		};

		zp_int pixelFormat;
		zp_uint numFormats;
		wglChoosePixelFormatARB( hdc, pixelAttr, ZP_NULL, 1, &pixelFormat, &numFormats );
		SetPixelFormat( hdc, pixelFormat, &pfd );

		HGLRC rc = wglCreateContextAttribsARB( hdc, 0, contextAttr );
		wglMakeCurrent( ZP_NULL, ZP_NULL );
		wglDeleteContext( context );
		wglMakeCurrent( hdc, rc );
		context = rc;
	}
	const zp_byte* vv = glGetString( GL_VERSION );

	// create the immediate context directly
	m_immediateContext = new zpOpenGLRenderingContext( context, hdc, "immediate" );

	// create the render target and depth/stencil buffer
	m_immediateRenderTarget = createRenderTarget( ZP_DISPLAY_FORMAT_RGBA8_UNORM, m_window->getScreenSize().getX(), m_window->getScreenSize().getY() );
	m_immediateDepthStencilBuffer = createDepthBuffer( ZP_DISPLAY_FORMAT_D24S8_UNORM_UINT, m_window->getScreenSize().getX(), m_window->getScreenSize().getY() );

	// set the render target and depth/stencil buffer to the immediate context
	m_immediateContext->setRenderTarget( m_immediateRenderTarget );
	m_immediateContext->setDepthStencilBuffer( m_immediateDepthStencilBuffer );

	// remove reference so immediate context has reference
	m_immediateRenderTarget->removeReference();
	m_immediateDepthStencilBuffer->removeReference();

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

void zpOpenGLRenderingEngine::present() {
	zpOpenGLRenderingContext* ctx = (zpOpenGLRenderingContext*)m_immediateContext;

	wglMakeCurrent( (HDC)ctx->m_hdc, (HGLRC)ctx->m_context );

	glDrawBuffer( GL_FRAMEBUFFER );

	glFlush(); 
	
	SwapBuffers( (HDC)ctx->m_hdc );
}

zpRenderingContext* zpOpenGLRenderingEngine::createRenderingContext( const zpString& name ) {
	HWND hwnd = (HWND)m_window->getWindowHandle();
	HDC hdc = GetDC( hwnd );

	// @TODO: make function to return better GL context
	HGLRC context = wglCreateContext( hdc );
	
	return new zpOpenGLRenderingContext( context, hdc, name );
}
zp_bool zpOpenGLRenderingEngine::removeRenderingContext( const zpString& name ) { return false; }
zpRenderingContext* zpOpenGLRenderingEngine::getRenderingContextByIndex( zp_uint index ) const { return ZP_NULL; }
zpRenderingContext* zpOpenGLRenderingEngine::getRenderingContext( const zpString& name ) const { return ZP_NULL; }
zp_uint zpOpenGLRenderingEngine::getRenderingContextCount() const { return 0; }
zpRenderingContext* zpOpenGLRenderingEngine::getImmediateRenderingContext() const {
	return m_immediateContext;
}

zpBuffer* zpOpenGLRenderingEngine::createBuffer() {
	return new zpOpenGLBuffer;
}

zpTextureResource* zpOpenGLRenderingEngine::createTextureResource() { return ZP_NULL; }
zpShaderResource* zpOpenGLRenderingEngine::createShaderResource() {
	return new zpOpenGLShaderResource;
}

zpRenderTarget* zpOpenGLRenderingEngine::createRenderTarget( zpDisplayFormat format, zp_uint width, zp_uint height ) {
	zp_uint framebuffer = 0;
	zp_uint renderbuffer = 0;

	glGenFramebuffers( 1, &framebuffer );
	glGenRenderbuffers( 1, &renderbuffer );

	checkError();

	glBindRenderbuffer( GL_RENDERBUFFER, renderbuffer );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA8, width, height );

	checkError();

	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, framebuffer );
	glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer );

	checkError();

	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

	checkError();

	return new zpOpenGLRenderTarget( framebuffer, &renderbuffer, 1, width, height );
}
zpRenderTarget* zpOpenGLRenderingEngine::createMultiRenderTarget( zp_uint targetCount, zpDisplayFormat* formats, zp_uint width, zp_uint height ) {
	return ZP_NULL;
}
zpDepthStencilBuffer* zpOpenGLRenderingEngine::createDepthBuffer( zpDisplayFormat format, zp_uint width, zp_uint height ) {
	zp_uint framebuffer = 0;
	zp_uint renderbuffer = 0;

	//glGenFramebuffers( 1, &framebuffer );
	glGenRenderbuffers( 1, &renderbuffer );

	checkError();

	glBindRenderbuffer( GL_RENDERBUFFER, renderbuffer );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height );

	checkError();

	//glBindFramebuffer( GL_DRAW_FRAMEBUFFER, framebuffer );
	//glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer );

	//checkError();

	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
	//glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

	checkError();

	return new zpOpenGLDepthStencilBuffer( format, framebuffer, renderbuffer, width, height );
}

zpVertexLayout* zpOpenGLRenderingEngine::createVertexLayout( const zpString& desc ) {
	return ZP_NULL;
}

zpSamplerState* zpOpenGLRenderingEngine::createSamplerState( const zpSamplerStateDesc& desc ) {
	return ZP_NULL;
}

zp_bool zpOpenGLRenderingEngine::initialize() {
	return true;
}
void zpOpenGLRenderingEngine::shutdown() {}
