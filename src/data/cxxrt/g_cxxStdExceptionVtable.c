// bdc 0x08af5a80 g_cxxStdExceptionVtable
#include "bdc.h"

__typeof__(void *[6]) g_cxxStdExceptionVtable = {
    NULL, (void *)&g_cxxStdExceptionTypeInfo, NULL, (void *)CxxExceptionDtor, NULL,
    (void *)CxxExceptionWhat,
};
