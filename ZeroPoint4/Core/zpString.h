#pragma once
#ifndef ZP_STRING_H
#define ZP_STRING_H

class zpString {
public:
	zpString();
	~zpString();

private:
	zp_char* m_buff;
	zp_uint m_capacity;
	zp_uint m_length;
};

#endif