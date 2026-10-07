// bdc 0x08af58fc g_ioDecodeJobVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_ioDecodeJobVtbl = { {0}, { .fn = (void *)IoDecodeDtor } };
