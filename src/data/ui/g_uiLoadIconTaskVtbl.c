// bdc 0x08af1424 g_uiLoadIconTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiLoadIconTaskVtbl = {
    {0}, { .fn = (void *)UiLoadIconTaskDtor }, { .fn = (void *)UiLoadIconTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
