// bdc 0x08af1210 g_cxxTerminateHandler
#include "bdc.h"

__typeof__(void *) g_cxxTerminateHandler = (void *)CxxDefaultTerminate;
