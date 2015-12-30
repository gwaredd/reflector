////////////////////////////////////////////////////////////////////////////////
//
// ReflectContext keeps hold of the "context" (whether we are reflecting symbols
// or not) and parsing the special %%reflect comments
//

#pragma once


// parses %%reflect comments and works out context

class ReflectContext
{
    public:

        bool ParseComment( comments::FullComment* comment );

        void SetDefault( bool reflect )     { mReflect = reflect; }

        void OutputAttrs( JSON& json );


    protected:

        StringRef NextWord();
        void SkipWhitespace();
        void SkipPunct();
        void SkipIf( char ch );

        bool mReflect;

        const char* mP;
        std::vector< std::pair< std::string, std::string > > mAttrs;

    public:

        ReflectContext()
            : mReflect( true )
            , mP( nullptr )
        {
        }
};

