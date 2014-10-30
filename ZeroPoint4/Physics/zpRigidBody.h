#ifndef ZP_RIGID_BODY_H
#define ZP_RIGID_BODY_H

class zpRigidBody
{
public:
	zpRigidBody();
	~zpRigidBody();

	void create( const zpBison::Value& v, zp_bool isStatic );
	void initialize( const zpMatrix4f& transform );
	void destroy();

	zp_bool getMatrix( zpMatrix4f& transform );

	zp_bool isStatic() const;
	zp_short getGroup() const;
	zp_short getMask() const;

	zp_handle getRigidBody() const;

private:
	zp_handle m_rigidBody;
	zp_handle m_motionState;
	zpCollider* m_collider;

	zp_bool m_isStatic;
	zp_float m_mass;
	zp_short m_group;
	zp_short m_mask;
};

class zpCollisionMask
{
public:
	~zpCollisionMask();

	static zpCollisionMask* getInstance();

	zp_short getCollisionMask( const zpString& maskName );
	zp_short getCollisionMask( const zp_char* maskName );

	void getCollisionMaskNames( zpArrayList< zpString >& names ) const;

private:
	zpCollisionMask();

	zpHashMap< zpString, zp_short> m_collisionMasks;
};
#endif
