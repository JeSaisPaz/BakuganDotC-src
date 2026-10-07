// bdc 0x08829830 GfxPuffKindRisingSmoke
#include "bdc.h"

/* Kind 3 kind handler of the sprite puff (`GfxPuffCtor`) (member-pointer table `0x08ab9f10` indexed
   by the kind byte `+0x16c`, run by `GfxPuffUpdate`); step byte `+0x16e`, released from its layer
   with `UiSpriteLayerRelease` when faded out: orange-tinted (0.8, 0.6, 0.4) `"kemuri1"` smoke
   (`g_puffSmokeTexture`) that fades in from 0.2 to 0.5 alpha (+0.1 per frame) then out by 0.02
   per frame, growing by the global vector `g_puffStaticVec` and rising 0.1 per frame. */

void GfxPuffKindRisingSmoke(GfxPuff *puff)
{
    unsigned char step;
    float alpha;

    step = puff->step;
    if (step == 0) {
        puff->base.texture = g_puffSmokeTexture;
        puff->base.tint[0] = 0.8f;
        puff->base.tint[1] = 0.6f;
        puff->base.tint[2] = 0.4f;
        puff->base.alpha = 0.2f;
        puff->step = puff->step + 1;
        return;
    }
    if (step < 2) {
        /* fade in */
        alpha = puff->base.alpha + 0.1f;
        puff->base.alpha = alpha;
        if (!(alpha < 0.5f)) {
            puff->step = puff->step + 1;
        }
    } else if (step < 3) {
        /* fade out */
        alpha = puff->base.alpha - 0.02f;
        puff->base.alpha = alpha;
        if (alpha <= 0.0f) {
            UiSpriteLayerRelease(puff->layer, puff);
            return;
        }
    }
    /* xyz of width/height/depth += g_puffStaticVec; w (maybe_sizeW) kept */
    puff->base.width = puff->base.width + g_puffStaticVec[0];
    puff->base.height = puff->base.height + g_puffStaticVec[1];
    puff->base.depth = puff->base.depth + g_puffStaticVec[2];
    puff->base.posY = puff->base.posY + 0.1f;
}
