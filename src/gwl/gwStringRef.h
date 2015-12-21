////////////////////////////////////////////////////////////////////////////////
// StringRef
//
//  "Pascal-style" strings (pointer and length rather than 'C' null terminated).
//  Useful for processing text 'in-place' without having to allocate memory
//
//  Inspired by LLVM::StringRef
//

#pragma once

#include "gw/gw.h"
#include "gw/gwstring.h"

#include <string.h>
#include <ctype.h>
#include <string>

namespace gw
{
    ////////////////////////////////////////////////////////////////////////////////

    class StringRef
    {
        public:

            typedef const gwchar *iterator;
            typedef const gwchar *const_iterator;

            iterator begin() const  { return mData; }
            iterator end() const    { return mData + mLength; }


        public:

            StringRef& operator=( const StringRef& str )
            {
                mData    = str.mData;
                mLength  = str.mLength;

                return *this;
            }

            StringRef& operator++()
            {
                if( NotEmpty() )
                {
                    mData++;
                    mLength--;
                }

                return *this;
            }

            gwchar operator[]( unsigned int index ) const
            {
                return index < (unsigned int) mLength ? mData[ index ] : '\0';
            }

            StringRef operator++( int )
            {
                auto ToReturn = *this;
                operator++();
                return ToReturn;
            }

            operator std::string() const
            {
                return Str();
            }


        public:

            int Length() const
            {
                return mLength;
            }

            const gwchar* Data() const
            {
                return mData;
            }

            std::string Str() const
            {
                return IsEmpty() ? std::string() : std::string( mData, mLength );
            }

            void CopyTo( char* buffer, int len )
            {
                gw_strncpy( buffer, len, mData, mLength );
            }

            bool IsEmpty() const
            {
                return mLength == 0;
            }

            bool NotEmpty() const
            {
                return mLength > 0;
            }

            bool IsNumber() const
            {
                return mLength > 0 && isdigit( (unsigned char) *mData ) != 0;
            }

            bool IsWord() const
            {
                return mLength > 0 && ( isalpha( (unsigned char) *mData ) != 0 || *mData == '_' );
            }

            bool Equals( const StringRef& str ) const
            {
                return mLength == str.mLength && ( mLength == 0 || gw_strnicmp( mData, str.mData, mLength ) == 0 );
            }

            int Compare( const StringRef& str ) const;

        public:

            StringRef& Trim();

            StringRef& SkipIf( const gwchar ch )
            {
                if( NotEmpty() && *mData == ch )
                {
                    mData++;
                    mLength--;
                }

                return *this;
            }

            StringRef& ShrinkIf( const gwchar ch )
            {
                if( NotEmpty() && mData[ mLength - 1 ] == ch )
                {
                    mLength--;
                }

                return *this;
            }

            StringRef Split( const gwchar ch );
        
            // NB: use with caution!

            const gwchar* TerminateInPlace()
            {
                if( mData )
                {
                    const_cast<char*>(mData)[ mLength ] = '\0';
                }
                
                return mData;
            }


        protected:

            const gwchar*   mData;
            int             mLength;


        public:

            StringRef()
                : mData( nullptr )
                , mLength( 0 )
            {
            }

            StringRef( const StringRef& str )
                : mData( str.mData )
                , mLength( str.mLength )
            {
            }

            StringRef( const gwchar* str )
                : mData( str )
                , mLength( str ? gw_strlen( str ) : 0 )
            {
            }

            StringRef( const gwchar* str, int _length )
                : mData( str )
                , mLength( _length )
            {
            }

            StringRef( const gwchar* start, const gwchar* end )
                : mData( start )
                , mLength( int( end - start ) )
            {
            }

            StringRef( const unsigned char* start, const unsigned char* end )
                : mData( (const char*) start )
                , mLength( int( end - start ) )
            {
            }

            StringRef( const std::string& str )
                : mData( str.c_str() )
                , mLength( (int) str.size() )
            {
            }
    };


    ////////////////////////////////////////////////////////////////////////////////
    // operators

    inline bool operator==( const StringRef& LHS, const StringRef& RHS )
    {
        return LHS.Equals( RHS );
    }

    inline bool operator!=( const StringRef& LHS, const StringRef& RHS )
    {
        return !LHS.Equals( RHS );
    }

    inline bool operator<( const StringRef& LHS, const StringRef& RHS )
    {
        return LHS.Compare( RHS ) < 0;
    }

    inline bool operator<=( const StringRef& LHS, const StringRef& RHS )
    {
        return LHS.Compare( RHS ) <= 0;
    }

    inline bool operator>( const StringRef& LHS, const StringRef& RHS )
    {
        return LHS.Compare( RHS ) > 0;
    }

    inline bool operator>=( const StringRef& LHS, const StringRef& RHS )
    {
        return LHS.Compare( RHS ) >= 0;
    }
}

