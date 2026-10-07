// bdc 0x08af1188 g_cxxBadAllocDtorCleanup
#include "bdc.h"

__typeof__(void *) g_cxxBadAllocDtorCleanup = (void *)CxxExceptionDtor;
