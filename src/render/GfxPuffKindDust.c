// bdc 0x088294bc GfxPuffKindDust
#include "bdc.h"

/* Kind 1 kind handler of the sprite puff (`GfxPuffCtor`) (member-pointer table `0x08ab9f10` indexed
   by the kind byte `+0x16c`, run by `GfxPuffUpdate`); step byte `+0x16e`, released from its layer
   with `UiSpriteLayerRelease` when faded out: `"kemuri1"` dust puff with a random light-grey tint
   (0.7–0.9): fades in by 0.1 to 0.7 alpha while its velocity (`scaleX..Z`) is damped (×0.83), then
   fades out by 0.03 (×0.87 damping). Every non-init frame it grows by `g_puffStaticVec`, adds
   `matrix[0..2]` (acceleration) to its velocity and moves by it. The damping clears `angle` (the
   bank zero S713 stored as the fourth lane). */

void GfxPuffKindDust(GfxPuff *puff)
{
    unsigned char step;
    float rnd;
    float grey;
    float alpha;

    step = puff->step;
    if (step == 0) {
        puff->base.texture = g_puffSmokeTexture;
        rnd = PlatformRandFloat12() - 1.0f;
        grey = rnd * 0.2f + 0.7f;
        puff->base.tint[0] = grey;
        puff->base.tint[1] = grey;
        puff->base.tint[2] = grey * 0.9f;
        puff->base.alpha = 0.0f;
        puff->param170 = 0.7f;
        puff->step = puff->step + 1;
        return;
    }
    if (step < 2) {
        /* fade in */
        puff->base.alpha = puff->base.alpha + 0.1f;
        puff->base.scaleX = puff->base.scaleX * 0.83f;
        puff->base.scaleY = puff->base.scaleY * 0.83f;
        puff->base.scaleZ = puff->base.scaleZ * 0.83f;
        puff->base.angle = 0.0f;
        if (!(puff->base.alpha < puff->param170)) {
            puff->base.alpha = puff->param170;
            puff->step = puff->step + 1;
        }
    } else if (step < 3) {
        /* fade out */
        puff->base.scaleX = puff->base.scaleX * 0.87f;
        puff->base.scaleY = puff->base.scaleY * 0.87f;
        puff->base.scaleZ = puff->base.scaleZ * 0.87f;
        puff->base.angle = 0.0f;
        alpha = puff->base.alpha - 0.03f;
        puff->base.alpha = alpha;
        if (alpha <= 0.0f) {
            UiSpriteLayerRelease(puff->layer, puff);
            return;
        }
    }
    puff->base.width = puff->base.width + g_puffStaticVec[0];
    puff->base.height = puff->base.height + g_puffStaticVec[1];
    puff->base.depth = puff->base.depth + g_puffStaticVec[2];
    puff->base.scaleX = puff->base.scaleX + puff->base.matrix[0];
    puff->base.scaleY = puff->base.scaleY + puff->base.matrix[1];
    puff->base.scaleZ = puff->base.scaleZ + puff->base.matrix[2];
    puff->base.posX = puff->base.posX + puff->base.scaleX;
    puff->base.posY = puff->base.posY + puff->base.scaleY;
    puff->base.posZ = puff->base.posZ + puff->base.scaleZ;
}
