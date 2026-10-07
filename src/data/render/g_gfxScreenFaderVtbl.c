// bdc 0x08af577c g_gfxScreenFaderVtbl
#include "bdc.h"

__typeof__(VtblEntry[6]) g_gfxScreenFaderVtbl = {
    {0}, { .fn = (void *)GfxScreenFaderDtor }, { .fn = (void *)GfxScreenFaderSetVisible },
    { .fn = (void *)GfxFaderBaseOnStart }, { .fn = (void *)GfxScreenFaderApply },
    { .fn = (void *)GfxScreenFaderDraw },
};
