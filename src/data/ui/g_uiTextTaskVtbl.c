// bdc 0x08af56e4 g_uiTextTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiTextTaskVtbl = {
    {0}, { .fn = (void *)UiTextTaskDtor }, { .fn = (void *)UiTextTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiTextTaskDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)UiTextTaskGetField },
};
