// bdc 0x088267c4 GfxMeshObjCreateSmokeColumn
#include "bdc.h"

/* Creates a stencil-masked smoke column in mesh list 3 (`GfxMeshObjCreateList3`): two state-5
   mask meshes (`GfxMeshObjStateStencilMask`) – the first writes stencil value `ref` (STST
   always, SOP replace), the second (basis columns 0..2 scaled by (-1, -6, -1): flipped, stretched
   ×6) tests equal `ref` and increments – and a state-7 smoke quad
   (`GfxMeshObjStateStencilSmoke`) drawn where the stencil equals `ref + 1`, which owns the two
   masks (`child0`, `child1`). Returns the smoke object. Called by `BtlShadowInit`. */

void *GfxMeshObjCreateSmokeColumn(s32 ref)
{
    GfxMeshObj *mask0;
    GfxMeshObj *mask1;
    GfxMeshObj *smoke;
    float *m;
    s32 i;

    mask0 = GfxMeshObjCreateList3(5, NULL);
    GfxMeshObjStateStencilMask(mask0);
    mask0->stencilOp = 0xdd020000;
    mask0->stencilTest = (u32)ref << 8 | 0xdcff0001;
    mask1 = GfxMeshObjCreateList3(5, NULL);
    GfxMeshObjStateStencilMask(mask1);
    mask1->stencilOp = 0xdd000400;
    mask1->stencilTest = (u32)ref << 8 | 0xdcff0002;
    m = mask1->basis;
    for (i = 0; i < 4; i++) {
        m[i] = m[i] * -1.0f;
        m[4 + i] = m[4 + i] * -6.0f;
        m[8 + i] = m[8 + i] * -1.0f;
    }
    smoke = GfxMeshObjCreateList3(7, NULL);
    GfxMeshObjStateStencilSmoke(smoke);
    smoke->stencilOp = 0xdd000000;
    smoke->stencilTest = (u32)(ref + 1) << 8 | 0xdcff0002;
    smoke->child0 = mask0;
    smoke->u158.child1 = mask1;
    return smoke;
}
