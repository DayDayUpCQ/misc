#ifndef SHAREDCOUNT_H_
#define SHAREDCOUNT_H_

#include <assert.h>
#include "SharedCountBase.h"
#include "SharedBaseImpl.h"

class weak_count;

class shared_count
{
private:
    friend class weak_count;

public:

    shared_count()
        : pi_(0)
    {
    }

    template<class Y> 
    explicit shared_count( Y * p )
        : pi_( 0 )
    {
        pi_ = new sp_counted_impl_p<Y>( p );

        if( pi_ == 0 )
        {
            delete p;
        }
    }

    ~shared_count() // nothrow
    {
        if( pi_ != 0 ) 
            pi_->release();
    }

    shared_count(shared_count const & r)
        : pi_(r.pi_) // nothrow
    {
        if( pi_ != 0 ) 
            pi_->add_ref_copy();
    }

    explicit shared_count(weak_count const & r); // throws bad_weak_ptr when r.use_count() == 0

    shared_count & operator= (shared_count const & r) // nothrow
    {
        sp_counted_base * tmp = r.pi_;

        if( tmp != pi_ )
        {
            if( tmp != 0 )
                tmp->add_ref_copy();
            if( pi_ != 0 ) 
                pi_->release();
            pi_ = tmp;
        }

        return *this;
    }

    void swap(shared_count & r) // nothrow
    {
        sp_counted_base * tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long use_count() const // nothrow
    {
        return pi_ != 0 ? pi_->use_count(): 0;
    }

    bool unique() const // nothrow
    {
        return use_count() == 1;
    }

    bool empty() const // nothrow
    {
        return pi_ == 0;
    }

    friend inline bool operator==(shared_count const & a, shared_count const & b)
    {
        return a.pi_ == b.pi_;
    }

    //friend inline bool operator<(shared_count const & a, shared_count const & b)
    //{
    //    return std::less<sp_counted_base *>()( a.pi_, b.pi_ );
    //}
private:
    sp_counted_base * pi_;
};


class weak_count
{
private:
    friend class shared_count;

public:

    weak_count(): pi_(0) // nothrow
    {
    }

    weak_count(shared_count const & r)
        : pi_(r.pi_) // nothrow
    {
        if(pi_ != 0) 
            pi_->weak_add_ref();
    }

    weak_count(weak_count const & r)
        : pi_(r.pi_) // nothrow
    {
        if(pi_ != 0) 
            pi_->weak_add_ref();
    }

    ~weak_count() // nothrow
    {
        if(pi_ != 0) 
            pi_->weak_release();
    }

    weak_count & operator= (shared_count const & r) // nothrow
    {
        sp_counted_base * tmp = r.pi_;

        if( tmp != pi_ )
        {
            if(tmp != 0) 
                tmp->weak_add_ref();
            if(pi_ != 0) 
                pi_->weak_release();
            pi_ = tmp;
        }

        return *this;
    }

    weak_count & operator= (weak_count const & r) // nothrow
    {
        sp_counted_base * tmp = r.pi_;

        if( tmp != pi_ )
        {
            if(tmp != 0) 
                tmp->weak_add_ref();
            if(pi_ != 0) 
                pi_->weak_release();
            pi_ = tmp;
        }

        return *this;
    }

    void swap(weak_count & r) // nothrow
    {
        sp_counted_base * tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long use_count() const // nothrow
    {
        return pi_ != 0? pi_->use_count(): 0;
    }

    bool empty() const // nothrow
    {
        return pi_ == 0;
    }

    friend inline bool operator==(weak_count const & a, weak_count const & b)
    {
        return a.pi_ == b.pi_;
    }

    //friend inline bool operator<(weak_count const & a, weak_count const & b)
    //{
    //    return std::less<sp_counted_base *>()(a.pi_, b.pi_);
    //}
private:
    sp_counted_base * pi_;
};

inline shared_count::shared_count( weak_count const & r )
    : pi_( r.pi_ )
{
    if( pi_ == 0 || !pi_->add_ref_lock() )
    {
        assert(0); // need exception or something else.
    }
}

#endif // SHAREDCOUNT_H_
 