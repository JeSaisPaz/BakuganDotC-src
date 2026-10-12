// bdc 0x08af14cc g_saveDeleteTaskVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_saveDeleteTaskVtable = {
    {0}, { .fn = (void *)SaveDeleteTaskDtor }, { .fn = (void *)SaveDeleteTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
