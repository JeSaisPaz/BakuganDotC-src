// bdc 0x089542fc UiBattleRuleSelectStartButtonTween
#include "bdc.h"

/* Sets up the slide-in (`out` = 0) or slide-out of the buttons of the battle-rule menu (task 340,
   `UiBattleRuleSelectCtor`; four battle types, reached from the battle-mode screen 350; the
   class purpose is inferred from what it writes to the save profile).
   Slide-in: centres the pivot of data sprites 1..8, enables linear filtering and unit scale, saves
   sprite 1's position as `panelHomeX/Y`, then for the four buttons (sprites/tweens 1..4) shows
   them, tints disabled entries (`buttonEnabled[i]` 0) grey 0.6, gives the cursor entry alpha 1 and
   the others 0.4, parks them at X = -224 and fills the tween (delay = 2 * enabled buttons before
   it, slide from -224 towards 704); disabled buttons are hidden again.
   Slide-out: hides sprite `cursor + 5` and snapshots alpha and scale X of the cursor button into
   its tween with `t` = 0. */

void UiBattleRuleSelectStartButtonTween(UiBattleRuleSelect *self, bool out)
{
    GfxSprite **sprites;
    GfxSprite *sprite;
    UiTween *tw;
    int i;
    int cur;
    s8 shown;
    u8 enabled;

    shown = 0;
    sprites = (GfxSprite **)self->base.data;
    if (out) {
        sprites[self->cursor + 5]->flags &= ~1u;
        cur = self->cursor;
        sprites = (GfxSprite **)self->base.data;
        tw = &self->tweens[cur + 1];
        tw->t = 0.0f;
        tw->startAlpha = sprites[cur + 1]->alpha;
        tw->startScale = sprites[cur + 1]->scaleX;
        return;
    }

    for (i = 1; i < 9; i++) {
        GfxSpriteCenterPivot(sprites[i]);
        ((GfxSprite **)self->base.data)[i]->flags |= 0x20; /* linear filter */
        UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
        sprites = (GfxSprite **)self->base.data;
    }
    self->panelHomeX = sprites[1]->posX;
    self->panelHomeY = sprites[1]->posY;

    for (i = 1; i < 5; i++) {
        tw = &self->tweens[i];
        ((GfxSprite **)self->base.data)[i]->flags |= 1;
        sprite = ((GfxSprite **)self->base.data)[i];
        if (self->cursor == i - 1) {
            if (self->buttonEnabled[i - 1] == 0) {
                sprite->tint[0] = 0.6f;
                sprite->tint[1] = 0.6f;
                sprite->tint[2] = 0.6f;
            } else {
                sprite->tint[0] = 1.0f;
                sprite->tint[1] = 1.0f;
                sprite->tint[2] = 1.0f;
            }
            sprite->alpha = 1.0f;
        } else {
            if (self->buttonEnabled[i - 1] == 0) {
                sprite->tint[0] = 0.6f;
                sprite->tint[1] = 0.6f;
                sprite->tint[2] = 0.6f;
            } else {
                sprite->tint[0] = 1.0f;
                sprite->tint[1] = 1.0f;
                sprite->tint[2] = 1.0f;
            }
            sprite->alpha = 0.4f;
        }
        sprite = ((GfxSprite **)self->base.data)[i];
        GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
        ((GfxSprite **)self->base.data)[i]->posX = -224.0f;
        tw->delay0b = (u8)(shown * 2);
        sprites = (GfxSprite **)self->base.data;
        tw->toggle07 = 0;
        tw->t = 0.0f;
        enabled = self->buttonEnabled[i - 1];
        tw->slideStart = (s16)sprites[i]->posX;
        tw->slideDelta = (s16)(704.0f - sprites[i]->posX);
        if (enabled == 0) {
            sprites[i]->flags &= ~1u;
        } else {
            shown++;
        }
    }
}
