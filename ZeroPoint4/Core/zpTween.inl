
template<typename T>
zpTween<T>::zpTween( const T& start ) :
    m_currentTime( 0.f ),
    m_tweenSteps(),
    m_startValue( start ),
    m_currentValue( start )
{}
template<typename T>
zpTween<T>::~zpTween() {
    clear();
}

template<typename T>
void zpTween<T>::update( zp_float deltaTime ) {
    if( m_tweenSteps.isEmpty() ) return;

    m_currentTime += deltaTime;

    zpTweenStep& step = m_tweenSteps.front();

    if( m_currentTime >= step.time ) {
        m_currentTime = 0.f;

        m_startValue = step.value;

        m_tweenSteps.popFront();
        if( m_tweenSteps.isEmpty() ) {
            m_currentValue = m_startValue;
            return;
        }

        step = m_tweenSteps.front();
    }

    T diff = step.value - m_startValue;

#define EASING_CASE( e )    case e: { m_currentValue = zpTweenFunctions::easing<e>( m_startValue, diff, m_currentTime, step.time ); } break
    
    switch( step.easing ) {
        EASING_CASE( ZP_TWEEN_EASING_LINEAR );

        EASING_CASE( ZP_TWEEN_EASING_QUADRATIC_IN );
        EASING_CASE( ZP_TWEEN_EASING_QUADRATIC_OUT );
        EASING_CASE( ZP_TWEEN_EASING_QUADRATIC_IN_OUT );

        EASING_CASE( ZP_TWEEN_EASING_CUBIC_IN );
        EASING_CASE( ZP_TWEEN_EASING_CUBIC_OUT );
        EASING_CASE( ZP_TWEEN_EASING_CUBIC_IN_OUT );

        EASING_CASE( ZP_TWEEN_EASING_QUARTIC_IN );
        EASING_CASE( ZP_TWEEN_EASING_QUARTIC_OUT );
        EASING_CASE( ZP_TWEEN_EASING_QUARTIC_IN_OUT );

        EASING_CASE( ZP_TWEEN_EASING_QUINTIC_IN );
        EASING_CASE( ZP_TWEEN_EASING_QUINTIC_OUT );
        EASING_CASE( ZP_TWEEN_EASING_QUINTIC_IN_OUT );

        EASING_CASE( ZP_TWEEN_EASING_SIN_IN );
        EASING_CASE( ZP_TWEEN_EASING_SIN_OUT );
        EASING_CASE( ZP_TWEEN_EASING_SIN_IN_OUT );

        EASING_CASE( ZP_TWEEN_EASING_EXP_IN );
        EASING_CASE( ZP_TWEEN_EASING_EXP_OUT );
        EASING_CASE( ZP_TWEEN_EASING_EXP_IN_OUT );

        EASING_CASE( ZP_TWEEN_EASING_CIRCULAR_IN );
        EASING_CASE( ZP_TWEEN_EASING_CIRCULAR_OUT );
        EASING_CASE( ZP_TWEEN_EASING_CIRCULAR_IN_OUT );
    };

#undef EASING_CASE

}

template<typename T>
void zpTween<T>::queue( zp_float time, const T& val, zpTweenEasing func ) {
    zpTweenStep step;
    step.easing = func;
    step.time = time;
    step.value = val;

    m_tweenSteps.pushBack( step );
}
template<typename T>
void zpTween<T>::clear() {
    m_tweenSteps.clear();
}

template<typename T>
const T& zpTween<T>::getCurrentValue() const {
    return m_currentValue;
}
template<typename T>
zp_bool zpTween<T>::isComplete() const {
    return m_tweenSteps.isEmpty();
}

template<typename T>
void zpTween<T>::setStartValue( const T& start ) {
    m_startValue = start;
}
