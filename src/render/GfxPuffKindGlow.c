// bdc 0x08829ef8 GfxPuffKindGlow
#include "bdc.h"

/* Kind 5 kind handler of the sprite puff (`GfxPuffCtor`) (member-pointer table `0x08ab9f10` indexed
   by the kind byte `+0x16c`, run by `GfxPuffUpdate`); step byte `+0x16e`, released from its layer
   with `UiSpriteLayerRelease` when faded out: additive glow that starts at size 0.3 and expands
   by a growth rate starting at 2.0 (×0.83 per frame) while fading by 0.2 per frame. */

void GfxPuffKindGlow(GfxPuff *puff)
{
    float size;
    float alpha;

    if (puff->step != 0) {
        size = puff->base.width + puff->param170;
        alpha = puff->base.alpha - puff->param174;
        puff->base.width = size;
        puff->base.depth = size;
        puff->base.height = size;
        puff->param170 = puff->param170 * 0.83f;
        puff->base.alpha = alpha;
        if (alpha < 0.0f) {
            UiSpriteLayerRelease(puff->layer, puff);
        }
        return;
    }
    puff->base.width = 0.3f;
    puff->base.height = 0.3f;
    puff->base.depth = 0.3f;
    puff->base.maybe_sizeW = 0.0f;
    puff->base.blendMode = 2;
    puff->base.billboardMode = 2;
    puff->base.textureSlot = 1;
    puff->step = puff->step + 1;
    puff->param170 = 2.0f;
    puff->param174 = 0.2f;
}
