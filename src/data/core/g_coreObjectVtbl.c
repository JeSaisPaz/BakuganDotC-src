// bdc 0x08af5394 g_coreObjectVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_coreObjectVtbl = { {0}, { .fn = (void *)CoreObjectDtor } };
