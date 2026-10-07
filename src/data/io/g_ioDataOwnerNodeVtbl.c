// bdc 0x08af7058 g_ioDataOwnerNodeVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_ioDataOwnerNodeVtbl = { {0}, { .fn = (void *)IoDataOwnerNodeDtor } };
