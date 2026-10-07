// bdc 0x088c810c UiTalkBalloonCheckUpdate
#include "bdc.h"

/* Kind 2 update of a talk balloon sprite (`UiTalkBalloonSprite`: `GfxSprite` subclass, vtable
   `0x08af2d34`, owner window `+0x160`, kind `+0x168`, state `+0x16c`) (probably the `huki_check`
   icon). Every call steps `frame` through the 13-step pattern `g_uiTalkBalloonCheckPattern`.
   States 1 and 2: state 1 switches to 2 (phase 1.0, frame restarted at 1); the sprite fades in by
   0.2 per frame below alpha 1 and shows the 24×32 UV cell picked by the pattern entry. State 0:
   fades out by 0.2 per frame. Always returns 0. */

s32 UiTalkBalloonCheckUpdate(GfxSprite *sprite)
{
    UiTalkBalloonSprite *self = (UiTalkBalloonSprite *)sprite;
    s32 frame = self->frame;
    s32 cell = g_uiTalkBalloonCheckPattern[(u32)frame % 13];
    s32 state = self->state;
    float rect[4];

    self->frame = frame + 1;
    if (state <= 0) {
        if (state == 0 && !(self->base.alpha <= 0.0f)) {
            self->base.alpha = self->base.alpha - 0.2f;
        }
        return 0;
    }
    if (state < 2) {
        self->state = state + 1;
        self->phase = 1.0f;
        self->frame = 1;
    } else if (state >= 3) {
        return 0;
    }
    if (self->base.alpha < 1.0f) {
        self->base.alpha = self->base.alpha + 0.2f;
    }
    rect[0] = g_uiTalkBalloonCheckCellU[cell & 1];
    rect[1] = g_uiTalkBalloonCheckCellV[(cell >> 1) & 1];
    rect[2] = 24.0f;
    rect[3] = 32.0f;
    GfxSpriteSetUvRectXYWH(sprite, rect);
    return 0;
}
