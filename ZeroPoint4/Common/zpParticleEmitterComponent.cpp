#include "zpCommon.h"

const zp_char* g_shape[] =
{
	"Sphere",
	"Hemisphere",
	"Cone",
	"Box",
};

zpParticleEmitterComponent::zpParticleEmitterComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_layer( ZP_RENDERING_LAYER_DEFAULT )
	, m_queue( ZP_RENDERING_QUEUE_TRANSPARENT )
	, m_maxParticles( 0 )
	, m_random( zpRandom::getInstance() )
{
	const zpBison::Value& layer = def[ "Layer" ];
	if( layer.isInt() )
	{
		m_layer = layer.asInt();
	}

	m_maxParticles = def[ "MaxParticles" ].asInt();
	m_allParticles.reserve( m_maxParticles );
	m_allParticles.resize( m_maxParticles );

	m_freeParticles.reserve( m_maxParticles );

	zpParticle* b = m_allParticles.begin();
	zpParticle* e = m_allParticles.end();
	for( ; b != e; ++b )
	{
		m_freeParticles.pushBack( b );
	}

	const zpBison::Value& effects = def[ "Effects" ];
	zp_uint effectCount = effects.size();
	m_effects.reserve( effectCount );
	effects.foreachObject( [ this ]( const zpBison::Value& name, const zpBison::Value& effect )
	{
		createEffect( name.asCString(), effect );
	} );
}
zpParticleEmitterComponent::~zpParticleEmitterComponent()
{}

void zpParticleEmitterComponent::render( zpRenderingContext* i, const zpCamera* camera )
{
	if( m_flags.isAllMarked( 1 << ZP_COMPONENT_FLAG_CREATED | 1 << ZP_COMPONENT_FLAG_ENABLED ) )
	{
		onRender( i, camera );
	}
}

void zpParticleEmitterComponent::setRenderLayer( zp_uint layer )
{
	m_layer = layer;
}
zp_uint zpParticleEmitterComponent::getRenderLayer() const
{
	return m_layer;
}

void zpParticleEmitterComponent::setRenderingQueue( zpRenderingQueue queue )
{
	m_queue = queue;
}
zpRenderingQueue zpParticleEmitterComponent::getRenderingQueue() const
{
	return m_queue;
}

void zpParticleEmitterComponent::play( const zp_char* effectName, zp_bool force )
{
	zpParticleEffect* b = m_effects.begin();
	zpParticleEffect* e = m_effects.end();
	for( ; b != e; ++b )
	{
		if( b->name == effectName )
		{
			if( b->state == ZP_PARTICLE_EFFECT_STATE_DISABLED || force )
			{
				playEffect( b );
			}
			break;
		}
	}
}
void zpParticleEmitterComponent::stop( const zp_char* effectName )
{
	zpParticleEffect* b = m_effects.begin();
	zpParticleEffect* e = m_effects.end();
	for( ; b != e; ++b )
	{
		if( b->name == effectName )
		{
			b->state = ZP_PARTICLE_EFFECT_STATE_FINISHING;
			break;
		}
	}
}
void zpParticleEmitterComponent::stopAll()
{
	zpParticleEffect* b = m_effects.begin();
	zpParticleEffect* e = m_effects.end();
	for( ; b != e; ++b )
	{
		b->state = ZP_PARTICLE_EFFECT_STATE_FINISHING;
	}
}

zp_bool zpParticleEmitterComponent::isPlaying( const zp_char* effectName ) const
{
	zp_bool playing = false;

	const zpParticleEffect* b = m_effects.begin();
	const zpParticleEffect* e = m_effects.end();
	for( ; b != e && !playing; ++b )
	{
		playing = b->name == effectName;
	}

	return playing;
}
zp_bool zpParticleEmitterComponent::isAnyPlaying() const
{
	zp_bool playing = false;
	
	const zpParticleEffect* b = m_effects.begin();
	const zpParticleEffect* e = m_effects.end();
	for( ; b != e && !playing; ++b )
	{
		playing = b->state == ZP_PARTICLE_EFFECT_STATE_PLAYING;
	}

	return playing;
}
zp_bool zpParticleEmitterComponent::isPaused() const
{
	return m_isPaused;
}
void zpParticleEmitterComponent::pause( zp_bool isPaused )
{
	m_isPaused = isPaused;
}

void zpParticleEmitterComponent::onRender( zpRenderingContext* i, const zpCamera* camera )
{
	zpParticleEffect* effect = m_effects.begin();
	zpParticleEffect* end = m_effects.end();
	for( ; effect != end; ++effect )
	{
		if( effect->state != ZP_PARTICLE_EFFECT_STATE_PLAYING && effect->state != ZP_PARTICLE_EFFECT_STATE_FINISHING ) continue;

		// if depth sort enabled, sort particles based on distance to camera
		if( effect->useDepthSort )
		{
			//effect->usedParticles.sort( [ camera ]( zpParticle* a, zpParticle* b ) -> zp_bool
			//{
			//	zp_int cmp;
			//	zpScalar lenA, lenB;
			//	zpVector4f posA, posB;
			//	zpMath::Sub( posA, a->position, camera->getPosition() );
			//	zpMath::Sub( posB, b->position, camera->getPosition() );
			//	zpMath::LengthSquared3( lenA, posA );
			//	zpMath::LengthSquared3( lenB, posB );
			//	zpMath::Cmp( cmp, lenA, lenB );
			//
			//	return cmp > 0;
			//} );
		}

		i->beginDrawImmediate( m_layer, m_queue, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, &effect->material );

		//if( !effect->isWorldSpace )
		//{
		//	i->setMatrix( getParentObject()->getComponents()->getTransformComponent()->getWorldTransform() );
		//}
		i->setSortBias( effect->sortBias );

		// setup UV rect
		zpRectf uv( 0, 0, 1, 1 );

		// if flipbook values set, calculate new UVs
		if( effect->flipbookX != 0 && effect->flipbookY != 0 )
		{
			zp_uint index = zp_floor_to_int( effect->time * effect->flipbookSpeed );

			index = index % ( effect->flipbookX * effect->flipbookY );

			zpVector2f size( 1.f / effect->flipbookX, 1.f / effect->flipbookY );

			zp_uint uIndex = index % effect->flipbookX;
			zp_uint vIndex = index / effect->flipbookX;

			zpVector2f offset( uIndex * size.getX(), vIndex * size.getY() );

			uv.setPosition( offset );
			uv.setSize( size );
		}

		// setup normals
		//const zp_float n = 0.70711f;
		//zpVector4f n0( -n, -n, n, 0 );
		//zpVector4f n1( -n,  n, n, 0 );
		//zpVector4f n2(  n, -n, n, 0 );
		//zpVector4f n3(  n,  n, n, 0 );

		zpParticle** p = effect->usedParticles.begin();
		zpParticle** e = effect->usedParticles.end();
		for( ; p != e; ++p )
		{
			zpParticle* particle = *p;

			zpMatrix4f wvp;

			// scale points out from the center
			zpVector4f p0 = zpMath::Vector4( -1,  1, 0, 1 );
			zpVector4f p1 = zpMath::Vector4(  1,  1, 0, 1 );
			zpVector4f p2 = zpMath::Vector4(  1, -1, 0, 1 );
			zpVector4f p3 = zpMath::Vector4( -1, -1, 0, 1 );

			// scale particles
			p0 = zpMath::Vector4Scale( p0, particle->scale );
			p1 = zpMath::Vector4Scale( p1, particle->scale );
			p2 = zpMath::Vector4Scale( p2, particle->scale );
			p3 = zpMath::Vector4Scale( p3, particle->scale );

			// if billboard enabled, rotate towards camera
			zpVector4f look, right, up;
			//if( effect->isBillboard )
			//{
			//	zpMath::Sub( look, particle->position, camera->getPosition() );
			//	zpMath::Normalize3( look, look );
			//	zpMath::Cross3( right, camera->getUp(), look );
			//	zpMath::Normalize3( right, right );
			//	zpMath::Cross3( up, look, right );
			//}
			//// otherwise, set position and rotate so the normal is facing up
			//else
			{
				look = particle->normal;
				right = zpMath::Vector4Perpendicular3( look );
				up = zpMath::Vector4Cross3( right, look );
			}

			wvp.r[ 0 ] = right;
			wvp.r[ 1 ] = up;
			wvp.r[ 2 ] = look;
			wvp.r[ 3 ] = particle->position;

			// get final point
			p0 = zpMath::MatrixTransform( wvp, p0 );
			p1 = zpMath::MatrixTransform( wvp, p1 );
			p2 = zpMath::MatrixTransform( wvp, p2 );
			p3 = zpMath::MatrixTransform( wvp, p3 );

			// draw quad
			i->addQuad(
				p0, uv.getTopLeft(),
				p1, uv.getTopRight(),
				p2, uv.getBottomRight(),
				p3, uv.getBottomLeft(),
				particle->color
				);
		}

		i->endDrawImmediate();
	}
}

void zpParticleEmitterComponent::onCreate()
{

}
void zpParticleEmitterComponent::onInitialize()
{
	m_prevPosition = getParentObject()->getComponents()->getTransformComponent()->getWorldPosition();

	zpParticleEffect* b = m_effects.begin();
	zpParticleEffect* e = m_effects.end();
	for( ; b != e; ++b )
	{
		if( b->playOnCreate )
		{
			playEffect( b );
		}
	}
}
void zpParticleEmitterComponent::onDestroy()
{
	// TODO: currently playing effects need to copy to external player in component pool to fade out particles better
}

void zpParticleEmitterComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
	if( m_isPaused ) return;

	zpVector4f position( getParentObject()->getComponents()->getTransformComponent()->getWorldPosition() ), pos, velocity;

	pos = zpMath::Vector4Sub( position, m_prevPosition );
	m_prevPosition = position;

	zpScalar dt = zpMath::Scalar( deltaTime );
	velocity = zpMath::Vector4Mul( pos, dt );

	zpParticleEffect* effect = m_effects.begin();
	zpParticleEffect* end = m_effects.end();
	for( ; effect != end; ++effect )
	{
		if( effect->state == ZP_PARTICLE_EFFECT_STATE_DISABLED ) continue;

		// use either dt or rt based on the delta time flag
		zp_float fdt = effect->useRealTime ? realTime : deltaTime;

		// increment effect time
		effect->time += fdt;

		// if effect is waiting to start and is passed the delay, star the effect
		if( effect->state == ZP_PARTICLE_EFFECT_STATE_STARTING )
		{
			if( effect->time > effect->delay )
			{
				effect->state = ZP_PARTICLE_EFFECT_STATE_PLAYING;
				effect->time = 0.f;
				effect->emitTime = 0.f;
			}
			// otherwise, effect still waiting to start
			else
			{
				continue;
			}
		}

		// if the effect time has reached the duration of the effect, disable or restart based on loop flag
		if( effect->time > effect->duration )
		{
			if( effect->isLooping )
			{
				effect->time = effect->time - effect->duration;
			}
			else
			{
				if( effect->usedParticles.isEmpty() )
				{
					effect->state = ZP_PARTICLE_EFFECT_STATE_DISABLED;
					continue;
				}
				else
				{
					effect->state = ZP_PARTICLE_EFFECT_STATE_FINISHING;
				}
			}
		}

		// if not finishing effect, still emit particles
		if( effect->state != ZP_PARTICLE_EFFECT_STATE_FINISHING )
		{
			effect->emitTime -= fdt;
			if( effect->emitTime < 0.f )
			{
				effect->emitTime = m_random->randomFloat( effect->minEmitRate, effect->maxEmitRate );
				emitParticle( effect );
			}
		}

		zpScalar velScale = zpMath::Scalar( effect->inheritVelocityScale );

		// update particles
		zpParticle** p = effect->usedParticles.begin();
		zpParticle** e = effect->usedParticles.end();
		for( ; p != e; ++p )
		{
			zpParticle* particle = *p;

			particle->life -= fdt;
			if( particle->life < 0.f ) continue;

			// calculate velocity, position and rotation
			particle->velocity = zpMath::Vector4Madd( particle->velocity, velScale, velocity );
			particle->velocity = zpMath::Vector4Madd( particle->velocity, dt, effect->gravity );
			particle->position = zpMath::Vector4Madd( particle->position, dt, particle->velocity );
			particle->rotation = zpMath::Vector4Madd( particle->rotation, dt, particle->angularVelocity );

			// get life and speed percents
			zpScalar sLife     = zpMath::Scalar( particle->life );
			zpScalar sMaxLife  = zpMath::Scalar( particle->maxLife );
			zpScalar sMaxSpeed = zpMath::Scalar( particle->maxSpeed );
			zpScalar sSpeed;

			sSpeed = zpMath::Vector4Length3( particle->velocity );
			sLife = zpMath::ScalarSub( sMaxLife, sLife );
			sLife = zpMath::ScalarDiv( sLife, sMaxLife );
			sSpeed = zpMath::ScalarDiv( sSpeed, sMaxSpeed );

			// blend between start and end color based on life or speed
			switch( effect->colorRange )
			{
			case ZP_PARTICLE_EFFECT_RANGE_CONSTANT:
				particle->color = effect->startColor;
				break;

			case ZP_PARTICLE_EFFECT_RANGE_RANDOM:
				zpColor4f::Lerp( particle->color, effect->startColor, effect->endColor, zpMath::Scalar( m_random->randomFloat( 0.f, 1.f ) ) );
				break;

			case ZP_PARTICLE_EFFECT_RANGE_LIFETIME:
				zpColor4f::Lerp( particle->color, effect->startColor, effect->endColor, sLife );
				break;

			case ZP_PARTICLE_EFFECT_RANGE_SPEED:
				zpColor4f::Lerp( particle->color, effect->startColor, effect->endColor, sSpeed );
				break;
			}

			// blend between start and end scale based on life or speed
			switch( effect->scaleRange )
			{
			case ZP_PARTICLE_EFFECT_RANGE_CONSTANT:
				particle->scale = effect->startScale;
				break;

			case ZP_PARTICLE_EFFECT_RANGE_RANDOM:
				particle->scale = zpMath::ScalarLerp( effect->startScale, effect->endScale, zpMath::Scalar( m_random->randomFloat( 0.f, 1.f ) ) );
				break;

			case ZP_PARTICLE_EFFECT_RANGE_LIFETIME:
				particle->scale = zpMath::ScalarLerp( effect->startScale, effect->endScale, sLife );
				break;

			case ZP_PARTICLE_EFFECT_RANGE_SPEED:
				particle->scale = zpMath::ScalarLerp( effect->startScale, effect->endScale, sSpeed );
				break;
			}
		}

		// remove dead particles
		for( zp_int i = 0; i < (zp_int)effect->usedParticles.size(); )
		{
			zpParticle* p = effect->usedParticles[ i ];
			if( p->life < 0.f )
			{
				m_freeParticles.pushBack( p );
				effect->usedParticles.erase( i );
				continue;
			}

			++i;
		}

		// sort new particles to the back
		effect->usedParticles.sort( []( zpParticle* a, zpParticle* b )
		{
			return a->life > b->life;
		} );
	}
}
void zpParticleEmitterComponent::onSimulate()
{

}

void zpParticleEmitterComponent::onEnabled()
{

}
void zpParticleEmitterComponent::onDisabled()
{

}

zp_bool zpParticleEmitterComponent::createEffect( const zp_char* name, const zpBison::Value& effectDef )
{
	zp_bool ok = true;

	zpParticleEffect& effect = m_effects.pushBackEmpty();
	zp_zero_memory( &effect );

	effect.name = name;

	effect.gravity = zpBisonArray3ToVector4( effectDef[ "Gravity" ], 0.f );
	effect.shapeSize = zpBisonArray4ToVector4( effectDef[ "ShapeSize" ] );

	effect.startScale = zpBisonArray2ToVector4( effectDef[ "StartScale" ], 1.f, 1.f );
	effect.endScale = zpBisonArray2ToVector4( effectDef[ "EndScale" ], 1.f, 1.f );

	zpBisonArray4ToColor4( effectDef[ "StartColor" ], effect.startColor );
	zpBisonArray4ToColor4( effectDef[ "EndColor" ], effect.endColor );

	effect.delay = effectDef[ "Delay" ].asFloat();
	effect.duration = effectDef[ "Duration" ].asFloat();
	effect.time = 0.f;

	effect.minSpeed = effectDef[ "MinSpeed" ].asFloat();
	effect.maxSpeed = effectDef[ "MaxSpeed" ].asFloat();

	effect.inheritVelocityScale = effectDef[ "InheritVelocityScale" ].asFloat();

	effect.minLife = effectDef[ "MinLife" ].asFloat();
	effect.maxLife = effectDef[ "MaxLife" ].asFloat();

	effect.sortBias = (zp_ushort)effectDef[ "SortBias" ].asInt();
	effect.minEmitRate = effectDef[ "MinEmitRate" ].asFloat();
	effect.maxEmitRate = effectDef[ "MaxEmitRate" ].asFloat();

	effect.flipbookSpeed = effectDef[ "FlipbookSpeed" ].asFloat();
	effect.flipbookX = effectDef[ "FlipbookX" ].asInt();
	effect.flipbookY = effectDef[ "FlipbookY" ].asInt();

	effect.scaleRange = ZP_PARTICLE_EFFECT_RANGE_LIFETIME;
	effect.colorRange = ZP_PARTICLE_EFFECT_RANGE_LIFETIME;

	zpBisonStringToEnum( g_shape, effectDef[ "Shape" ], effect.shape );
	effect.emitFromShell = effectDef[ "EmitFromShell" ].asBool();
	effect.randomDirection = effectDef[ "RandomDirection" ].asBool();
	effect.isLooping = effectDef[ "IsLooping" ].asBool();
	effect.preWarm = effectDef[ "PreWarm" ].asBool();
	effect.useRealTime = effectDef[ "UseRealTime" ].asBool();
	effect.isWorldSpace = effectDef[ "IsWorldSpace" ].asBool();
	effect.isBillboard = effectDef[ "IsBillboard" ].asBool();
	effect.playOnCreate = effectDef[ "PlayOnCreate" ].asBool();
	effect.useDepthSort = effectDef[ "UseDepthSort" ].asBool();

	effect.state = ZP_PARTICLE_EFFECT_STATE_DISABLED;

	const zp_char* materialName = effectDef[ "Material" ].asCString();
	ok = getApplication()->getRenderPipeline()->getMaterialContentManager()->getResource( materialName, effect.material );

	return ok;
}

void zpParticleEmitterComponent::playEffect( zpParticleEffect* effect )
{
	effect->state = ZP_PARTICLE_EFFECT_STATE_STARTING;
	effect->time = 0.f;
}

void zpParticleEmitterComponent::emitParticle( zpParticleEffect* effect )
{
	if( m_freeParticles.isEmpty() ) return;

	zpParticle* particle = m_freeParticles.back();
	m_freeParticles.popBack();
	effect->usedParticles.pushBack( particle );

	zpVector4f pos, norm;
	zpTransformComponent* transform = getParentObject()->getComponents()->getTransformComponent();
	pos = effect->isWorldSpace ? transform->getWorldPosition() : transform->getLocalPosition();

	switch( effect->shape )
	{
	case ZP_PARTICLE_EFFECT_SHAPE_SPHERE:
		{
			norm = m_random->randomUnitSphere( 0.f );
			if( effect->emitFromShell )
			{
				norm = zpMath::Vector4Normalize3( norm );
			}
			norm = zpMath::Vector4Mul( norm, effect->shapeSize );
			pos = zpMath::Vector4Add( pos, norm );

			if( effect->randomDirection )
			{
				norm = m_random->randomUnitSphere( 0.f );
			}
		}
		break;

	case ZP_PARTICLE_EFFECT_SHAPE_HEMISPHERE:
		{

		}
		break;

	case ZP_PARTICLE_EFFECT_SHAPE_CONE:
		{
			norm = effect->shapeSize;
			pos = zpMath::Vector4Add( pos, m_random->randomVector( zpMath::Vector4( 0, 0, 0, 0 ), norm ) );
		}
		break;

	case ZP_PARTICLE_EFFECT_SHAPE_BOX:
		{
			zpVector4f half, negHalf;
			half = zpMath::Vector4Mul( effect->shapeSize, zpMath::Scalar( 0.5f ) );
			negHalf = zpMath::Vector4Mul( effect->shapeSize, zpMath::Scalar( -0.5f ) );

			norm = m_random->randomVector( negHalf, half );
			pos = zpMath::Vector4Add( pos, norm );

			if( effect->randomDirection )
			{
				norm = m_random->randomUnitSphere( 0.f );
			}
		}
		break;
	}

	particle->normal = zpMath::Vector4Normalize3( norm );

	particle->position = pos;

	zpScalar sSpeed = zpMath::Scalar( m_random->randomFloat( effect->minSpeed, effect->maxSpeed ) );
	particle->velocity = zpMath::Vector4Mul( particle->normal, sSpeed );

	particle->scale = effect->startScale;
	particle->color = effect->startColor;

	particle->maxLife = m_random->randomFloat( effect->minLife, effect->maxLife );
	particle->life = particle->maxLife;

	particle->maxSpeed = m_random->randomFloat( effect->minSpeed, effect->maxSpeed );
}

zpParticleEmitterComponentPool::zpParticleEmitterComponentPool()
{}
zpParticleEmitterComponentPool::~zpParticleEmitterComponentPool()
{}
void zpParticleEmitterComponentPool::update( zp_float deltaTime, zp_float realTime )
{
	m_used.foreach( [ &deltaTime, &realTime ]( zpParticleEmitterComponent* o )
	{
		o->update( deltaTime, realTime );
	} );
}
void zpParticleEmitterComponentPool::simulate()
{
	m_used.foreach( []( zpParticleEmitterComponent* o )
	{
		o->simulate();
	} );
}

void zpParticleEmitterComponentPool::render( zpRenderingContext* i, const zpCamera* camera )
{
	m_used.foreach( [ i, camera ]( zpParticleEmitterComponent* o )
	{
		o->render( i, camera );
	} );
}
