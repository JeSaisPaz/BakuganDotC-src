// bdc 0x08954e44 UiBattleRuleSelectAnimateCursorMove
#include "bdc.h"

/* Animates the cursor of the battle-rule menu (task 340, `UiBattleRuleSelectCtor`; four battle
   types, reached from the battle-mode screen 350; the class purpose is inferred from what it writes
   to the save profile) sliding to the newly selected entry after a direction press (pad `repeat` /
   `pressed` bits 0x80/0x20; direction `switchDir`, speed `switchFrames` = 4.0). Each frame the
   previous and the new entry sprite (sprites prevCursor+1 / cursor+1) move along an ease-out curve
   from their tween's `slideStart` by `slideDelta` (added for direction 0, subtracted for 1).
   Returns 0 while `switchFrames` is 0 or the move is still running; once `switchT` reaches 1 both
   sprites snap to `slideEnd`, sprite 20's alpha is set to 1 and copied to `switchAlpha`, and it
   returns 1. */

s32 UiBattleRuleSelectAnimateCursorMove(UiBattleRuleSelect *self)
{
    PadState *pad;
    GfxSprite **sprites;
    s32 idx;
    float frames;
    float t;
    float start;
    float delta;
    float ease;

    UiFlashStep(0);
    pad = self->base.pad;
    if (pad->repeat & 0x80) {
        if (self->switchDir == 0) {
            self->switchFrames = 4.0f;
        }
    } else if (pad->repeat & 0x20) {
        if (self->switchDir == 1) {
            self->switchFrames = 4.0f;
        }
    }
    if (pad->pressed & 0x80) {
        if (self->switchDir == 0) {
            self->switchFrames = 4.0f;
            self->switchQueued = 1;
        }
    } else if (pad->pressed & 0x20) {
        if (self->switchDir == 1) {
            self->switchQueued = 1;
            self->switchFrames = 4.0f;
        }
    }

    frames = self->switchFrames;
    if (frames == 0.0f) {
        return 0;
    }

    idx = self->prevCursor;
    start = (float)self->tweens[idx + 1].slideStart;
    delta = (float)self->tweens[idx + 1].slideDelta;
    t = self->switchT + 1.0f / frames;
    self->switchT = t;
    sprites = (GfxSprite **)self->base.data;
    if (self->switchDir == 0) {
        sprites[idx + 1]->posX = start + (1.0f - (t - 1.0f) * (t - 1.0f)) * delta;
        idx = self->cursor;
        t = self->switchT - 1.0f;
        ease = 1.0f - t * t;
        ((GfxSprite **)self->base.data)[idx + 1]->posX =
            (float)self->tweens[idx + 1].slideStart + ease * (float)self->tweens[idx + 1].slideDelta;
    } else {
        sprites[idx + 1]->posX = start - (1.0f - (t - 1.0f) * (t - 1.0f)) * delta;
        idx = self->cursor;
        t = self->switchT - 1.0f;
        ease = 1.0f - t * t;
        ((GfxSprite **)self->base.data)[idx + 1]->posX =
            (float)self->tweens[idx + 1].slideStart - ease * (float)self->tweens[idx + 1].slideDelta;
    }
    if (self->switchT < 1.0f) {
        return 0;
    }

    idx = self->prevCursor;
    ((GfxSprite **)self->base.data)[idx + 1]->posX = (float)self->tweens[idx + 1].slideEnd;
    idx = self->cursor;
    ((GfxSprite **)self->base.data)[idx + 1]->posX = (float)self->tweens[idx + 1].slideEnd;
    ((GfxSprite **)self->base.data)[20]->alpha = 1.0f;
    self->switchAlpha = ((GfxSprite **)self->base.data)[20]->alpha;
    return 1;
}
