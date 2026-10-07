// bdc 0x08829fcc GfxPuffKindFlash
#include "bdc.h"

/* Kind 6 kind handler of the sprite puff (`GfxPuffCtor`) (member-pointer table `0x08ab9f10` indexed
   by the kind byte `+0x16c`, run by `GfxPuffUpdate`); step byte `+0x16e`, released from its layer
   with `UiSpriteLayerRelease` when faded out: additive `"new_flash"` flash with random rotation
   that expands quickly (growth 2.0, halved every frame) and fades by 0.28 per frame.
   The rotation is (rand[1,2) - 1) * 2pi - pi, i.e. uniform in [-pi, pi). */

void GfxPuffKindFlash(GfxPuff *puff)
{
    float size;
    float alpha;
    float rot;

    if (puff->step == 0) {
        puff->base.width = 1.0f;
        puff->base.height = 1.0f;
        puff->base.depth = 1.0f;
        puff->base.maybe_sizeW = 0.0f;
        puff->base.blendMode = 2;
        puff->base.texture = GfxFindTexture("new_flash");
        puff->base.billboardMode = 0;
        puff->step = puff->step + 1;
        puff->param170 = 2.0f;
        rot = (PlatformRandFloat12() - 1.0f) * 6.28318548f - 3.14159274f;
        puff->base.maybe_billboardParams80[2] = rot;
    } else {
        size = puff->base.width + puff->param170;
        puff->base.maybe_sizeW = 0.0f;
        puff->base.width = size;
        puff->base.height = size;
        alpha = puff->base.alpha - 0.28f;
        puff->base.depth = size;
        puff->param170 = puff->param170 * 0.5f;
        puff->base.alpha = alpha;
        if (alpha < 0.0f) {
            UiSpriteLayerRelease(puff->layer, puff);
        }
    }
}
