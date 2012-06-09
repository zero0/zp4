#pragma once
#ifndef ZP_PAIR_H
#define ZP_PAIR_H

template<typename First, typename Second>
class zpPair {
public:
	zpPair();
	zpPair( const First& first, const Second& second );
	zpPair( First&& first, Second&& second );
	zpPair( First&& first, const Second& second );
	zpPair( const First& first, Second&& second );
	zpPair( const zpPair& pair );
	zpPair( zpPair&& pair );
	~zpPair();

	template<typename F, typename S>
	void operator=( const zpPair<F, S>& pair );
	void operator=( zpPair&& pair );

	const First& first() const;
	const Second& second() const;

	First& first();
	Second& second();

	zp_bool operator==( const zpPair<First, Second>& pair ) const;
	zp_bool operator!=( const zpPair<First, Second>& pair ) const;

private:
	First m_first;
	Second m_second;
};

#include "zpPair.inl"

#endif