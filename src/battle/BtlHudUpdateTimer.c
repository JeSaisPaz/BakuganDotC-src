// bdc 0x0882f398 BtlHudUpdateTimer
#include "bdc.h"

/* Timer widget of the battle HUD (`BtlHudUpdate`) (`BtlHudPhaseMain`): reads the remaining time
   (profile word 2, in frames, `SaveProfileGetWord`). -1 (no time limit) writes `999:999` with
   `BtlHudSetTimerDigits`, then hides digit sprites 3..9 and sets their tint to black, alpha 1;
   otherwise shows sprites 3..9 and writes `(t / 60, t % 60)`. While window 0xb is active
   (`UiGetWindowActive`) and `timerScroll[0]` exists, moves it down by the frame step
   (`frameSkip + 1` of `g_gfxDisplay`); past 200 it restarts at a random height in [-128, 0)
   (`PlatformRandFloat12() - 1`, VFPU `vrndf1` minus the bank's 1) with `timerScroll[1]` 8 above it. */
void BtlHudUpdateTimer(BtlHud *self)
{
    GfxSprite *sprite;
    s32 time;
    s32 i;
    float rnd;

    time = (s32)SaveProfileGetWord(SaveGetProfile(), 2);
    if (time == -1) {
        BtlHudSetTimerDigits(self, 999, 999);
        for (i = 3; i < 10; i++) {
            self->sprites[i]->flags &= ~1u;
            sprite = self->sprites[i];
            /* tint/alpha = {0, 0, 0, 1} (one quad store) */
            sprite->tint[0] = 0.0f;
            sprite->tint[1] = 0.0f;
            sprite->tint[2] = 0.0f;
            sprite->alpha = 1.0f;
        }
    } else {
        for (i = 3; i < 10; i++) {
            self->sprites[i]->flags |= 1;
        }
        BtlHudSetTimerDigits(self, time / 60, time % 60);
    }
    if (UiGetWindowActive(0xb) == 0 || self->timerScroll[0] == NULL) {
        return;
    }
    self->timerScroll[0]->posY = self->timerScroll[0]->posY + (float)(g_gfxDisplay->frameSkip + 1);
    if (self->timerScroll[0]->posY <= 200.0f) {
        return;
    }
    rnd = PlatformRandFloat12() - 1.0f;
    self->timerScroll[0]->posY = rnd * 128.0f + -128.0f;
    self->timerScroll[1]->posY = self->timerScroll[0]->posY - 8.0f;
}
