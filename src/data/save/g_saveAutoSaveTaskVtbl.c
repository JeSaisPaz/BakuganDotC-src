// bdc 0x08af161c g_saveAutoSaveTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_saveAutoSaveTaskVtbl = {
    {0}, { .fn = (void *)SaveAutoSaveTaskDtor }, { .fn = (void *)SaveAutoSaveTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)SaveAutoSaveTaskDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
