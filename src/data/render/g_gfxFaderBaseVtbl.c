// bdc 0x08af574c g_gfxFaderBaseVtbl
#include "bdc.h"

__typeof__(VtblEntry[6]) g_gfxFaderBaseVtbl = {
    {0}, { .fn = (void *)GfxFaderBaseDtor }, { .fn = (void *)GfxFaderBaseSetVisible },
    { .fn = (void *)GfxFaderBaseOnStart }, { .fn = (void *)GfxFaderBaseApply },
    { .fn = (void *)GfxFaderBaseDraw },
};
