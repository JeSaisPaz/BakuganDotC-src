// bdc 0x0882ced8 BtlHudStartFinishMode2
#include "bdc.h"

/* Once per battle (`finishStarted`), starts the HUD's end-of-battle display in mode 2:
   `finishMode = 2`, `finishState = 1`, texture slot 1 on HUD sprites 3..8, and, when the battle
   main task exists, queues BGM 0x25 with a 0.5 s fade on it (`BtlMainQueueBgm`). Same as
   `BtlHudStartFinishMode1` except for the mode. Called by `BtlMainPhaseBattle`. */
void BtlHudStartFinishMode2(BtlHud *self)
{
    s32 i;

    if (self->finishStarted != 0) {
        return;
    }
    self->finishMode = 2;
    self->finishState = 1;
    self->finishStarted = 1;
    for (i = 3; i < 9; i++) {
        self->sprites[i]->textureSlot = 1;
    }
    if (BtlCameraTaskExists()) {
        BtlMainQueueBgm(BtlGetCameraTask(), 0x25, 0.5f);
    }
}
