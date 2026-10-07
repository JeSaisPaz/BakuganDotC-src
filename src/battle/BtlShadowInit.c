// bdc 0x08885204 BtlShadowInit
#include "bdc.h"

/* Initialises a ground-shadow object for its mode: zeroes the size vector (bank constant
   C720 = 0), sets `enabled`, clears both visuals, then creates one. Mode 0: a
   smoke-column mesh object, variant ((owner stencilRef & 3) * 2 + 10). Modes 1, 2 and 4: a black
   "kemuri1" billboard on `g_worldSpriteLayer`, alpha 0.5, billboard mode 2, hidden (flags bit 0
   cleared). Other modes create nothing. */

void BtlShadowInit(BtlShadow *shadow)
{
    GfxSprite *billboard;
    s32 mode;

    shadow->size[0] = 0.0f;
    shadow->size[1] = 0.0f;
    shadow->size[2] = 0.0f;
    shadow->size[3] = 0.0f;
    shadow->enabled = 1;
    shadow->billboard = NULL;
    shadow->mesh = NULL;
    mode = shadow->mode;
    if (mode == 0) {
        /* Mode 0 is only set by BtlShadowCtorForUnit, whose owner is a BtlBakugan. */
        BtlBakugan *unit = (BtlBakugan *)shadow->owner;
        shadow->mesh = GfxMeshObjCreateSmokeColumn((s32)(((u32)unit->stencilRef & 3) * 2 + 10));
    } else if (mode == 1 || mode == 2 || mode == 4) {
        billboard = GfxSpriteLayerCreateBillboardByName(g_worldSpriteLayer, "kemuri1");
        shadow->billboard = billboard;
        billboard->tint[0] = 0.0f;
        billboard->tint[1] = 0.0f;
        billboard->tint[2] = 0.0f;
        billboard->alpha = 0.5f;
        shadow->billboard->billboardMode = 2;
        shadow->billboard->flags = shadow->billboard->flags & ~1u;
    }
}
