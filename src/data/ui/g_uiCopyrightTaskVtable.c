// bdc 0x08af13ec g_uiCopyrightTaskVtable
#include "bdc.h"

__typeof__(VtblEntry[4]) g_uiCopyrightTaskVtable = {
    {0}, { .fn = (void *)UiCopyrightTaskDtor }, { .fn = (void *)UiCopyrightTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop },
};
