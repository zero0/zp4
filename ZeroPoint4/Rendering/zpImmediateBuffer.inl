
template<typename V, typename I>
zpImmediateBuffer<V, I>::zpImmediateBuffer() 
	: m_vertexSize( 0 )
	, m_indexSize( 0 )
	, m_topology( ZP_TOPOLOGY_UNKNOWN )
	, m_vertexBuffer( ZP_NULL )
	, m_indexBuffer( ZP_NULL )
{}
template<typename V, typename I>
zpImmediateBuffer<V, I>::~zpImmediateBuffer() {
	destroy();

	ZP_SAFE_DELETE( m_vertexBuffer );
	ZP_SAFE_DELETE( m_indexBuffer );
}

template<typename V, typename I>
void zpImmediateBuffer<V, I>::create( zp_uint vertexBufferCount, zp_uint indexBufferCount ) {
	zpRenderingEngine* engine = zpRenderingFactory::getRenderingEngine();

	m_vertexBuffer = engine->createBuffer();
	m_indexBuffer = engine->createBuffer();

	m_vertexBuffer->create( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_DYNAMIC, vertexBufferCount, sizeof( V ) );
	m_indexBuffer->create( ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_DYNAMIC, indexBufferCount, sizeof( I ) );
}
template<typename V, typename I>
void zpImmediateBuffer<V, I>::destroy() {
	m_vertexBuffer->destroy();
	m_indexBuffer->destroy();
}

template<typename V, typename I>
void zpImmediateBuffer<V, I>::addPoint( const V& v0 ) {
	if( m_topology != ZP_TOPOLOGY_POINT_LIST ) return;

	V* verts = ZP_NULL;
	m_vertexBuffer->map( &verts, ZP_MAP_TYPE_WRITE );
	verts[ m_vertexSize ] = v0;
	m_vertexBuffer->unmap();

	I* index = ZP_NULL;
	m_indexBuffer->map( &index, ZP_MAP_TYPE_WRITE );
	index[ m_indexSize ] = (I)m_vertexSize;
	m_indexBuffer->unmap();

	m_vertexSize += 1;
	m_indexSize += 1;
}
template<typename V, typename I>
void zpImmediateBuffer<V, I>::addLine( const V& v0, const V& v1 ) {
	if( m_topology != ZP_TOPOLOGY_LINE_LIST ) return;

	V* verts = ZP_NULL;
	m_vertexBuffer->map( &verts, ZP_MAP_TYPE_WRITE );
	verts[ m_vertexSize + 0 ] = v0;
	verts[ m_vertexSize + 1 ] = v1;
	m_vertexBuffer->unmap();
	
	I* index = ZP_NULL;
	m_indexBuffer->map( &index, ZP_MAP_TYPE_WRITE );
	index[ m_indexSize + 0 ] = (I)m_vertexSize + 0;
	index[ m_indexSize + 1 ] = (I)m_vertexSize + 1;
	m_indexBuffer->unmap();

	m_vertexSize += 2;
	m_indexSize += 2;
}
template<typename V, typename I>
void zpImmediateBuffer<V, I>::addTriangle( const V& v0, const V& v1, const V& v2 ) {
	if( m_topology != ZP_TOPOLOGY_TRIANGLE_LIST ) return;

	V* verts = ZP_NULL;
	m_vertexBuffer->map( &verts, ZP_MAP_TYPE_WRITE );
	verts[ m_vertexSize + 0 ] = v0;
	verts[ m_vertexSize + 1 ] = v1;
	verts[ m_vertexSize + 2 ] = v2;
	m_vertexBuffer->unmap();

	I* index = ZP_NULL;
	m_indexBuffer->map( &index, ZP_MAP_TYPE_WRITE );
	index[ m_indexSize + 0 ] = (I)m_vertexSize + 0;
	index[ m_indexSize + 1 ] = (I)m_vertexSize + 1;
	index[ m_indexSize + 2 ] = (I)m_vertexSize + 2;
	m_indexBuffer->unmap();

	m_vertexSize += 3;
	m_indexSize += 3;
}
template<typename V, typename I>
void zpImmediateBuffer<V, I>::addQuad( const V& v0, const V& v1, const V& v2, const V& v3 ) {
	if( m_topology != ZP_TOPOLOGY_TRIANGLE_LIST ) return;
	
	V* verts = ZP_NULL;
	m_vertexBuffer->map( &verts, ZP_MAP_TYPE_WRITE );
	verts[ m_vertexSize + 0 ] = v0;
	verts[ m_vertexSize + 1 ] = v1;
	verts[ m_vertexSize + 2 ] = v2;
	verts[ m_vertexSize + 3 ] = v3;
	m_vertexBuffer->unmap();

	I* index = ZP_NULL;
	m_indexBuffer->map( &index, ZP_MAP_TYPE_WRITE );
	index[ m_indexSize + 0 ] = (I)m_vertexSize + 0;
	index[ m_indexSize + 1 ] = (I)m_vertexSize + 1;
	index[ m_indexSize + 2 ] = (I)m_vertexSize + 2;

	index[ m_indexSize + 3 ] = (I)m_vertexSize + 0;
	index[ m_indexSize + 4 ] = (I)m_vertexSize + 2;
	index[ m_indexSize + 5 ] = (I)m_vertexSize + 3;
	m_indexBuffer->unmap();

	m_vertexSize += 4;
	m_indexSize += 6;
}

template<typename V, typename I>
void zpImmediateBuffer<V, I>::clear() {
	m_vertexSize = 0;
	m_indexSize = 0;
}
template<typename V, typename I>
void zpImmediateBuffer<V, I>::draw() {
	zpRenderingContext* context = zpRenderingFactory::getRenderingEngine()->getImmediateRenderingContext();
	
	context->setTopology( m_topology );
	context->setBuffer( m_vertexBuffer );
	context->setBuffer( m_indexBuffer );

	context->drawIndexed( m_indexSize );
}
