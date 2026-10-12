// bdc 0x08af13ec g_uiCopyrightTaskVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiCopyrightTaskVtable = {
    {0}, { .fn = (void *)UiCopyrightTaskDtor }, { .fn = (void *)UiCopyrightTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiCopyrightTaskDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
