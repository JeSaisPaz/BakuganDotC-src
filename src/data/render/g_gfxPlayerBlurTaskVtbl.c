// bdc 0x08af171c g_gfxPlayerBlurTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gfxPlayerBlurTaskVtbl = {
    {0}, { .fn = (void *)GfxPlayerBlurTaskDtor }, { .fn = (void *)GfxPlayerBlurTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)GfxPlayerBlurTaskDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
