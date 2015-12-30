////////////////////////////////////////////////////////////////////////////////

#include "Example2.h"

#ifdef MODULEB_EXPORTS
    #define MODULEB_API MODULE_EXPORT
#else
    #define MODULEB_API MODULE_IMPORT
#endif


////////////////////////////////////////////////////////////////////////////////


// %%

class MODULEB_API ObjectB
{
    public:
        float someValue;
};

extern "C"
{
    MODULEB_API void* ModuleB();
}

