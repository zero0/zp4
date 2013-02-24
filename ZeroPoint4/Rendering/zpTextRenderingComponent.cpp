#include "zpRendering.h"

zpTextRenderingComponent::zpTextRenderingComponent()
	: m_isLocalToGameObject( true )
	, m_isTextDirty( false )
	, m_size()
	, m_localTransform()
	, m_text()
	, m_font()
	, m_shader()
	, m_buffer()
{}
zpTextRenderingComponent::~zpTextRenderingComponent() {}

void zpTextRenderingComponent::render() {
	if( m_text.isEmpty() ) return;
 	if( !m_font ) return;
	if( !m_shader ) return;

	zpRenderingContext* c = zpRenderingFactory::getRenderingEngine()->getImmediateRenderingContext();

	c->setShader( &m_shader );
	c->setTexture( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, ZP_MATERIAL_TEXTURE_SLOT_DIFFUSE, m_font.getResource()->getTexture().getResource()->getTexture() );
	m_buffer.draw();
}

void zpTextRenderingComponent::receiveMessage( const zpMessage& message ) {}

void zpTextRenderingComponent::serialize( zpSerializedOutput* out ) {}
void zpTextRenderingComponent::deserialize( zpSerializedInput* in ) {}

void zpTextRenderingComponent::setText( const zpString& text ) {
	m_text = text;
	m_isTextDirty = true;
}
const zpString& zpTextRenderingComponent::getText() const {
	return m_text;
}

void zpTextRenderingComponent::onCreate() {
	//m_buffer.create( 256 * 4, 256 * 6 );
	//zpContentManager* content = getGame()->getGameManagers()->getContentManager();
	//
	//m_font = content->createInstanceOfResource<zpFontResource>( m_fontAlias );
	//m_shader = content->createInstanceOfResource<zpShaderResource>( m_shaderAlias );
}
void zpTextRenderingComponent::onDestroy() {
	m_buffer.destroy();
}

void zpTextRenderingComponent::onUpdate() {
	//if( m_isTextDirty ) {
	//	m_buffer.clear();
	//	const zpFontSet& set = m_font.getResource()->getFontSet();
	//
	//	zp_float fontSize = 12.f;
	//	zp_float sizeScale = fontSize / (zp_float)set.size;
	//	zp_float lineWidth = 0.f;
	//	zp_float wordWidth = 0.f;
	//	zp_float maxWidth = m_size.getX();
	//	zp_float x = 0.f;
	//	zp_float y = 0.f;
	//	zp_uint lineNumber = 1;
	//	zp_uint wordNumber = 1;
	//
	//	zpVector4f position;
	//	if( m_isLocalToGameObject ) {
	//		zpMatrix4f transform;
	//		m_localTransform.mul( getParentGameObject()->getTransform(), transform );
	//		transform.getPosition( position );
	//	} else {
	//		m_localTransform.getPosition( position );
	//	}
	//
	//	for( zp_uint i = 0; i < m_text.size(); ++i ) {
	//		const zpFontGlyph& glyph = set.glyphs[ (zp_uint)m_text[ i ] ];
	//		zp_float xOffset = glyph.xOffset * sizeScale;
	//		zp_float yOffset = glyph.yOffset * sizeScale;
	//		zp_float xAdvance = glyph.xAdvance * sizeScale;
	//		zp_float width = glyph.width * sizeScale;
	//		zp_float height = glyph.height * sizeScale;
	//		
	//		if( glyph.character == '\n' || glyph.character == '\r' || ( lineWidth + xAdvance >= maxWidth ) ) {
	//			
	//		}
	//	}
	//	m_isTextDirty = false;
	//}
}

void zpTextRenderingComponent::onEnabled() {}
void zpTextRenderingComponent::onDisabled() {}

void zpTextRenderingComponent::onShow() {}
void zpTextRenderingComponent::onHide() {}