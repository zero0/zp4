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
	zpVector4f up( 0, 1, 0, 0 );
	zpVector4f lookTo( 0, 0, -1, 0 );

	zpBison::Value proj = def[ "Projection" ];
	if( proj.isInt() )
	{
		projection = (zpCameraProjection)proj.asInt();
	}

	zpBison::Value size = def[ "ScreenSize" ];
	if( size.isArray() )
	{
		screenSize.setX( size[ 0 ].asInt() );
		screenSize.setY( size[ 1 ].asInt() );
	}

	zpBison::Value position = def[ "ScreenPosition" ];
	if( position.isArray() )
	{
		screenPosition.setX( position[ 0 ].asInt() );
		screenPosition.setY( position[ 1 ].asInt() );
	}

	zpBison::Value depthValues = def[ "NearFarZ" ];
	if( depthValues.isArray() )
	{
		nearZ = depthValues[ 0 ].asFloat();
		farZ = depthValues[ 1 ].asFloat();
	}

	zpBison::Value fov = def[ "FOV" ];
	if( fov.isFloat() )
	{
		fovy = fov.asFloat();
	}

	zpBison::Value upValue = def[ "Up" ];
	if( upValue.isArray() )
	{
		up = zpVector4f( upValue[ 0 ].asFloat(), upValue[ 1 ].asFloat(), upValue[ 2 ].asFloat(), 0.0f );
	}

	zpBison::Value lookToValue = def[ "LookTo" ];
	if( lookToValue.isArray() )
	{
		lookTo = zpVector4f( lookToValue[ 0 ].asFloat(), lookToValue[ 1 ].asFloat(), lookToValue[ 2 ].asFloat() );
	}

	viewport.topX = (zp_float)screenPosition.getX();
	viewport.topY = (zp_float)screenPosition.getY();
	viewport.width = (zp_float)screenSize.getX();
	viewport.height = (zp_float)screenSize.getY();
	viewport.minDepth = nearZ;
	viewport.maxDepth = farZ;

	m_camera = pipeline->getCamera( 0 );
	m_camera->setProjectionType( projection );
	m_camera->setPosition( getParentObject()->getTransform().getRow( 3 ) );
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
void zpCameraComponent::onDestroy() {}

void zpCameraComponent::onUpdate() {}
void zpCameraComponent::onSimulate() {}

void zpCameraComponent::onEnabled() {}
void zpCameraComponent::onDisabled() {}


zpCameraComponentPool::zpCameraComponentPool() {}
zpCameraComponentPool::~zpCameraComponentPool() {}
void zpCameraComponentPool::update()
{
	m_used.foreach( []( zpCameraComponent* o )
	{
		o->update();
	} );
}
void zpCameraComponentPool::simulate() {}
