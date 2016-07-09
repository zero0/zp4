
template<typename First, typename Second>
zpPair<First, Second>::zpPair() : m_first(), m_second() {}
template<typename First, typename Second>
zpPair<First, Second>::zpPair( const First& first, const Second& second ) : m_first( first ), m_second( second ) {}
template<typename First, typename Second>
zpPair<First, Second>::zpPair( First&& first, Second&& second ) : m_first( (First&&)first ), m_second( (Second&&)second ) {}
template<typename First, typename Second>
zpPair<First, Second>::zpPair( First&& first, const Second& second ) : m_first( (First&&)first ), m_second( second ) {}
template<typename First, typename Second>
zpPair<First, Second>::zpPair( const First& first, Second&& second ) : m_first( first ), m_second( (Second&&)second ) {}
template<typename First, typename Second>
zpPair<First, Second>::zpPair( const zpPair& pair ) : m_first( pair.m_first ), m_second( pair.m_second ) {}
template<typename First, typename Second>
zpPair<First, Second>::zpPair( zpPair&& pair ) : m_first( (First&&)pair.m_first ), m_second( (Second&&)pair.m_second ) {}
template<typename First, typename Second>
zpPair<First, Second>::~zpPair() {}

template<typename First, typename Second> template<typename F, typename S>
void zpPair<First, Second>::operator=( const zpPair<F, S>& pair ) {
    m_first = pair.m_first;
    m_second = pair.m_second;
}
template<typename First, typename Second>
void zpPair<First, Second>::operator=( zpPair&& pair ) {
    if( this == &pair ) return;
    m_first = (First&&)pair.m_first;
    m_second = (Second&&)pair.m_second;
}

template<typename First, typename Second>
const First& zpPair<First, Second>::first() const {
    return m_first;
}
template<typename First, typename Second>
const Second& zpPair<First, Second>::second() const {
    return m_second;
}

template<typename First, typename Second>
First& zpPair<First, Second>::first() {
    return m_first;
}
template<typename First, typename Second>
Second& zpPair<First, Second>::second() {
    return m_second;
}

template<typename First, typename Second>
zp_bool zpPair<First, Second>::operator==( const zpPair<First, Second>& pair ) const {
    return m_first == pair.m_first && m_second == pair.m_second;
}
template<typename First, typename Second>
zp_bool zpPair<First, Second>::operator!=( const zpPair<First, Second>& pair ) const {
    return m_first != pair.m_first || m_second != pair.m_second;
}
