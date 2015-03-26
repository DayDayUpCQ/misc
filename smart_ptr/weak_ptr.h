#ifndef WEAK_PTR_H_
#define WEAK_PTR_H_

#include "SharedCount.h"

template<class T>
class shared_ptr;


template<class T> 
class weak_ptr
{
private:
    typedef weak_ptr<T> this_type;
    template<class Y> friend class shared_ptr;
public:

    typedef T element_type;

    weak_ptr()
        : px_(0), pn_() // never throws in 1.30+
    {
    }
    //
    //  The "obvious" converting constructor implementation:
    //
    //  template<class Y>
    //  weak_ptr(weak_ptr<Y> const & r): px(r.px), pn(r.pn) // never throws
    //  {
    //  }
    //
    //  has a serious problem.
    //
    //  r.px may already have been invalidated. The px(r.px)
    //  conversion may require access to *r.px (virtual inheritance).
    //
    //  It is not possible to avoid spurious access violations since
    //  in multithreaded programs r.px may be invalidated at any point.
    //

    template<class Y>
    weak_ptr( weak_ptr<Y> const & r )
        : px_(r.lock().get()), pn_(r.pn_) // never throws
    {
    }

    template<class Y>
    weak_ptr( shared_ptr<Y> const & r )
        : px_( r.px_ ), pn_( r.pn_ ) // never throws
    {
    }

    template<class Y>
    weak_ptr & operator=(weak_ptr<Y> const & r) // never throws
    {
        px_ = r.lock().get();
        pn_ = r.pn_;
        return *this;
    }

    template<class Y>
    weak_ptr & operator=(shared_ptr<Y> const & r) // never throws
    {
        px_ = r.px_;
        pn_ = r.pn_;
        return *this;
    }

    shared_ptr<T> lock() const // never throws
    {
        return shared_ptr<element_type>( *this );
    }

    long use_count() const // never throws
    {
        return pn_.use_count();
    }

    bool expired() const // never throws
    {
        return pn_.use_count() == 0;
    }

    bool _empty() const // extension, not in std::weak_ptr
    {
        return pn_.empty();
    }

    void reset() // never throws in 1.30+
    {
        this_type().swap(*this);
    }

    void swap(this_type & other) // never throws
    {
        std::swap(px_, other.px_);
        pn_.swap(other.pn_);
    }


    T * px_;                       // contained pointer
    weak_count pn_; // reference counter

};  // weak_ptr

#endif // WEAK_PTR_H_
 