// bdc 0x08af153c g_saveNopTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_saveNopTaskVtbl = {
    {0}, { .fn = (void *)SaveNopTaskDtor }, { .fn = (void *)SaveNopTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
