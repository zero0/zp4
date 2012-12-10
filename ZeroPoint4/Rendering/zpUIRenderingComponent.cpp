#include "zpRendering.h"
#include "Awesomium\awesomium_capi.h"

zpString& awe_string_to_std( const awe_string* aweString, zpString& outString ) {
	int length = awe_string_to_wide( aweString, NULL, 0 );

	wchar_t* wbuff = new wchar_t[ length + 1 ];
	char* buff = new char[ length + 1 ];
	zp_memset( buff, 0, length + 1 );
	zp_memset( wbuff, 0, sizeof( wchar_t ) * (length + 1) );
	awe_string_to_wide( aweString, wbuff, length );

	size_t outCount;
	wcstombs_s( &outCount, buff, length+1, wbuff, _TRUNCATE );

	outString = buff;

	delete[] wbuff;
	delete[] buff;

	return outString;
}

class AweString {
public:
	AweString() : m_aweString( ZP_NULL ) {}
	AweString( awe_string* str ) : m_aweString( str ) {
		awe_string_to_std( m_aweString, m_zpString );
	}
	AweString( const zpString& str ) : m_aweString( ZP_NULL ) {
		m_zpString = str;
		m_aweString = awe_string_create_from_ascii( m_zpString.c_str(), m_zpString.length() );
	}
	~AweString() {
		if( m_aweString ) {
			awe_string_destroy( m_aweString );
			m_aweString = ZP_NULL;
		}
	}

	operator const awe_string*() const { return m_aweString; }
	operator const zpString&() const { return m_zpString; }

private:
	AweString( const AweString& other ) : m_aweString( other.m_aweString ), m_zpString( other.m_zpString ) {}
	AweString& operator=( const AweString& other ) { return (*this); }

	awe_string* m_aweString;
	zpString m_zpString;
};

void register_js_callback( awe_webview* webView, const zpString& objectName, const zpString& callbackName ) {
	AweString objname( objectName );
	AweString callback( callbackName );

	awe_webview_set_object_callback( webView, objname, callback );
}

void __jsout( awe_webview* caller, const awe_string* message, int line_number, const awe_string* source ) {
	zpString msg;
	awe_string_to_std( message, msg );

	zpString src;
	awe_string_to_std( source, src );

	//zp_printfcln( zp_console_color( CC_CYAN, CC_GRAY ), "%s[%d]: %s", src.c_str(), line_number, msg.c_str() );
}
awe_resource_response* __jsrequestresource( awe_webview* caller, awe_resource_request* request ) {
	AweString url( awe_resource_request_get_url( request ) );

	//zp_printfln( "Request: %s", ( (const std::string&)url ).c_str() );

	return awe_resource_response_create_from_file( url );
}

zpUIRenderingComponent::zpUIRenderingComponent()
	: m_webView( ZP_NULL )
	, m_texture( ZP_NULL )
{}
zpUIRenderingComponent::~zpUIRenderingComponent() {}

void zpUIRenderingComponent::render() {
	zpRenderingContext* context = zpRenderingFactory::getRenderingEngine()->getImmediateRenderingContext();

	context->setShader( &m_uiShader );
	context->setTexture( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, 0, m_texture );

	m_uiVertexBuffer.draw();
}

void zpUIRenderingComponent::receiveMessage( const zpMessage& message ) {}

void zpUIRenderingComponent::serialize( zpSerializedOutput* out ) {}
void zpUIRenderingComponent::deserialize( zpSerializedInput* in ) {}

void zpUIRenderingComponent::onCreate() {
	zpContentManager* content = getGame()->getGameManagers()->getContentManager();

	m_ui = content->createInstanceOfResource<zpUIResource>( m_uiAlias );

	awe_webview* view = awe_webcore_create_webview( m_size.getX(), m_size.getY(), false );
	m_webView = view;

	AweString file( m_ui.getResource()->getFilename() );

	awe_webview_load_file( view, file, awe_string_empty() );

	awe_webview_set_transparent( view, true );
	//awe_webview_create_object( view, objname );
	//awe_webview_set_callback_js_callback( view, __jscallback );
	//awe_webview_set_callback_request_file_chooser( view, __jsopenfile );

#ifdef ZP_DEBUG
	// register to output errors if in debug mode
	awe_webview_set_callback_js_console_message( view, __jsout );
	awe_webview_set_callback_resource_request( view, __jsrequestresource );
#endif

	zpVertexPositionNormalTexture v0 = { zpVector4f(), zpVector4f(), zpVector2f() };
	zpVertexPositionNormalTexture v1 = { zpVector4f(), zpVector4f(), zpVector2f() };
	zpVertexPositionNormalTexture v2 = { zpVector4f(), zpVector4f(), zpVector2f() };
	zpVertexPositionNormalTexture v3 = { zpVector4f(), zpVector4f(), zpVector2f() };

	m_uiVertexBuffer.create( 6, 4 );
	m_uiVertexBuffer.addQuad( v0, v1, v2, v3 );

	m_texture = zpRenderingFactory::getRenderingEngine()->createTexture( m_size.getX(), m_size.getY(), ZP_TEXTURE_TYPE_TEXTURE, ZP_TEXTURE_DIMENSION_2D, ZP_DISPLAY_FORMAT_RGBA8_UNORM, ZP_CPU_ACCESS_WRITE, ZP_NULL, 1 );
}
void zpUIRenderingComponent::onDestroy() {
	awe_webview_destroy( (awe_webview*)m_webView );
	m_webView = ZP_NULL;

	ZP_SAFE_DELETE( m_texture );
}

void zpUIRenderingComponent::onUpdate() {
	awe_webview* view = (awe_webview*)m_webView;
	if( awe_webview_is_dirty( view ) ) {
		const awe_renderbuffer* buffer = awe_webview_render( view );
		if( buffer ) {
			zpRenderingFactory::getRenderingEngine()->getImmediateRenderingContext()->updateTexture( m_texture, (void*)awe_renderbuffer_get_buffer( buffer ) );
		}
	}
}

void zpUIRenderingComponent::onEnabled() {}
void zpUIRenderingComponent::onDisabled() {}

void zpUIRenderingComponent::onShow() {}
void zpUIRenderingComponent::onHide() {}
