#pragma once
#ifndef ZP_HASH_MAP_H
#define ZP_HASH_MAP_H

template<typename Key, typename Value>
class zpHashMap {
public:
	zpHashMap();
	~zpHashMap();

private:
	zpArrayList<zpPair<Key, Value>> m_map;
};

#include "zpHashMap.inl"

#endif