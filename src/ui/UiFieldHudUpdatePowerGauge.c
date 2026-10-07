// bdc 0x088d1e88 UiFieldHudUpdatePowerGauge
#include "bdc.h"

/* Updates one power gauge of the field HUD (sprite array base.data, player `player`): power 0 =
   scan (gaugeB, flag `scan`, sprites empty 0x44 / bar 0x12 / icon 4), 1 = stealth (gaugeA, flag from
   ActorPlayerIsStealthed, sprites 0x15/0x11/5), any other value uses gauge 0 and sprite 0. Bar
   height h = gauge * 38. h == 0: shows the empty and icon sprites, hides the bar and resets its tint
   and alpha to 1. Otherwise: bar tint[1] = 1, addColor[0] = 0, icon shown; at gauge <= 1/3 the bar
   turns red and, while the power is active, *blink advances modulo 12 and the icon is hidden while
   it is above 6. The bar then keeps its width, gets posY 255, height h, UV rect {0, 39 - h, w, h},
   and is shown while the empty sprite is hidden. */

void UiFieldHudUpdatePowerGauge(UiFieldHud *self, s32 power, s32 *blink)
{
    float gauge = 0.0f;
    float h = 0.0f;
    float width;
    s32 emptyIdx = 0;
    s32 barIdx = 0;
    s32 iconIdx = 0;
    u32 active = 0;
    GfxSprite *sprite;
    float rect[4];

    if (power == 0) {
        gauge = self->player->gaugeB;
        emptyIdx = 0x44;
        barIdx = 0x12;
        h = gauge * 38.0f;
        iconIdx = 4;
        active = self->player->scan;
    }
    else if (power == 1) {
        gauge = self->player->gaugeA;
        emptyIdx = 0x15;
        barIdx = 0x11;
        h = gauge * 38.0f;
        iconIdx = 5;
        active = ActorPlayerIsStealthed(self->player);
    }

    if (h == 0.0f) {
        sprite = ((GfxSprite **)self->base.data)[emptyIdx];
        sprite->flags |= 1;
        sprite = ((GfxSprite **)self->base.data)[iconIdx];
        sprite->flags |= 1;
        sprite = ((GfxSprite **)self->base.data)[barIdx];
        sprite->flags &= ~1u;
        sprite = ((GfxSprite **)self->base.data)[barIdx];
        sprite->tint[0] = 1.0f;
        sprite->tint[1] = 1.0f;
        sprite->tint[2] = 1.0f;
        sprite->alpha = 1.0f;
        return;
    }

    ((GfxSprite **)self->base.data)[barIdx]->tint[1] = 1.0f;
    ((GfxSprite **)self->base.data)[barIdx]->addColor[0] = 0.0f;
    sprite = ((GfxSprite **)self->base.data)[iconIdx];
    sprite->flags |= 1;
    if (gauge <= 0.33333334f) {
        sprite = ((GfxSprite **)self->base.data)[barIdx];
        sprite->tint[0] = 1.0f;
        sprite->tint[1] = 0.0f;
        sprite->tint[2] = 0.0f;
        sprite->alpha = 1.0f;
        sprite = ((GfxSprite **)self->base.data)[barIdx];
        sprite->addColor[0] = 1.0f;
        sprite->addColor[1] = 0.0f;
        sprite->addColor[2] = 0.0f;
        sprite->addColor[3] = 1.0f;
        if (active != 0) {
            *blink = (*blink + 1) % 12;
            if (!((float)*blink <= 6.0f)) {
                sprite = ((GfxSprite **)self->base.data)[iconIdx];
                sprite->flags &= ~1u;
            }
        }
    }

    width = GfxSpriteGetWidth(((GfxSprite **)self->base.data)[barIdx]);
    ((GfxSprite **)self->base.data)[barIdx]->posY = 255.0f;
    UiSpriteSetSize(width, h, ((GfxSprite **)self->base.data)[barIdx]);
    rect[0] = 0.0f;
    rect[1] = 39.0f - h;
    rect[2] = width;
    rect[3] = h;
    GfxSpriteSetUvRectXYWH(((GfxSprite **)self->base.data)[barIdx], rect);
    sprite = ((GfxSprite **)self->base.data)[barIdx];
    sprite->flags |= 1;
    sprite = ((GfxSprite **)self->base.data)[emptyIdx];
    sprite->flags &= ~1u;
}
