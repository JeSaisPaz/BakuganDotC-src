// bdc 0x08af5224 g_coreTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[6]) g_coreTaskVtbl = {
    {0}, { .fn = (void *)CoreTaskDestroy }, { .fn = (void *)CoreTaskBaseUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField },
};
