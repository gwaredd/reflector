////////////////////////////////////////////////////////////////////////////////

#include "Example2.h"

#ifdef MODULEA_EXPORTS
    #define MODULEA_API MODULE_EXPORT
#else
    #define MODULEA_API MODULE_IMPORT
#endif


////////////////////////////////////////////////////////////////////////////////
// %% -- module A defines VectorA

struct MODULEA_API VectorA
{
    float x, y, z;
};

extern "C"
{
    MODULEA_API void* ModuleA();
}
