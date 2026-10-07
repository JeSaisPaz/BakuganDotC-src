// bdc 0x0894d6fc UiBattleRecordMenuPhase
#include "bdc.h"

/* Category-menu phase (entry 2 of the phase table `0x08a9d19c` (load `UiBattleRecordLoadPhase`, finish
   `UiBattleRecordFinishPhase`, menu, open `UiBattleRecordOpenPhase`, mode list, totals list `UiBattleRecordTotalListPhase`)) of the
   battle-record screen (task 3005, `UiBattleRecordCtor`; per-Bakugan win/loss statistics from the
   save profile, layout package `"data/2d/%s/record.lzs"`): runs `UiBattleRecordMenuInput`; cancel
   (bit 0x20, sound 2) goes to the finish phase 1; confirm (bit 0x40, sound 0) plays
   `UiBattleRecordAnimateMenuSwitch` and then, for entries 0..2, loads wins/losses/draws of that
   mode for the 20 Bakugan and opens the mode list (phase 4, `UiBattleRecordBuildModeList`); for
   entry 3 sums all modes and opens the totals list (phase 5, `UiBattleRecordBuildTotalList`).
   Bakugan not owned get zero counts. Sub-state 2 replays the switch animation backwards after
   returning. */

void UiBattleRecordMenuPhase(UiBattleRecord *self)
{
    PadState *pad;
    s32 step;
    s32 mode;
    s32 i;
    s32 total2;
    s32 total0;

    step = self->base.phaseStep;
    if (step == 0) {
        UiBattleRecordMenuInput(self);
        pad = self->base.pad;
        if (pad->pressed & 0x4000) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0, 0, 0);
            }
            self->animFrame = 0;
            self->base.phaseStep = 1;
        } else if (pad->pressed & 0x2000) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 2, 0, 0);
            }
            self->base.phaseStep = 0;
            self->base.phase = 1;
        }
    } else if (step == 1) {
        if (UiBattleRecordAnimateMenuSwitch(self, 1) == 0) {
            return;
        }
        self->base.phaseStep = 0;
        mode = self->mode;
        self->scrollTop = 0;
        self->animFrame = 0;
        if (mode >= 0 && mode < 3) {
            memset(self->wins, 0, sizeof(self->wins));
            memset(self->losses, 0, sizeof(self->losses));
            memset(self->draws, 0, sizeof(self->draws));
            for (i = 0; i < 20; i++) {
                if (self->owned[i] == 0) {
                    self->wins[i] = 0;
                    self->losses[i] = 0;
                    self->draws[i] = 0;
                } else {
                    mode = self->mode;
                    self->wins[i] = (s16)SaveRecordGetWins(SaveGetProfile(), i + 1, mode);
                    self->losses[i] = (s16)SaveRecordGetLosses(SaveGetProfile(), i + 1, mode);
                    self->draws[i] = (s16)(SaveRecordGetBattles(SaveGetProfile(), i + 1, mode) -
                                           (self->wins[i] + self->losses[i]));
                }
            }
            self->base.phase = 4;
            UiBattleRecordBuildModeList(self);
        } else if (mode == 3) {
            memset(self->battles, 0, sizeof(self->battles));
            for (i = 0; i < 20; i++) {
                if (self->owned[i] == 0) {
                    self->battles[i] = 0;
                } else {
                    total2 = SaveRecordGetBattles(SaveGetProfile(), i + 1, 2);
                    total0 = SaveRecordGetBattles(SaveGetProfile(), i + 1, 0);
                    self->battles[i] =
                        (s16)(total0 + SaveRecordGetBattles(SaveGetProfile(), i + 1, 1) + total2);
                }
            }
            self->base.phase = 5;
            UiBattleRecordBuildTotalList(self);
        }
    } else if (step == 2) {
        if (UiBattleRecordAnimateMenuSwitch(self, 0) != 0) {
            self->base.phaseStep = 0;
            self->animFrame = 0;
        }
    }
}
