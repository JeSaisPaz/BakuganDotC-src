// bdc 0x0882a0c4 GfxPuffKindShockwave
#include "bdc.h"

/* Kind 7 kind handler of the sprite puff (`GfxPuffCtor`) (member-pointer table `0x08ab9f10` indexed
   by the kind byte `+0x16c`, run by `GfxPuffUpdate`); step byte `+0x16e`, released from its layer
   with `UiSpriteLayerRelease` when faded out: `"shockwave"` ring (flat, tint 0.8/0.8/0.7, alpha
   0.8) that starts at size 5 and expands by 3.0 decreasing by 0.25 per frame (minimum 0.8); after 8
   frames it fades by 0.045 per frame. */

void GfxPuffKindShockwave(GfxPuff *puff)
{
    unsigned char step;
    float size;
    float growth;
    float alpha;

    step = puff->step;
    if (step == 0) {
        puff->base.width = 5.0f;
        puff->base.height = 5.0f;
        puff->base.depth = 5.0f;
        puff->base.maybe_sizeW = 0.0f;
        puff->base.blendMode = 1;
        puff->base.texture = GfxFindTexture("shockwave");
        puff->base.billboardMode = 2;
        puff->base.tint[0] = 0.8f;
        puff->base.tint[1] = 0.8f;
        puff->base.tint[2] = 0.7f;
        puff->base.alpha = 0.8f;
        puff->param170 = 3.0f;
        puff->step = puff->step + 1;
        return;
    }

    size = puff->base.width + puff->param170;
    growth = puff->param170 - 0.25f;
    puff->base.width = size;
    puff->base.depth = size;
    puff->base.height = size;
    puff->param170 = growth;
    if (growth < 0.8f) {
        puff->param170 = 0.8f;
    }
    if (step >= 9) {
        alpha = puff->base.alpha - 0.045f;
        puff->base.alpha = alpha;
        if (alpha < 0.0f) {
            UiSpriteLayerRelease(puff->layer, puff);
            return;
        }
    }
    puff->step = step + 1;
}
