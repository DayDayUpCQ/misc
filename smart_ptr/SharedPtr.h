#ifndef SHAREDPTR_H_
#define SHAREDPTR_H_

#include "SharedCount.h"

template<class T>
class weak_ptr;

template<class T> 
class shared_ptr
{
private:
    typedef shared_ptr<T> this_type;
    template<class Y> friend class weak_ptr;
public:
    typedef T element_type;
    typedef T value_type;
    typedef T * pointer;
    typedef T& reference;

    shared_ptr()
        : px_(0), pn_()
    {
    }

    template<class Y>
    explicit shared_ptr( Y * p )
        : px_( p ), pn_( p ) // Y must be complete
    {
        //boost::detail::sp_enable_shared_from_this( this, p, p );
    }
    //  generated copy constructor, destructor are fine

    template<class Y>
    explicit shared_ptr(weak_ptr<Y> const & r)
        : pn_(r.pn_) // may throw
    {
        // it is now safe to copy r.px, as pn(r.pn) did not throw
        px_ = r.px_;
    }

    template<class Y>
    shared_ptr( shared_ptr<Y> const & r )
        : px_( r.px_ ), pn_( r.pn_ ) // never throws
    {
    }

    // aliasing
    template< class Y >
    shared_ptr( shared_ptr<Y> const & r, T * p )
        : px_( p ), pn_( r.pn_ ) // never throws
    {
    }

    // assignment
    shared_ptr & operator=( shared_ptr const & r ) // never throws
    {
        this_type(r).swap(*this);
        return *this;
    }

    template<class Y>
    shared_ptr & operator=(shared_ptr<Y> const & r) // never throws
    {
        this_type(r).swap(*this);
        return *this;
    }

    void reset() // never throws in 1.30+
    {
        this_type().swap(*this);
    }

    void reset(T* p) // never throws in 1.30+
    {
        assert(p == 0 || p != px_); // catch self-reset errors
        this_type(p).swap(*this);
    }

    template<class Y> void reset(Y * p) // Y must be complete
    {
        assert(p == 0 || p != px_); // catch self-reset errors
        this_type(p).swap(*this);
    }

    T& operator* () const // never throws
    {
        assert(px_ != 0);
        return *px_;
    }

    T * operator-> () const // never throws
    {
        assert(px_ != 0);
        return px_;
    }

    T * get() const // never throws
    {
        return px_;
    }

    bool unique() const // never throws
    {
        return pn_.unique();
    }

    long use_count() const // never throws
    {
        return pn_.use_count();
    }

    void swap(shared_ptr<T> & other) // never throws
    {
        std::swap(px_, other.px_);
        pn_.swap(other.pn_);
    }

    operator bool () const
    {
        return px_ != 0;
    }

private:

    T * px_;                     // contained pointer
    shared_count pn_;            // reference counter

};

#endif // SHAREDPTR_H_
 
 