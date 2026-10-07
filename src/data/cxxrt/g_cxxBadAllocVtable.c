// bdc 0x08af7068 g_cxxBadAllocVtable
#include "bdc.h"

__typeof__(void *[6]) g_cxxBadAllocVtable = {
    NULL, (void *)&g_cxxBadAllocTypeInfo, NULL, (void *)CxxBadAllocDtor, NULL,
    (void *)CxxBadAllocWhat,
};
