// bdc 0x08af53b4 g_coreNodeOwnerVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_coreNodeOwnerVtbl = { {0}, { .fn = (void *)CoreNodeOwnerDtor } };
