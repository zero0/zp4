#ifndef ZP_REAL_H
#define ZP_REAL_H

class zpReal {
public:
	zpReal();
	zpReal( zp_float real );
	zpReal( const zpReal& real );
	zpReal( zpReal&& real );
	~zpReal();

	void operator=( const zpReal& real );
	void operator=( zpReal&& real );

	zp_float getReal() const;

private:
	zp_real m_real;
};

#endif