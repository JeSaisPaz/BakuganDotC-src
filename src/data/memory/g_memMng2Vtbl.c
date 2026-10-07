// bdc 0x08af5374 g_memMng2Vtbl
#include "bdc.h"

__typeof__(VtblEntry[4]) g_memMng2Vtbl = { {0}, { .fn = (void *)Mem2Alloc }, { .fn = (void *)Mem2Free }, { .fn = (void *)Mem2ContainsPtr } };
