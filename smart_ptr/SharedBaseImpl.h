#ifndef SHAREDBASEIMPL_H_
#define SHAREDBASEIMPL_H_

#include "SharedCountBase.h"

template<class X> 
class sp_counted_impl_p : public sp_counted_base
{
private:
    sp_counted_impl_p( sp_counted_impl_p const & );
    sp_counted_impl_p & operator= ( sp_counted_impl_p const & );

    typedef sp_counted_impl_p<X> this_type;
public:

    explicit sp_counted_impl_p( X * px )
        : px_( px )
    {

    }

    virtual void dispose() // nothrow
    {
        delete px_;
    }

private:
    X * px_;
};

#endif // SHAREDBASEIMPL_H_
 