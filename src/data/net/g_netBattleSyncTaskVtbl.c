// bdc 0x08af1694 g_netBattleSyncTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_netBattleSyncTaskVtbl = {
    {0}, { .fn = (void *)NetBattleSyncTaskDtor }, { .fn = (void *)NetBattleSyncTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
