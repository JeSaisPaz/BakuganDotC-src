// bdc 0x088d1a9c UiFieldHudUpdateCounters
#include "bdc.h"

/* Updates the 7-digit point counter of the field HUD screen (task 3001, `UiFieldHudCtor`; sprite
   array `base.data`, digit sprites 0x37..0x3d, frame sprite 0x36): steps `shownPoints` by 5 toward
   the profile's points (`SaveGetProfile`). Equal: colour black, counting up: grey (0.5), both stop
   the player's penalty sound (`ActorPlayerStopPenaltySound`); counting down: red, and that colour
   goes to tint/alpha (addColor 0) instead of addColor (tint/alpha 1). Sprite 0x36 and every digit
   get the colour; each digit's cell is set with `GfxSpriteSetCell` (digit / 5, digit % 5); leading
   zeros are hidden (the ones digit always shown) and every hidden leading digit shifts all digits
   5.5 left of their layout x (`UiLayoutGetEntry(0xc, i)`). While the player scans, sprites
   0x36..0x3d are hidden. */

void UiFieldHudUpdateCounters(UiFieldHud *self)
{
    float black[4];
    float grey[4];
    float red[4];
    float color[4];
    s32 target;
    u8 decreasing;
    s32 leading;
    s32 divisor;
    s32 digit;
    s32 i;
    float shift;
    GfxSprite *sprite;
    s16 *entry;

    black[0] = 0.0f;
    black[1] = 0.0f;
    black[2] = 0.0f;
    black[3] = 1.0f;
    grey[3] = 1.0f;
    grey[0] = 0.5f;
    grey[1] = 0.5f;
    grey[2] = 0.5f;
    red[0] = 1.0f;
    red[1] = 0.0f;
    red[2] = 0.0f;
    red[3] = 1.0f;

    target = SaveGetProfile()->data->points;
    decreasing = 0;
    if (self->shownPoints == target) {
        color[0] = black[0];
        color[1] = black[1];
        color[2] = black[2];
        color[3] = black[3];
        ActorPlayerStopPenaltySound(self->player);
    }
    else if (target < self->shownPoints) {
        decreasing = 1;
        color[0] = red[0];
        color[1] = red[1];
        color[2] = red[2];
        color[3] = red[3];
        self->shownPoints -= 5;
    }
    else if (self->shownPoints < target) {
        color[0] = grey[0];
        color[1] = grey[1];
        color[2] = grey[2];
        color[3] = grey[3];
        self->shownPoints += 5;
        ActorPlayerStopPenaltySound(self->player);
    }
    /* else (unreachable unless shownPoints changed meanwhile): color stays unset, as in the binary */

    sprite = ((GfxSprite **)self->base.data)[0x36];
    sprite->flags |= 1;
    if (decreasing) {
        sprite = ((GfxSprite **)self->base.data)[0x36];
        sprite->addColor[0] = 0.0f;
        sprite->addColor[1] = 0.0f;
        sprite->addColor[2] = 0.0f;
        sprite->addColor[3] = 1.0f;
        sprite = ((GfxSprite **)self->base.data)[0x36];
        sprite->tint[0] = color[0];
        sprite->tint[1] = color[1];
        sprite->tint[2] = color[2];
        sprite->alpha = color[3];
    }
    else {
        sprite = ((GfxSprite **)self->base.data)[0x36];
        sprite->addColor[0] = color[0];
        sprite->addColor[1] = color[1];
        sprite->addColor[2] = color[2];
        sprite->addColor[3] = color[3];
        sprite = ((GfxSprite **)self->base.data)[0x36];
        sprite->tint[0] = 1.0f;
        sprite->tint[1] = 1.0f;
        sprite->tint[2] = 1.0f;
        sprite->alpha = 1.0f;
    }

    shift = 0.0f;
    leading = 0;
    divisor = 1000000;
    for (i = 0; i < 7; i++) {
        digit = (self->shownPoints / divisor) % 10;
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[0x37 + i], (float)(digit / 5),
                         (float)(digit % 5));
        if (decreasing) {
            sprite = ((GfxSprite **)self->base.data)[0x37 + i];
            sprite->addColor[0] = 0.0f;
            sprite->addColor[1] = 0.0f;
            sprite->addColor[2] = 0.0f;
            sprite->addColor[3] = 1.0f;
            sprite = ((GfxSprite **)self->base.data)[0x37 + i];
            sprite->tint[0] = color[0];
            sprite->tint[1] = color[1];
            sprite->tint[2] = color[2];
            sprite->alpha = color[3];
        }
        else {
            sprite = ((GfxSprite **)self->base.data)[0x37 + i];
            sprite->addColor[0] = color[0];
            sprite->addColor[1] = color[1];
            sprite->addColor[2] = color[2];
            sprite->addColor[3] = color[3];
            sprite = ((GfxSprite **)self->base.data)[0x37 + i];
            sprite->tint[0] = 1.0f;
            sprite->tint[1] = 1.0f;
            sprite->tint[2] = 1.0f;
            sprite->alpha = 1.0f;
        }
        if (digit != 0) {
            leading = 1;
        }
        if (leading) {
            ((GfxSprite **)self->base.data)[0x37 + i]->flags |= 1;
        }
        else {
            ((GfxSprite **)self->base.data)[0x37 + i]->flags &= ~1u;
            if (divisor != 1) {
                shift += 5.5f;
            }
        }
        divisor /= 10;
    }

    ((GfxSprite **)self->base.data)[0x3d]->flags |= 1;
    for (i = 0x37; i < 0x3e; i++) {
        entry = UiLayoutGetEntry(0xc, i);
        ((GfxSprite **)self->base.data)[i]->posX = (float)entry[0] - shift;
    }

    if (self->player->scan != 0) {
        for (i = 0x36; i < 0x3e; i++) {
            ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
        }
    }
}
