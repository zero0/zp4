#include "zpCore.h"

#if ZP_WIN_32
#include <windows.h>
#include <dbghelp.h>
#endif

zpStackTrace::zpStackTrace()
{
#if ZP_WIN_32
	m_count = CaptureStackBackTrace( 0, ZP_STACK_TRACE_MAX_TRACES, m_stackTrace, (PDWORD)&m_hash );
#endif
}
zpStackTrace::~zpStackTrace()
{}

void zpStackTrace::print() const
{
#if ZP_WIN_32
	HANDLE process;
	process = GetCurrentProcess();

	const zp_uint maxNameLen = 255;

	zp_byte symbolBuff[ sizeof( SYMBOL_INFO ) + maxNameLen * sizeof( zp_char ) ];
	zp_zero_memory_array( symbolBuff );

	PSYMBOL_INFO symbol = (PSYMBOL_INFO)symbolBuff;
	symbol->MaxNameLen = maxNameLen;
	symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

	for( zp_uint i = 0; i < m_count; ++i )
	{
		void* stack = m_stackTrace[ i ];

		SymFromAddr( process, (DWORD64)stack, 0, symbol );

		zp_printfln( "%i: %s - 0x%0X", m_count - i - 1, symbol->Name, stack );
	}
#endif
}
zp_uint zpStackTrace::size() const
{
	return m_count;
}
zp_hash zpStackTrace::hash() const
{
	return m_hash;
}

void zpStackTrace::Initialize()
{
#if ZP_WIN_32
	HANDLE process;
	zp_bool ok;
	
	process = GetCurrentProcess();
	
	SymSetOptions( SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES );
	ok = !!SymInitialize( process, ZP_NULL, true );
	ZP_ASSERT( ok, "Failed to initilize Sym" );
#endif
}
void zpStackTrace::Shutdown()
{
#if ZP_WIN_32
	HANDLE process;
	zp_bool ok;

	process = GetCurrentProcess();

	ok = !!SymCleanup( process );
	ZP_ASSERT( ok, "Failed to shutdown Sym" );
#endif
}
void zpStackTrace::Dump()
{
	zpStackTrace trace;
	trace.print();
}
