// bdc 0x08af4c9c g_netStatusTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_netStatusTaskVtbl = {
    {0}, { .fn = (void *)NetStatusTaskDtor }, { .fn = (void *)NetStatusTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)NetStatusTaskDraw },
    { .fn = (void *)NetStatusTaskSetField }, { .fn = (void *)NetStatusTaskGetField },
};
