// bdc 0x08af1754 g_actorCrystalSpawnTaskVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_actorCrystalSpawnTaskVtable = {
    {0}, { .fn = (void *)ActorCrystalSpawnTaskDtor }, { .fn = (void *)ActorCrystalSpawnTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
