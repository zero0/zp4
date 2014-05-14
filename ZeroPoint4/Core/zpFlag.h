#pragma once
#ifndef ZP_FLAG_H
#define ZP_FLAG_H

template<typename F>
class zpFlag {
public:
	zpFlag() : m_flag( 0 ) {}
	zpFlag( F flag ) : m_flag( flag ) {}
	zpFlag( const zpFlag& flag ) : m_flag( flag.m_flag ) {}
	zpFlag( zpFlag&& flag ) : m_flag( flag.m_flag ) {}
	~zpFlag() {}

	operator F() const { return m_flag; }
	void operator=( const zpFlag& flag ) { m_flag = flag; }
	void operator=( zpFlag&& flag ) { m_flag = flag; }

	void mark( zp_uint index ) { markAll( (F)( 1 << index ) ); }
	void markAll( F mask ) { or( mask ); }

	void unmark( zp_uint index ) { unmarkAll( (F)( 1 << index ) ); }
	void unmarkAll( F mask ) { and( ~mask ); }

	void setMarked( zp_uint index, zp_bool marked ) { marked ? mark( index ) : unmark( index ); }
	zp_bool toggle( zp_uint index ) { xor( (F)( 1 << index ) ); return isMarked( index ); }

	zp_bool isMarked( zp_uint index ) const { return isAllMarked( (F)( 1 << index ) ); }
	zp_bool isAnyMarked( F mask ) const { return ( m_flag & mask ) != 0; }
	zp_bool isAllMarked( F mask ) const { return ( m_flag & mask ) == mask; }
	zp_bool isZero() const { return m_flag == (F)0; };

	void clear() { m_flag = 0; }

	void set( F flag ) { m_flag = flag; }
	void and( F flag ) { m_flag &= flag; }
	void or( F flag ) { m_flag |= flag; }
	void xor( F flag ) { m_flag ^= flag; }

	zp_bool operator==( const zpFlag& flag ) const { return m_flag == flag.m_flag; }
	zp_bool operator!=( const zpFlag& flag ) const { return m_flag != flag.m_flag; }

private:
	F m_flag;
};

typedef zpFlag<zp_byte> zpFlag8;
typedef zpFlag<zp_ushort> zpFlag16;
typedef zpFlag<zp_uint> zpFlag32;
typedef zpFlag<zp_ulong> zpFlag64;

#endif