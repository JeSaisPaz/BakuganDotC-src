// bdc 0x08af7048 g_ioDiscBufNodeVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_ioDiscBufNodeVtbl = { {0}, { .fn = (void *)IoDiscBufferNodeDtor } };
