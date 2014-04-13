#pragma once
#ifndef ZP_STACK_TRACE
#define ZP_STACK_TRACE

#define ZP_STACK_TRACE_MAX_TRACES	62

class zpStackTrace
{
public:
	zpStackTrace();
	~zpStackTrace();

	void print() const;
	zp_uint size() const;
	zp_hash hash() const;

	static void Initialize();
	static void Shutdown();
	static void Dump();

private:
	void* m_stackTrace[ ZP_STACK_TRACE_MAX_TRACES ];
	zp_uint m_count;
	zp_hash m_hash;
};

#endif
