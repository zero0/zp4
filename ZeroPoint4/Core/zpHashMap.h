#pragma once
#ifndef ZP_MAP_H
#define ZP_MAP_H

template<typename Key, typename Value>
class zpHashMap {
public:

private:
	zpArray<zpPair<Key, Value>> m_map;
};

#endif