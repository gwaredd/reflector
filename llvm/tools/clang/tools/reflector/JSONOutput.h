////////////////////////////////////////////////////////////////////////////////
// cheesy JSON formatter

#pragma once

class JSON
{
    public:
        
        void BeginObject( const char* name = nullptr )
        {
            Comma();
            
            if( name )
            {
                llvm::outs() << "\"" << name << "\": ";
            }
            
            llvm::outs() << "{";
            
            BeginBlock();
        }
        
        void EndObject()
        {
            EndBlock();
            NewLine();
            llvm::outs() << "}";
        }
        
        void BeginArray( const char* name = nullptr )
        {
            Comma();
            
            if( name )
            {
                llvm::outs() << "\"" << name << "\":";
                NewLine();
            }
            
            llvm::outs() << "[";
            BeginBlock();
        }
        
        void EndArray()
        {
            EndBlock();
            NewLine();
            llvm::outs() << "]";
        }
        
        void Field( const char* name, const char* value )
        {
            Comma();
            
            if( name )
            {
                llvm::outs() << "\"" << name << "\": ";
            }
            
            llvm::outs() << "\"" << value << "\"";
        }
        
        
    protected:
        
        void Comma()
        {
            if( mFirst )
            {
                mFirst = false;
            }
            else
            {
                llvm::outs() << ",";
            }
            
            if( mFirstNode )
            {
                mFirstNode = false;
            }
            else
            {
                NewLine();
            }
        }
        
        void NewLine()
        {
            llvm::outs() << "\n";
            
            if( mIndent > 0 )
            {
                memset( mBuffer, ' ', mIndent * 4 );
                mBuffer[ mIndent * 4 ] = '\0';
                llvm::outs() << mBuffer;
            }
        }
        
        void BeginBlock()
        {
            if( mIndent < 63 )
            {
                mIndent++;
            }
            
            mFirst = true;
        }
        
        void EndBlock()
        {
            if( mIndent > 0 )
            {
                mIndent--;
            }
            
            mFirst = false;
        }
    
    
    protected:
    
        int     mIndent;
        bool    mFirst;
        bool    mFirstNode;
        char    mBuffer[ 256 ];
        
    public:
        
        JSON()
            : mIndent( 0 )
            , mFirst( true )
            , mFirstNode( true )
        {}
        
};

