#pragma once

class ReflectOutput
{
    public:

        void WriteDecl( TagDecl* D );
        void SetContext( ASTContext* context ) { mContext = context; }


    protected:

        void WriteClass( CXXRecordDecl* record );
        void WriteEnum( EnumDecl* enumType );

        JSON&               mJSON;
        CompilerInstance&   mCI;
        ASTContext*         mContext;
        ReflectContext      mReflect;


    public:

        ReflectOutput( JSON& json, CompilerInstance& ci )
            : mJSON( json )
            , mCI( ci )
            , mContext( nullptr )
        {
        }

};

