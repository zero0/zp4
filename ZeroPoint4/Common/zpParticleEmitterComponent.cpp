#include "zpCommon.h"

const zp_char* g_shape[] =
{
	"Sphere",
	"Hemisphere",
	"Cone",
	"Box",
};

template< typename T, zp_uint count >
zp_bool _strToEnum( const zp_char* (&e)[ count ], const zpBison::Value& str, T& t )
{
	zp_bool ok = false;

	if( str.isString() )
	{
		const zp_char* s = str.asCString();
		for( zp_uint i = 0; i < count; ++i )
		{
			if( zp_strcmp( s, e[ i ] ) == 0 )
			{
				t = (T)i;
				ok = true;
				break;
			}
		}
	}

	return ok;
}

zpParticleEmitterComponent::zpParticleEmitterComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_layer( 0 )
	, m_queue( ZP_RENDERING_QUEUE_TRANSPARENT )
	, m_maxParticles( 0 )
	, m_random( zpRandom::getInstance() )
	, m_time( zpTime::getInstance() )
{
	m_layer = ZP_RENDERING_LAYER_DEFAULT; //1 << def[ "Layer" ].asInt();

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

void zpParticleEmitterComponent::render( zpRenderingContext* i, zpCamera* camera )
{
	zpParticleEffect* effect = m_effects.begin();
	zpParticleEffect* end = m_effects.end();
	for( ; effect != end; ++effect )
	{
		if( effect->state != ZP_PARTICLE_EFFECT_STATE_PLAYING ) continue;

		i->beginDrawImmediate( m_layer, m_queue, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, &effect->material );

		if( !effect->isWorldSpace )
		{
			i->setMatrix( getParentObject()->getTransform() );
		}
		i->setSortBias( effect->sortBias );

		zpParticle** p = effect->usedParticles.begin();
		zpParticle** e = effect->usedParticles.end();
		for( ; p != e; ++p )
		{
			zpParticle* particle = *p;

			zpMatrix4f wvp;
			wvp.setIdentity();
			
			// scale points out from the center
			zpVector4f p0( -1,  1, 0, 1 );
			zpVector4f p1(  1,  1, 0, 1 );
			zpVector4f p2(  1, -1, 0, 1 );
			zpVector4f p3( -1, -1, 0, 1 );

			zpMath::Mul( p0, p0, particle->scale );
			zpMath::Mul( p1, p1, particle->scale );
			zpMath::Mul( p2, p2, particle->scale );
			zpMath::Mul( p3, p3, particle->scale );

			// setup UV rect
			zpRectf uv( 0, 0, 1, 1 );

			// setup normals
			//const zp_float n = 0.70711f;
			//zpVector4f n0( -n, -n, n, 0 );
			//zpVector4f n1( -n,  n, n, 0 );
			//zpVector4f n2(  n, -n, n, 0 );
			//zpVector4f n3(  n,  n, n, 0 );

			// if billboard enabled, rotate towards camera
			if( effect->isBillboard )
			{
				zpVector4f look, right, up;

				zpMath::Sub( look, particle->position, camera->getPosition() );
				zpMath::Normalize3( look, look );
				zpMath::Cross3( right, camera->getUp(), look );
				zpMath::Normalize3( right, right );
				zpMath::Cross3( up, look, right );

				wvp.setRow( 0, right );
				wvp.setRow( 1, up );
				wvp.setRow( 2, look );
				wvp.setRow( 3, particle->position );
			}
			// otherwise, just set position
			else
			{
				wvp.setRow( 3, particle->position );
			}

			// get final point
			zpMath::Mul( p0, p0, wvp );
			zpMath::Mul( p1, p1, wvp );
			zpMath::Mul( p2, p2, wvp );
			zpMath::Mul( p3, p3, wvp );

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

void zpParticleEmitterComponent::onCreate()
{

}
void zpParticleEmitterComponent::onInitialize()
{
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

}

void zpParticleEmitterComponent::onUpdate()
{
	if( m_isPaused ) return;

	zpParticleEffect* effect = m_effects.begin();
	zpParticleEffect* end = m_effects.end();
	for( ; effect != end; ++effect )
	{
		if( effect->state == ZP_PARTICLE_EFFECT_STATE_DISABLED ) continue;

		// use either dt or rt based on the delta time flag
		zp_float dt = effect->useRealTime ? m_time->getActualDeltaSeconds() : m_time->getDeltaSeconds();

		// increment effect time
		effect->time += dt;

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
				effect->state = ZP_PARTICLE_EFFECT_STATE_DISABLED;
				continue;
			}
		}

		// increment emit time by the emit rate
		zp_float emitRate = dt * m_random->randomUInt( effect->minEmitRate, effect->maxEmitRate );
		effect->emitTime += dt;

		// if the time has passed the rate, emit a particle
		if( effect->emitTime > emitRate )
		{
			effect->emitTime = effect->emitTime - emitRate;
			emitParticle( effect );
		}

		zpScalar sdt( dt );

		// update particles
		zpParticle** p = effect->usedParticles.begin();
		zpParticle** e = effect->usedParticles.end();
		for( ; p != e; ++p )
		{
			zpParticle* particle = *p;

			particle->life -= dt;
			if( particle->life < 0.f ) continue;

			// calculate velocity, position and rotation
			zpMath::Madd( particle->velocity, particle->velocity, sdt, effect->gravity );
			zpMath::Madd( particle->position, particle->position, sdt, particle->velocity );
			zpMath::Madd( particle->rotation, particle->rotation, sdt, particle->angularVelocity );

			// get life and speed percents
			zpScalar sLife( particle->life ), sMaxLife( particle->maxLife ), sSpeed, sMaxSpeed( particle->maxSpeed );
			zpMath::Length3( sSpeed, particle->velocity );
			zpMath::Sub( sLife, sMaxLife, sLife );
			zpMath::Div( sLife, sLife, sMaxLife );
			zpMath::Div( sSpeed, sSpeed, sMaxSpeed );

			// blend between start and end color based on life or speed
			if( effect->colorRange & ZP_PARTICLE_EFFECT_RANGE_LIFETIME )
			{
				zpColor4f::Lerp( particle->color, effect->startColor, effect->endColor, sLife );
			}
			if( effect->colorRange & ZP_PARTICLE_EFFECT_RANGE_SPEED )
			{
				zpColor4f::Lerp( particle->color, effect->startColor, effect->endColor, sSpeed );
			}

			// blend between start and end scale based on life or speed
			if( effect->scaleRange & ZP_PARTICLE_EFFECT_RANGE_LIFETIME )
			{
				zpMath::Lerp( particle->scale, effect->startScale, effect->endScale, sLife );
			}
			if( effect->scaleRange & ZP_PARTICLE_EFFECT_RANGE_SPEED )
			{
				zpMath::Lerp( particle->scale, effect->startScale, effect->endScale, sSpeed );
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

ZP_FORCE_INLINE zpVector4f _BisonArray2ToVector4( const zpBison::Value& vec, zp_float z, zp_float w )
{
	return zpVector4f( vec[0].asFloat(), vec[1].asFloat(), z, w );
}
ZP_FORCE_INLINE zpVector4f _BisonArray3ToVector4( const zpBison::Value& vec, zp_float w )
{
	return zpVector4f( vec[0].asFloat(), vec[1].asFloat(), vec[2].asFloat(), w );
}
ZP_FORCE_INLINE zpVector4f _BisonArray4ToVector4( const zpBison::Value& vec )
{
	return zpVector4f( vec[0].asFloat(), vec[1].asFloat(), vec[2].asFloat(), vec[3].asFloat() );
}
ZP_FORCE_INLINE zpColor4f _BisonArray4ToColor4( const zpBison::Value& color )
{
	return zpColor4f( color[0].asFloat(), color[1].asFloat(), color[2].asFloat(), color[3].asFloat() );
}

zp_bool zpParticleEmitterComponent::createEffect( const zp_char* name, const zpBison::Value& effectDef )
{
	zp_bool ok = true;

	zpParticleEffect& effect = m_effects.pushBackEmpty();
	zp_zero_memory( &effect );

	effect.name = name;

	effect.gravity = _BisonArray3ToVector4( effectDef[ "Gravity" ], 0.f );
	effect.shapeSize = _BisonArray4ToVector4( effectDef[ "ShapeSize" ] );

	effect.startScale = _BisonArray2ToVector4( effectDef[ "StartScale" ], 0.f, 1.f );
	effect.endScale = _BisonArray2ToVector4( effectDef[ "EndScale" ], 0.f, 1.f );

	effect.startColor = _BisonArray4ToColor4( effectDef[ "StartColor" ] );
	effect.endColor = _BisonArray4ToColor4( effectDef[ "EndColor" ] );

	effect.delay = effectDef[ "Delay" ].asFloat();
	effect.duration = effectDef[ "Duration" ].asFloat();
	effect.time = 0.f;

	effect.minSpeed = effectDef[ "MinSpeed" ].asFloat();
	effect.maxSpeed = effectDef[ "MaxSpeed" ].asFloat();

	effect.inheritVelocityScale = effectDef[ "InheritVelocityScale" ].asFloat();

	effect.minLife = effectDef[ "MinLife" ].asFloat();
	effect.maxLife = effectDef[ "MaxLife" ].asFloat();

	effect.sortBias = (zp_ushort)effectDef[ "SortBias" ].asInt();
	effect.minEmitRate = effectDef[ "MinEmitRate" ].asInt();
	effect.maxEmitRate = effectDef[ "MaxEmitRate" ].asInt();

	effect.scaleRange = ZP_PARTICLE_EFFECT_RANGE_LIFETIME;
	effect.colorRange = ZP_PARTICLE_EFFECT_RANGE_LIFETIME;

	_strToEnum( g_shape, effectDef[ "Shape" ], effect.shape );
	effect.emitFromShell = effectDef[ "EmitFromShell" ].asBool();
	effect.randomDirection = effectDef[ "RandomDirection" ].asBool();
	effect.isLooping = effectDef[ "IsLooping" ].asBool();
	effect.preWarm = effectDef[ "PreWarm" ].asBool();
	effect.useRealTime = effectDef[ "UseRealTime" ].asBool();
	effect.isWorldSpace = effectDef[ "IsWorldSpace" ].asBool();
	effect.isBillboard = effectDef[ "IsBillboard" ].asBool();
	effect.playOnCreate = effectDef[ "PlayOnCreate" ].asBool();

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

	switch( effect->shape )
	{
	case ZP_PARTICLE_EFFECT_SHAPE_SPHERE:
		{
			zpVector4f pos;
			pos = effect->isWorldSpace ? getParentObject()->getTransform().getRow( 3 ) : zpVector4f( 0, 0, 0, 1 );

			zpVector4f norm( m_random->randomUnitSphere( 0.f ) );
			if( effect->emitFromShell )
			{
				zpMath::Normalize3( norm, norm );
			}
			zpMath::Mul( norm, norm, effect->shapeSize );
			zpMath::Add( particle->position, pos, norm );

			if( effect->randomDirection )
			{
				norm = m_random->randomUnitSphere( 0.f );
			}

			zpMath::Normalize3( norm, norm );
			zpScalar sSpeed( m_random->randomFloat( effect->minSpeed, effect->maxSpeed ) );
			zpMath::Mul( particle->velocity, norm, sSpeed );

			particle->scale = effect->startScale;
			particle->color = effect->startColor;
		}
		break;
	}

	particle->maxLife = m_random->randomFloat( effect->minLife, effect->maxLife );
	particle->life = particle->maxLife;

	particle->maxSpeed = m_random->randomFloat( effect->minSpeed, effect->maxSpeed );
}

zpParticleEmitterComponentPool::zpParticleEmitterComponentPool()
{}
zpParticleEmitterComponentPool::~zpParticleEmitterComponentPool()
{}
void zpParticleEmitterComponentPool::update()
{
	m_used.foreach( []( zpParticleEmitterComponent* o )
	{
		o->update();
	} );
}
void zpParticleEmitterComponentPool::simulate()
{
	m_used.foreach( []( zpParticleEmitterComponent* o )
	{
		o->simulate();
	} );
}

void zpParticleEmitterComponentPool::render( zpRenderingContext* i, zpCamera* camera )
{
	m_used.foreach( [ i, camera ]( zpParticleEmitterComponent* o )
	{
		o->render( i, camera );
	} );
}
