#pragma once
#ifndef ZP_KINEMATIC_BODY_H
#define ZP_KINEMATIC_BODY_H

class zpKinematicBody
{
public:
	zpKinematicBody();
	~zpKinematicBody();

	void create( const zpBison::Value& v );
	void initialize( const zpMatrix4f& transform );
	void destroy();

	void getMatrix( zpMatrix4f& transform ) const;
	void getPositionRotation( zpVector4f& position, zpQuaternion4f& rotation ) const;

	zp_short getGroup() const;
	zp_short getMask() const;

	void jump();
	zp_bool canJump() const;
	zp_bool onGround() const;

	void warp( const zpVector4f& position );

	void setWalkDirection( const zpVector4f& direction );

	zp_handle getKinematicController() const;
	zp_handle getCollisionGhost() const;

private:
	zp_handle m_controller;
	zp_handle m_ghost;
	zpCollider* m_collider;

	zp_float m_mass;
	zp_short m_group;
	zp_short m_mask;
};

#endif
