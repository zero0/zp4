#pragma once
#ifndef ZP_REFERENCED_POINTER_H
#define ZP_REFERENCED_POINTER_H

template<typename Ref>
class zpReferencedPointer {
public:
    zpReferencedPointer( Ref* ptr = ZP_NULL ) : m_ptr( ptr ) {
        if( m_ptr ) m_ptr->addReference();
    }
    zpReferencedPointer( const zpReferencedPointer& ptr ) : m_ptr( ptr.m_ptr ) {
        if( m_ptr ) m_ptr->addReference();
    }
    ~zpReferencedPointer() {
        ZP_SAFE_REMOVE_REF( m_ptr );
    }

    zpReferencedPointer& operator=( Ref* ptr ) {
        if( ptr ) ptr->addReference();
        if( m_ptr ) m_ptr->removeReference();
        m_ptr = ptr;
        return (*this);
    }
    zpReferencedPointer& operator=( const zpReferencedPointer& ptr ) {
        if( ptr.m_ptr ) ptr.m_ptr->addReference();
        if( m_ptr ) m_ptr->removeReference();
        m_ptr = ptr.m_ptr;
        return (*this);
    }

    Ref* operator->() const {
        return m_ptr;
    }
    Ref& operator*() const {
        return (*m_ptr);
    }

    operator Ref*() const {
        return m_ptr;
    }
    operator zp_bool() const {
        return m_ptr != ZP_NULL;
    }

    zp_bool operator==( Ref* prt ) const {
        return m_ptr == ptr;
    }
    zp_bool operator==( const zpReferencedPointer& ptr ) {
        return m_ptr == ptr.m_ptr;
    }

    zp_bool operator!=( Ref* prt ) const {
        return m_ptr != ptr;
    }
    zp_bool operator!=( const zpReferencedPointer& ptr ) {
        return m_ptr != ptr.m_ptr;
    }

private:
    Ref* m_ptr;
};

#endif