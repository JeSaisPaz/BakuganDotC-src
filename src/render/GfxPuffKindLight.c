// bdc 0x08829980 GfxPuffKindLight
#include "bdc.h"

/* Kind 4 kind handler of the sprite puff (`GfxPuffCtor`) (member-pointer table `0x08ab9f10` indexed
   by the kind byte `+0x16c`, run by `GfxPuffUpdate`); step byte `+0x16e`, released from its layer
   with `UiSpriteLayerRelease` when faded out: additive light flare that alternates between
   `"corelight"` (random rotation, spinning by 0.01 per frame, pulsing size, alpha flickering around
   0.2) and `"eyelight"` (fixed rotation and size, alpha flickering around 0.6) on successive spawns
   (counter `g_puffLightSpawnCount`). It sits 3 units from its attach target (`unk160`) toward
   (target facing the camera) or away from the `camera`, is scaled by the
   target's w, and fades out by 0.3 per frame once the target is gone (0.05 per frame at any step
   past 1).
   The VFPU bank constants it read (2π, π, 2/π, 0, 1) are literals; the random values come from
   PlatformRandFloat12 (vrndf1). */

#define PUFF_PI 3.14159274f
#define PUFF_TWO_PI 6.28318548f

void GfxPuffKindLight(GfxPuff *puff)
{
    float toCam[3];
    float inv;
    float dot;
    float rot;
    float facing;
    float scale;
    float size;
    float alpha;

    if (puff->step == 0) {
        puff->base.width = 12.0f;
        puff->base.height = 12.0f;
        puff->base.depth = 12.0f;
        puff->base.maybe_sizeW = 0.0f;
        puff->base.blendMode = 2;
        puff->param170 = 0.0f;
        puff->pulsePhase = 0.0f;
        if (g_puffLightSpawnCount & 1) {
            puff->base.texture = GfxFindTexture("corelight");
            puff->param174 = 0.01f;
            puff->maybe_variant16d = 4;
            rot = (PlatformRandFloat12() - 1.0f) * PUFF_TWO_PI - PUFF_PI;
            puff->base.maybe_billboardParams80[2] = rot;
        } else {
            puff->base.texture = GfxFindTexture("eyelight");
            puff->param174 = -0.02f;
            puff->maybe_variant16d = 0;
            puff->base.maybe_billboardParams80[2] = 0.0f;
        }
        g_puffLightSpawnCount = g_puffLightSpawnCount + 1;
        puff->step2 = 0;
        puff->step = puff->step + 1;
        return;
    }

    if (puff->unk160 == NULL) {
        /* attach target gone: fade out */
        alpha = puff->base.alpha - 0.3f;
        puff->base.alpha = alpha;
        if (alpha < 0.0f) {
            UiSpriteLayerRelease(puff->layer, puff);
        }
        return;
    }

    /* toCam = camera eye - target position */
    toCam[0] = g_gfxActiveCamera->eye[0] - puff->unk160[0];
    toCam[1] = g_gfxActiveCamera->eye[1] - puff->unk160[1];
    toCam[2] = g_gfxActiveCamera->eye[2] - puff->unk160[2];
    /* facing = target direction . camera view direction */
    facing = puff->unk160[8] * g_gfxActiveCamera->dir[0] + puff->unk160[9] * g_gfxActiveCamera->dir[1]
           + puff->unk160[10] * g_gfxActiveCamera->dir[2];
    if (facing < -0.1f) {
        scale = 3.0f;
    } else {
        scale = -3.0f;
    }
    /* toCam = normalize(toCam) * scale (a zero vector stays zero) */
    dot = toCam[0] * toCam[0] + toCam[1] * toCam[1] + toCam[2] * toCam[2];
    if (dot == 0.0f) {
        inv = 0.0f;
    } else {
        inv = VfRsq(dot);
    }
    inv = inv * scale;
    toCam[0] = toCam[0] * inv;
    toCam[1] = toCam[1] * inv;
    toCam[2] = toCam[2] * inv;
    /* posXYZ = target position + toCam; posW = target w */
    puff->base.posX = puff->unk160[0] + toCam[0];
    puff->base.posY = puff->unk160[1] + toCam[1];
    puff->base.posZ = puff->unk160[2] + toCam[2];
    puff->base.posW = puff->unk160[3];

    if (!(puff->param174 <= 0.0f)) {
        /* corelight: spin and pulse the size */
        rot = puff->base.maybe_billboardParams80[2] + puff->param174;
        puff->base.maybe_billboardParams80[2] = rot;
        if (!(rot <= PUFF_PI)) {
            puff->base.maybe_billboardParams80[2] = puff->base.maybe_billboardParams80[2] - PUFF_TWO_PI;
        } else if (puff->base.maybe_billboardParams80[2] <= -PUFF_PI) {
            puff->base.maybe_billboardParams80[2] = puff->base.maybe_billboardParams80[2] + PUFF_TWO_PI;
        }
        rot = (PlatformRandFloat12() - 1.0f) * 0.6f + 0.2f + puff->pulsePhase;
        puff->pulsePhase = rot;
        if (!(rot <= PUFF_PI)) {
            puff->pulsePhase = puff->pulsePhase - PUFF_TWO_PI;
        } else if (puff->pulsePhase <= -PUFF_PI) {
            puff->pulsePhase = puff->pulsePhase + PUFF_TWO_PI;
        }
        size = puff->unk160[3] * 0.3f * (__builtin_sinf(puff->pulsePhase) * 3.0f + 16.0f);
        puff->base.width = size;
        puff->base.height = size;
        puff->base.depth = size;
        puff->base.maybe_sizeW = 0.0f;
    } else {
        /* eyelight: fixed 12x6 size scaled by the target's w */
        size = puff->unk160[3];
        puff->base.maybe_sizeW = 0.0f;
        puff->base.width = size * 12.0f;
        puff->base.depth = 1.0f;
        puff->base.height = size * 6.0f;
    }

    puff->step2 = puff->step2 + 1;
    if (puff->step == 1) {
        /* alpha flicker */
        rot = (PlatformRandFloat12() - 1.0f) * 2.0f + 0.5f + puff->param170;
        puff->param170 = rot;
        if (!(rot <= PUFF_PI)) {
            puff->param170 = puff->param170 - PUFF_TWO_PI;
        } else if (puff->param170 <= -PUFF_PI) {
            puff->param170 = puff->param170 + PUFF_TWO_PI;
        }
        if (!(puff->param174 <= 0.0f)) {
            alpha = __builtin_sinf(puff->param170) * 0.4f + 0.2f;
        } else {
            alpha = __builtin_sinf(puff->param170) * 0.4f + 0.6f;
        }
        puff->base.alpha = alpha;
    } else {
        alpha = puff->base.alpha - 0.05f;
        puff->base.alpha = alpha;
        if (alpha < 0.0f) {
            UiSpriteLayerRelease(puff->layer, puff);
        }
    }
}
