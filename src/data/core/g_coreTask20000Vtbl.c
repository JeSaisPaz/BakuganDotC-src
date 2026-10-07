// bdc 0x08af5d50 g_coreTask20000Vtbl
#include "bdc.h"

__typeof__(VtblEntry[6]) g_coreTask20000Vtbl = {
    {0}, { .fn = (void *)CoreTask20000Dtor }, { .fn = (void *)CoreTaskBaseUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField },
};
