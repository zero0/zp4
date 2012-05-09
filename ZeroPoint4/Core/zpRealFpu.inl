
zpReal::zpReal() {}
zpReal::zpReal( zp_float real ) : m_real( real ) {}
zpReal::zpReal( const zpReal& real ) : m_real( real.m_real ) {}
zpReal::zpReal( zpReal&& real ) : m_real( real.m_real ) {}
zpReal::~zpReal() {}

void zpReal::operator=( const zpReal& real ) {
	m_real = real.m_real;
}
void zpReal::operator=( zpReal&& real ) {
	m_real = real.m_real;
}

zp_float zpReal::getReal() const {
	return m_real;
}
