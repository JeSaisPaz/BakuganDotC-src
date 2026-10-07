// bdc 0x08af53a4 g_coreNodeVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_coreNodeVtbl = { {0}, { .fn = (void *)CoreNodeDtor } };
