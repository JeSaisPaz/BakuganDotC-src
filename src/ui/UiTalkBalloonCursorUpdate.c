// bdc 0x088c826c UiTalkBalloonCursorUpdate
#include "bdc.h"

/* Kind 3 update of a talk balloon sprite (`UiTalkBalloonSprite`: `GfxSprite` subclass, vtable
   `0x08af2d34`, owner window `+0x160`, kind `+0x168`, state `+0x16c`) (probably the `huki_cursor`
   choice cursor). State 1: fully opaque, placed at the owner's selected choice line
   (`choiceTop + lineHeight * choiceIndex - 6`), animates 8 frames of 32×32 cells
   (`g_uiTalkBalloonCursorFrames`), then holds ~21 frames and advances to state 2. State 3:
   fades out by 0.3 per frame. Always returns 0. */

s32 UiTalkBalloonCursorUpdate(GfxSprite *sprite)
{
    UiTalkBalloonSprite *self = (UiTalkBalloonSprite *)sprite;
    s32 state = self->state;

    if (state == 1) {
        UiTalkBalloon *owner;
        float rect[4];
        s32 hold;

        owner = self->owner;
        self->base.alpha = 1.0f;
        self->base.posY = owner->choiceTop + owner->lineHeight * (float)owner->choiceIndex - 6.0f;
        rect[1] = 0.0f;
        rect[2] = 32.0f;
        rect[0] = (float)((g_uiTalkBalloonCursorFrames[self->frame] & 3) << 5);
        rect[3] = 32.0f;
        GfxSpriteSetUvRectXYWH(sprite, rect);
        if (self->frame < 7) {
            self->frame = self->frame + 1;
        } else {
            hold = self->holdTimer;
            self->holdTimer = hold + 1;
            if (hold > 20) {
                self->state = self->state + 1;
            }
        }
    } else if (state == 3) {
        if (!(self->base.alpha <= 0.0f)) {
            self->base.alpha = self->base.alpha - 0.3f;
        }
    }
    return 0;
}
