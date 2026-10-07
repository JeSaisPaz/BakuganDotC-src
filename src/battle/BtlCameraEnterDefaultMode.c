// bdc 0x0884dd54 BtlCameraEnterDefaultMode
#include "bdc.h"

/* Switches the battle main task (`camera`, task id 100) back to its default follow
   camera: releases the control lock of every unit (BtlSetControlLockAll(0)) when
   global script bit 0x22 is set, resets the embedded follow camera
   (BtlCameraSetDefaultFollow, no snap), sets g_btlCameraDefaultMode, resumes the
   battle event script and sets both the update and draw phase to 1, or to 2 once the
   battle outcome is decided (g_btlBattleOutcome != 0). */
void BtlCameraEnterDefaultMode(void *camera)
{
    BtlMain *self = (BtlMain *)camera;
    s32 phase;

    if (CoreBitsetTest(0x22, g_scriptGlobalBits)) {
        BtlSetControlLockAll(0);
    }
    BtlCameraSetDefaultFollow(&self->camera, 0);
    g_btlCameraDefaultMode = 1;
    BtlStageResumeEventScript();
    phase = (g_btlBattleOutcome != 0) ? 2 : 1;
    self->phase = phase;
    self->drawPhase = phase;
}
