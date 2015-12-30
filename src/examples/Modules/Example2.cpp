////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "Example2.h"
#include "gwRTTI/gwRTTI.h"
#include "ModuleA.h"

#if defined( _WIN32 )
    #pragma comment( lib, "ModuleRTTI.lib" )
    #pragma comment( lib, "ModuleA.lib" )
#endif

using namespace gw::RTTI;


////////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
    printf( "-- objects at start up\n\n" );
    gw::RTTI::Registry::Dump();
    printf( "\n\n" );

    // dynamically load ModuleB

    auto hModule = LoadModule();

    if( hModule == nullptr )
    {
        printf( "Failed to load ModuleB\n" );
        return 1;
    }

    auto ModuleB = (void(*)()) FindSymbol( hModule, "ModuleB" );

    if( ModuleB == nullptr )
    {
        printf( "Failed to find ModuleB\n" );
        return 1;
    }

    printf( "-- objects after loading ModuleB (should contain ObjectB)\n\n" );
    gw::RTTI::Registry::Dump();
    printf( "\n\n" );

    ModuleA();
    ModuleB();

    const TypeInfo* type;

    type = Type< float >();
    type = Type< VectorA >();
    type = Type< class ObjectB >(); // dynamically loaded - no linking or header files
    type = Type< std::string >();

    UnloadModule( hModule );

    return 0;
}

