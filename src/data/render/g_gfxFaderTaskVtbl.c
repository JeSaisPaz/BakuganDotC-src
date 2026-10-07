// bdc 0x08af57e4 g_gfxFaderTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gfxFaderTaskVtbl = {
    {0}, { .fn = (void *)GfxFaderTaskDtor }, { .fn = (void *)GfxFaderTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)GfxFaderTaskDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
