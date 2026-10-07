// bdc 0x08af5a4c g_memLocalHeapVtbl
#include "bdc.h"

__typeof__(VtblEntry[4]) g_memLocalHeapVtbl = {
    {0}, { .fn = (void *)MemLocalHeapAlloc }, { .fn = (void *)MemLocalHeapFree },
    { .fn = (void *)MemLocalHeapContainsPtr },
};
