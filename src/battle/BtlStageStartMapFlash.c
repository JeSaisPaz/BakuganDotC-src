// bdc 0x0889d8ac BtlStageStartMapFlash
#include "bdc.h"

/* Starts the arena colour flash: records the triggering Bakugan's kind, enters state 1 (fading in)
   with the hold timer and fade level cleared, and installs the flash material callback (reading
   the fade level) on each loaded arena model. */

void BtlStageStartMapFlash(s32 kind)
{
    s32 slot;

    g_btlMapFlashKind = kind;
    g_btlMapFlashState = 1;
    g_btlMapFlashTimer = 0;
    g_btlMapFlashLevel = 0.0f;
    for (slot = 0; slot < 3; slot++) {
        if (g_btlArenaModels[slot] != NULL) {
            BtlStageModelSetFlash(g_btlArenaModels[slot], &g_btlMapFlashLevel, 1);
        }
    }
}
