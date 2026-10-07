// bdc 0x08af1574 g_saveLoadTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_saveLoadTaskVtbl = {
    {0}, { .fn = (void *)SaveLoadTaskDtor }, { .fn = (void *)SaveLoadTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
