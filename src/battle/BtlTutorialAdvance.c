// bdc 0x0884676c BtlTutorialAdvance
#include "bdc.h"

/* Tutorial step helper, needs the player's Bakugan (vtable slot 18 true):
   advancePhase 0 clears the player's stats and applies the action masks of
   the current tutorial step (script global variable 11), then moves to
   phase 1; phase 1 waits for `done`; phases 1 (once done) and 2 increment the
   tutorial step, reset the phase to 0 and return 1. Returns 0 otherwise. */
int BtlTutorialAdvance(void *task, bool done)
{
    BtlTutorialTask *self = (BtlTutorialTask *)task;
    s32 step = g_scriptGlobalVars[11];
    BtlBakugan *player = BtlGetPlayerBakugan();
    BtlBakugan *bakugan = NULL;
    const VtblEntry *isBakugan;

    if (player == NULL) {
        return 0;
    }
    isBakugan = &((const VtblEntry *)player->base.base.vtable)[18];
    if (((s32 (*)(void *))isBakugan->fn)((u8 *)player + isBakugan->delta) != 0) {
        bakugan = player;
    }
    if (bakugan == NULL) {
        return 0;
    }
    if (self->advancePhase <= 0) {
        if (self->advancePhase == 0) {
            BtlGetBakuganList();
            if (bakugan->stats != NULL) {
                BtlStatsClear(bakugan->stats);
            }
            BtlTutorialSetActionMasks(task, step);
            self->advancePhase++;
        }
        return 0;
    }
    if (self->advancePhase < 2) {
        if (!done) {
            return 0;
        }
        self->advancePhase++;
    } else if (self->advancePhase > 2) {
        return 0;
    }
    g_scriptGlobalVars[11] = step + 1;
    self->advancePhase = 0;
    return 1;
}
