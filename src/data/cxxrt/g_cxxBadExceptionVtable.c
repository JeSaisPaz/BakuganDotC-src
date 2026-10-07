// bdc 0x08af5a98 g_cxxBadExceptionVtable
#include "bdc.h"

__typeof__(void *[6]) g_cxxBadExceptionVtable = {
    NULL, (void *)&g_cxxBadExceptionTypeInfo, NULL, (void *)CxxBadExceptionDtor, NULL,
    (void *)CxxBadExceptionWhat,
};
