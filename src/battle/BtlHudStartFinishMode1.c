// bdc 0x0882ce54 BtlHudStartFinishMode1
#include "bdc.h"

/* Once per battle (guarded by `finishStarted`), starts the HUD's end-of-battle display in mode 1:
   sets `finishMode` = 1, `finishState` = 1 and `finishStarted`, switches HUD layout sprites 3..8
   to texture slot 1, and, when the battle main task (task 100) exists, has it queue BGM 0x25 with
   a 0.5 s fade-out (`BtlMainQueueBgm`). Called by `BtlMainPhaseBattle`. */
void BtlHudStartFinishMode1(BtlHud *self)
{
    int i;

    if (self->finishStarted != 0) {
        return;
    }
    self->finishMode = 1;
    self->finishState = 1;
    self->finishStarted = 1;
    for (i = 3; i < 9; i++) {
        self->sprites[i]->textureSlot = 1;
    }
    if (BtlCameraTaskExists() != 0) {
        BtlMainQueueBgm((BtlMain *)BtlGetCameraTask(), 0x25, 0.5f);
    }
}
