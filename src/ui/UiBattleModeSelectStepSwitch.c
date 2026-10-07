// bdc 0x089b1428 UiBattleModeSelectStepSwitch
#include "bdc.h"

/* Steps the panel swap of `UiBattleModeSelect`: runs the arrow flash
   (`UiFlashStep`), advances `t` (`+0x9b8`) by `1 / speed` (`+0x9b4`) and eases both panels (`1 -
   (t-1)^2`) along the slides set by `UiBattleModeSelectBeginSwitch`. Pressing or auto-repeating
   Left/Right in the current direction during the swap shortens it (speed 4.0), and a new press
   queues another switch (`+0x9b1`). Returns false while `t < 1`; otherwise snaps both panels to
   their end X, sets sprite 0 alpha to 1 (copied to `+0x588`) and returns true. */

bool UiBattleModeSelectStepSwitch(UiBattleModeSelect *self)
{
    PadState *pad;
    GfxSprite **sprites;
    UiTween *tween;
    int prev;
    int cur;
    u8 dir;
    float start;
    float delta;
    float t;
    float u;

    UiFlashStep(0);
    pad = self->base.pad;
    prev = self->prevCursor;
    dir = self->switchDir;
    t = self->switchT;
    tween = &self->tweens[prev + 4];
    start = (float)tween->slideStart;
    delta = (float)tween->slideDelta;
    sprites = (GfxSprite **)self->base.data;

    /* auto-repeat Left (dir 0) / Right (dir 1): speed the swap up */
    if ((pad->repeat & 0x80) != 0) {
        if (dir == 0) {
            self->switchFrames = 4.0f;
        }
    } else if ((pad->repeat & 0x20) != 0 && dir == 1) {
        self->switchFrames = 4.0f;
    }
    /* fresh press in the same direction: speed up and queue another switch */
    if ((pad->pressed & 0x80) != 0) {
        if (dir == 0) {
            self->switchFrames = 4.0f;
            self->switchQueued = 1;
        }
    } else if ((pad->pressed & 0x20) != 0 && dir == 1) {
        self->switchQueued = 1;
        self->switchFrames = 4.0f;
    }

    t = t + 1.0f / self->switchFrames;
    self->switchT = t;
    if (dir == 0) {
        sprites[prev + 4]->posX = start + (1.0f - (t - 1.0f) * (t - 1.0f)) * delta;
        cur = self->cursor;
        u = self->switchT - 1.0f;
        tween = &self->tweens[cur + 4];
        ((GfxSprite **)self->base.data)[cur + 4]->posX =
            (float)tween->slideStart + (1.0f - u * u) * (float)tween->slideDelta;
    } else {
        sprites[prev + 4]->posX = start - (1.0f - (t - 1.0f) * (t - 1.0f)) * delta;
        cur = self->cursor;
        u = self->switchT - 1.0f;
        tween = &self->tweens[cur + 4];
        ((GfxSprite **)self->base.data)[cur + 4]->posX =
            (float)tween->slideStart - (1.0f - u * u) * (float)tween->slideDelta;
    }
    if (self->switchT < 1.0f) {
        return false;
    }

    prev = self->prevCursor;
    ((GfxSprite **)self->base.data)[prev + 4]->posX = (float)self->tweens[prev + 4].slideEnd;
    cur = self->cursor;
    ((GfxSprite **)self->base.data)[cur + 4]->posX = (float)self->tweens[cur + 4].slideEnd;
    ((GfxSprite **)self->base.data)[0]->alpha = 1.0f;
    self->switchAlpha = ((GfxSprite **)self->base.data)[0]->alpha;
    return true;
}
