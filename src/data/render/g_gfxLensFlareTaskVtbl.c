// bdc 0x08af23fc g_gfxLensFlareTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gfxLensFlareTaskVtbl = {
    {0}, { .fn = (void *)GfxLensFlareTaskDtor }, { .fn = (void *)GfxLensFlareTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
