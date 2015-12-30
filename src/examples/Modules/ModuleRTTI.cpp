////////////////////////////////////////////////////////////////////////////////
#if defined( _WIN32 )

#include <windows.h>

BOOL APIENTRY DllMain( HMODULE, DWORD, LPVOID )
{
    return TRUE;
}

#endif

////////////////////////////////////////////////////////////////////////////////

#include <string>
#include "gwRTTI/gwRTTI.h"

gwRTTI_REGISTER( unsigned int );
gwRTTI_REGISTER( float );
gwRTTI_REGISTER( std::string );

// %%
enum AnEnum
{
    Red,Green,Blue
};

gwRTTI_REGISTER( AnEnum );
