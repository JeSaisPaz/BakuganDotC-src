// bdc 0x0893b9ac UiUnlockResultAnimateFlash
#include "bdc.h"

/* Steps the reveal flash of the unlock-result screen (task 375, `UiUnlockResultCtor`) by one
   frame (`flashScale` frame counter, `flashTimer` step) and returns 1 when it is finished (step
   past 3 or negative), or when no flash was requested (`flashRequested` clear); 0 while it runs.
   Step 0 shows the glow sprite 0x23 once the frame count exceeds 4; steps 0-1 scale burst sprite
   0x24 by `frame * 0.25` and hide it after frame 8; step 2 waits 30 frames; step 3 grows the glow
   (`1 + frame * 0.125`) while fading it out and hides it after frame 8. */

s32 UiUnlockResultAnimateFlash(UiUnlockResult *self)
{
    s32 step;
    s32 frame;
    GfxSprite *sprite;
    float scale;

    if (self->flashRequested == 0) {
        return 1;
    }
    step = self->flashTimer;
    if (step < 2) {
        if (step < 0) {
            return 1;
        }
        frame = self->flashScale;
        if (step <= 0 && !((float)frame <= 4.0f)) {
            ((GfxSprite **)self->base.data)[0x23]->flags |= 1;
            ((GfxSprite **)self->base.data)[0x23]->alpha = 1.0f;
            self->flashTimer = self->flashTimer + 1;
            frame = self->flashScale;
        }
        sprite = ((GfxSprite **)self->base.data)[0x24];
        if (!(frame < 9)) {
            sprite->flags &= ~1u;
            self->flashScale = 0;
            self->flashTimer = self->flashTimer + 1;
            frame = 0;
            sprite = ((GfxSprite **)self->base.data)[0x24];
        }
        GfxSpriteSetScaleRotation(sprite, (float)frame * 0.25f, (float)frame * 0.25f, 0.0f, false);
        frame = self->flashScale;
    } else if (step < 3) {
        frame = self->flashScale;
        if (!(frame < 31)) {
            self->flashScale = 0;
            self->flashTimer = step + 1;
            frame = 0;
        }
    } else {
        if (!(step < 4)) {
            return 1;
        }
        scale = (float)self->flashScale * 0.125f + 1.0f;
        GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[0x23], scale, scale, 0.0f, false);
        sprite = ((GfxSprite **)self->base.data)[0x23];
        sprite->alpha = sprite->alpha - (float)self->flashScale * 0.125f;
        frame = self->flashScale;
        if (!(frame < 9)) {
            ((GfxSprite **)self->base.data)[0x23]->flags &= ~1u;
            self->flashScale = 0;
            self->flashTimer = self->flashTimer + 1;
            frame = 0;
        }
    }
    self->flashScale = frame + 1;
    return 0;
}
