// bdc 0x08ab9ebc g_gfxMeshObjStateTable
#include "bdc.h"

__typeof__(VtblEntry[8]) g_gfxMeshObjStateTable = {
    { .fn = (void *)GfxMeshObjStateNop }, { .fn = (void *)GfxMeshObjStateHeatPuff },
    { .fn = (void *)GfxMeshObjStateEffectTrail }, { .fn = (void *)GfxMeshObjStateDistortionLarge },
    { .fn = (void *)GfxMeshObjStateSmokeBezier }, { .fn = (void *)GfxMeshObjStateStencilMask },
    { .fn = (void *)GfxMeshObjStateDistortion }, { .fn = (void *)GfxMeshObjStateStencilSmoke },
};
