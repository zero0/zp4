#include "zpCommon.h"

zpCameraComponent::zpCameraComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
    , m_camera( ZP_NULL )
{
    zpRenderingPipeline* pipeline = obj->getApplication()->getRenderPipeline();

    zpVector2i screenPosition( 0, 0 );
    zpVector2i screenSize( pipeline->getRenderingEngine()->getScreenSize() );
    zp_float nearZ = 1.0f;
    zp_float farZ = 100.0f;
    zp_float fovy = 45.0f;
    zpCameraProjection projection = ZP_CAMERA_PROJECTION_PERSPECTIVE;
    zpViewport viewport;

    zpBison::Value proj = def[ "Projection" ];
    if( proj.isString() )
    {
        const zp_char* projectionType = proj.asCString();
        if( zp_strcmp( projectionType, "Ortho" ) == 0 )
        {
            projection = ZP_CAMERA_PROJECTION_ORTHO;
        }
        else if( zp_strcmp( projectionType, "OrthoCenter" ) == 0 )
        {
            projection = ZP_CAMERA_PROJECTION_ORTHO_CENTERED;
        }
    }

    zpBison::Value size = def[ "ScreenSize" ];
    if( size.isArray() && !size.isEmpty() )
    {
        screenSize.setX( size[ 0 ].asInt() );
        screenSize.setY( size[ 1 ].asInt() );
    }

    zpBison::Value position = def[ "ScreenPosition" ];
    if( position.isArray() && !size.isEmpty() )
    {
        screenPosition.setX( position[ 0 ].asInt() );
        screenPosition.setY( position[ 1 ].asInt() );
    }

    zpBison::Value nearDepth = def[ "NearZ" ];
    if( nearDepth.isFloat() )
    {
        nearZ = nearDepth.asFloat();
    }

    zpBison::Value farDepth = def[ "FarZ" ];
    if( farDepth.isFloat() )
    {
        farZ = farDepth.asFloat();
    }

    zpBison::Value fov = def[ "FOV" ];
    if( fov.isFloat() )
    {
        fovy = fov.asFloat();
    }

    const zpMatrix4f& transform = getParentObject()->getComponents()->getTransformComponent()->getWorldTransform();
    const zpVector4f& pos = transform.r[ 3 ];
    const zpVector4f& lookTo = transform.r[ 2 ];
    const zpVector4f& up = transform.r[ 1 ];

    viewport.topX = (zp_float)screenPosition.getX();
    viewport.topY = (zp_float)screenPosition.getY();
    viewport.width = (zp_float)screenSize.getX();
    viewport.height = (zp_float)screenSize.getY();
    viewport.minDepth = nearZ;
    viewport.maxDepth = farZ;

    m_camera = pipeline->getCamera( ZP_CAMERA_TYPE_MAIN );
    m_camera->setActive( getParentObject()->isEnabled() );
    m_camera->setProjectionType( projection );
    m_camera->setPosition( pos );
    m_camera->setLookTo( lookTo );
    m_camera->setAspectRatio( viewport.width / viewport.height );
    m_camera->setUp( up );
    m_camera->setFovy( fovy );
    m_camera->setNearFar( nearZ, farZ );
    m_camera->setOrthoRect( zpRecti( screenPosition, screenSize ) );
    m_camera->setViewport( viewport );
    m_camera->setClipRect( zpRecti( screenPosition, screenSize ) );
}
zpCameraComponent::~zpCameraComponent() {}

void zpCameraComponent::onCreate() {}
void zpCameraComponent::onInitialize() {}
void zpCameraComponent::onDestroy()
{
    zpRenderingPipeline* pipeline = getParentObject()->getApplication()->getRenderPipeline();
    pipeline->releaseCamera( m_camera );
    m_camera = ZP_NULL;
}

void zpCameraComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
    zpTransformComponent* t = getParentObject()->getComponents()->getTransformComponent();
    if( t->getParentObject()->isFlagSet( ZP_OBJECT_FLAG_TRANSFORM_DIRTY ) )
    {
        const zpMatrix4f& transform = t->getWorldTransform();
        const zpVector4f& pos = transform.r[ 3 ];
        const zpVector4f& lookTo = transform.r[ 2 ];
        const zpVector4f& up = transform.r[ 1 ];

        m_camera->setPosition( pos );
        m_camera->setLookTo( lookTo );
        m_camera->setUp( up );
    }
}
void zpCameraComponent::onSimulate() {}

void zpCameraComponent::onEnabled()
{
    m_camera->setActive( true );
}
void zpCameraComponent::onDisabled()
{
    m_camera->setActive( false );
}


zpCameraComponentPool::zpCameraComponentPool() {}
zpCameraComponentPool::~zpCameraComponentPool() {}
void zpCameraComponentPool::update( zp_float deltaTime, zp_float realTime )
{
    m_used.foreach( [ &deltaTime, &realTime ]( zpCameraComponent* o )
    {
        o->update( deltaTime, realTime );
    } );
}
void zpCameraComponentPool::simulate() {}
