// bdc 0x08af15e4 g_saveSaveTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_saveSaveTaskVtbl = {
    {0}, { .fn = (void *)SaveSaveTaskDtor }, { .fn = (void *)SaveSaveTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
