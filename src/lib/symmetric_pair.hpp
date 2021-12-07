#ifndef SYMMETRIC_PAIR_HPP
#define SYMMETRIC_PAIR_HPP

template <typename T>
struct symmetric_pair
{
    using this_type   = symmetric_pair<T>;
    using type        = T;
    using first_type  = type;
    using second_type = type;

    T first, second;

    symmetric_pair () = delete;

    explicit symmetric_pair ( const T& x, const T& y )
    : 
        first { x },
        second { y } 
    {};

    symmetric_pair ( const std::pair<T, T>& p )
    : 
        first { p.first },
        second { p.second } 
    {};

    symmetric_pair ( std::pair<T, T>&& p )
    : 
        first { p.first },
        second { p.second } 
    {};

    bool contains( T x, T y ) const
    {
        return ( first == x && second == y ) || ( first == y && second == x );
    }

    bool contains( const std::pair<T, T>& p ) const
    {
        return contains( p.first, p.second );
    }

    bool operator== ( const this_type& sp ) const
    {
        return contains( sp.first, sp.second );
    }

    bool contains( const this_type& sp ) const
    {
        return *this == sp;
    }

    this_type& reverse()
    {
        std::swap( first, second );
        return *this;
    }
};

template <typename T>
symmetric_pair<T> make_symmetric_pair( const T& x, const T& y )
{
    return symmetric_pair( x, y );
}

template <typename T>
symmetric_pair<T> make_symmetric_pair( const std::pair<T, T>& p )
{
    return symmetric_pair( p );
}

#endif  // SYMMETRIC_PAIR_HPP