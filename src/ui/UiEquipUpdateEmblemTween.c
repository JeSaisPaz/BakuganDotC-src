// bdc 0x08962744 UiEquipUpdateEmblemTween
#include "bdc.h"

/* Advances the pop of the emblem sprite (sprite index `spriteIdx[11]`, `+0x5176`) of the
   Bakugan/gear loadout screen before a battle (task 302, `UiEquipCtor`) by 0.125 of its tween
   phase `t`. `out` = 0: scale = startScale - (1 - (t-1)^2) * 2.2 on both axes, snapped to 0.8 once
   `t` reaches 1. `out` = 1: alpha = startAlpha - t^2 * 0.6 and scale = startScale + t^2 * 0.7, and
   the sprite's flag bit 0 is cleared once `t` reaches 1. Either way the sprite matrix is rebuilt
   (GfxSpriteSetScaleRotation). Returns 1 (and sets the sprite's layerMask to 8) when `t` is no
   longer below 1, else 0. */

static inline GfxSprite *UiEquipEmblemSprite(UiEquip *self)
{
    return ((GfxSprite **)self->base.data)[self->spriteIdx[11]];
}

s32 UiEquipUpdateEmblemTween(UiEquip *self, bool out)
{
    UiTween *tw = &self->tweens[self->spriteIdx[11]];
    float t = tw->t + 0.125f;
    float d;
    s32 done = 0;
    GfxSprite *s;

    if (!out) {
        tw->t = t;
        d = t - 1.0f;
        UiEquipEmblemSprite(self)->scaleX = tw->startScale - (1.0f - d * d) * 2.2f;
        s = UiEquipEmblemSprite(self);
        s->scaleY = s->scaleX;
        if (!(self->tweens[self->spriteIdx[11]].t < 1.0f)) {
            UiEquipEmblemSprite(self)->scaleX = 0.8f;
            UiEquipEmblemSprite(self)->scaleY = 0.8f;
            done = 1;
        }
        s = UiEquipEmblemSprite(self);
        GfxSpriteSetScaleRotation(s, s->scaleX, s->scaleY, s->angle, false);
    } else {
        tw->t = t;
        UiEquipEmblemSprite(self)->alpha = tw->startAlpha - t * t * 0.6f;
        tw = &self->tweens[self->spriteIdx[11]];
        UiEquipEmblemSprite(self)->scaleX = tw->startScale + tw->t * tw->t * 0.7f;
        s = UiEquipEmblemSprite(self);
        s->scaleY = s->scaleX;
        if (!(self->tweens[self->spriteIdx[11]].t < 1.0f)) {
            s = UiEquipEmblemSprite(self);
            s->flags &= ~1u;
            done = 1;
        }
        s = UiEquipEmblemSprite(self);
        GfxSpriteSetScaleRotation(s, s->scaleX, s->scaleY, s->angle, false);
    }

    if (done != 1) {
        return 0;
    }
    UiEquipEmblemSprite(self)->layerMask = 8;
    return 1;
}
