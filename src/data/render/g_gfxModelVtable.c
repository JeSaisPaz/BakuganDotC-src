// bdc 0x08af5484 g_gfxModelVtable
#include "bdc.h"

__typeof__(VtblEntry[10]) g_gfxModelVtable = {
    {0}, { .fn = (void *)GfxModelDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)GfxModelUpdateAndApplyMotion }, { .fn = (void *)GfxModelDlWriteState },
    { .fn = (void *)GfxModelSetToon },
};
