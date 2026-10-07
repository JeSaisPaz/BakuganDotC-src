// bdc 0x08af1494 g_saveAutoLoadTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_saveAutoLoadTaskVtbl = {
    {0}, { .fn = (void *)SaveAutoLoadTaskDtor }, { .fn = (void *)SaveAutoLoadTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
