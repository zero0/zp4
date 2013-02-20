
struct zpScalar
{
	zpScalar() {}
	explicit zpScalar( const zp_float x )
		: m_x( x )
	{}
	zpScalar( const zpScalar& x )
		: m_x( x.m_x )
	{}
	zpScalar( zpScalar&& x )
		: m_x( x.m_x )
	{}

	zp_float getFloat() const { return m_x; }

	friend void zpMath::Add( zpScalar& s, const zpScalar& a, const zpScalar& b );
	friend void zpMath::Sub( zpScalar& s, const zpScalar& a, const zpScalar& b );
	friend void zpMath::Mul( zpScalar& s, const zpScalar& a, const zpScalar& b );
	friend void zpMath::Div( zpScalar& s, const zpScalar& a, const zpScalar& b );

	friend void zpMath::Add( zpVector4f& s, const zpVector4f& a, const zpScalar& b );
	friend void zpMath::Sub( zpVector4f& s, const zpVector4f& a, const zpScalar& b );
	friend void zpMath::Mul( zpVector4f& s, const zpVector4f& a, const zpScalar& b );
	friend void zpMath::Div( zpVector4f& s, const zpVector4f& a, const zpScalar& b );

	friend void zpMath::Add( zpVector4f& s, const zpScalar& a, const zpVector4f& b );
	friend void zpMath::Sub( zpVector4f& s, const zpScalar& a, const zpVector4f& b );
	friend void zpMath::Mul( zpVector4f& s, const zpScalar& a, const zpVector4f& b );
	friend void zpMath::Div( zpVector4f& s, const zpScalar& a, const zpVector4f& b );

	friend void zpMath::Madd( zpVector4f& s, const zpVector4f& a, const zpVector4f& b, const zpScalar& c );
	friend void zpMath::Madd( zpVector4f& s, const zpVector4f& a, const zpScalar& b, const zpVector4f& c );

	friend void zpMath::Dot2( zpScalar& s, const zpVector4f& a, const zpVector4f& b );
	friend void zpMath::Dot3( zpScalar& s, const zpVector4f& a, const zpVector4f& b );
	friend void zpMath::Dot4( zpScalar& s, const zpVector4f& a, const zpVector4f& b );

	friend void zpMath::LengthSquared2( zpScalar& s, const zpVector4f& a );
	friend void zpMath::LengthSquared3( zpScalar& s, const zpVector4f& a );

	friend void zpMath::Length2( zpScalar& s, const zpVector4f& a );
	friend void zpMath::Length3( zpScalar& s, const zpVector4f& a );

	friend void zpMath::Abs( zpScalar& s, const zpScalar& a );
	friend void zpMath::Neg( zpScalar& s, const zpScalar& a );
	friend void zpMath::Rcp( zpScalar& s, const zpScalar& a );
	friend void zpMath::Max( zpScalar& s, const zpScalar& a, const zpScalar& b );
	friend void zpMath::Min( zpScalar& s, const zpScalar& a, const zpScalar& b );
	friend void zpMath::Cmp( zp_int& s, const zpScalar& a, const zpScalar& b );

private:
	zp_float m_x, m_y, m_z, m_w;
};

struct zp_vec4
{
	ZP_FORCE_INLINE zp_vec4()
	{}
	ZP_FORCE_INLINE zp_vec4( zp_float _x, zp_float _y, zp_float _z, zp_float _w )
		: x( _x )
		, y( _y )
		, z( _z )
		, w( _w )
	{}

	template<zp_int I>
	ZP_FORCE_INLINE zpScalar get() const
	{
		return zpScalar( v[I] );
	}
	ZP_FORCE_INLINE zpScalar getX() const
	{
		return zpScalar( x );
	}
	ZP_FORCE_INLINE zpScalar getY() const
	{
		return zpScalar( y );
	}
	ZP_FORCE_INLINE zpScalar getZ() const
	{
		return zpScalar( z );
	}
	ZP_FORCE_INLINE zpScalar getW() const
	{
		return zpScalar( w );
	}

	template<zp_int I>
	ZP_FORCE_INLINE void set( const zpScalar& s )
	{
		v[ I ] = s.getFloat();
	}
	ZP_FORCE_INLINE void setX( const zpScalar& s )
	{
		x = s.getFloat();
	}
	ZP_FORCE_INLINE void setY( const zpScalar& s )
	{
		y = s.getFloat();
	}
	ZP_FORCE_INLINE void setZ( const zpScalar& s )
	{
		z = s.getFloat();
	}
	ZP_FORCE_INLINE void setW( const zpScalar& s )
	{
		w = s.getFloat();
	}

	ZP_FORCE_INLINE const zp_float* asFloat4() const
	{
		return v;
	}
	
	ZP_FORCE_INLINE void operator=( const zp_float* m )
	{
		v[ 0 ] = m[ 0 ];
		v[ 1 ] = m[ 1 ];
		v[ 2 ] = m[ 2 ];
		v[ 3 ] = m[ 3 ];
	}
	ZP_FORCE_INLINE void operator=( const zp_vec4& m )
	{
		x = m.x;
		y = m.y;
		z = m.z;
		w = m.w;
	}
	ZP_FORCE_INLINE void operator=( zp_vec4&& m )
	{
		x = m.x;
		y = m.y;
		z = m.z;
		w = m.w;
	}

private:
	union
	{
		zp_float v[4];
		struct
		{
			zp_float x, y, z, w;
		};
	};
};
