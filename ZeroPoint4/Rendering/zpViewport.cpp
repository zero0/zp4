#include "zpRendering.h"

zpViewport::zpViewport()
    : width( 0 )
    , height( 0 )
    , minDepth( 0 )
    , maxDepth( 0 )
    , topX( 0 )
    , topY( 0 )
{}
//zpViewport::zpViewport( zp_float width, zp_float height, zp_float minDepth, zp_float maxDepth, zp_float topX, zp_float topY )
//    : m_width( width )
//    , m_height( height )
//    , m_minDepth( minDepth )
//    , m_maxDepth( maxDepth )
//    , m_topX( topX )
//    , m_topY( topY )
//{}
//
//zp_float zpViewport::getWidth() const { return m_width; }
//zp_float zpViewport::getHeight() const { return m_height; }
//zp_float zpViewport::getMinDepth() const { return m_minDepth; }
//zp_float zpViewport::getMaxDepth() const { return m_maxDepth; }
//zp_float zpViewport::getTopX() const { return m_topX; }
//zp_float zpViewport::getTopY() const { return m_topY; }
