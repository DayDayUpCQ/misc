#ifndef SHAREDCOUNTBASE_H_
#define SHAREDCOUNTBASE_H_

#include <windows.h>

class sp_counted_base
{
private:

    sp_counted_base( sp_counted_base const & );
    sp_counted_base & operator= ( sp_counted_base const & );

    long use_count_;        // #shared
    long weak_count_;       // #weak + (#shared != 0)

public:

    sp_counted_base()
        : use_count_( 1 ), weak_count_( 1 )
    {
    }

    virtual ~sp_counted_base() // nothrow
    {
    }

    // dispose() is called when use_count_ drops to zero, to release
    // the resources managed by *this.

    virtual void dispose() = 0; // nothrow

    // destroy() is called when weak_count_ drops to zero.

    virtual void destroy() // nothrow
    {
        delete this;
    }

    void add_ref_copy()
    {
        InterlockedIncrement( &use_count_ );
    }

    bool add_ref_lock() // true on success
    {
        for( ;; )
        {
            long tmp = static_cast< long const volatile& >( use_count_ );
            if( tmp == 0 ) 
                return false;
            long tmp2 = tmp + 1;
            if( InterlockedCompareExchange( &use_count_, tmp2, tmp ) == tmp2 - 1 ) 
                return true;
        }
    }

    void release() // nothrow
    {
        if( InterlockedDecrement( &use_count_ ) == 0 )
        {
            dispose();
            weak_release();
        }
    }

    void weak_add_ref() // nothrow
    {
        InterlockedIncrement( &weak_count_ );
    }

    void weak_release() // nothrow
    {
        if( InterlockedDecrement( &weak_count_ ) == 0 )
        {
            destroy();
        }
    }

    long use_count() const // nothrow
    {
        return static_cast<long const volatile &>( use_count_ );
    }
};

#endif // SHAREDCOUNTBASE_H_
 