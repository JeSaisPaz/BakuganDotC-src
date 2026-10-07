// bdc 0x089b17b4 UiBattleModeSelectPhaseMain
#include "bdc.h"

/* Main phase (2) of `UiBattleModeSelect`, stepped by `phaseStep`:
   0..2 slide the two entries and then the arrows in and show the glow of the selected entry;
   3 blinks the glow and handles input: Cross (`UiBattleModeSelectCheckConfirm`) on an enabled
   entry plays sound 0, flashes it and goes to step 7 (`cancelled` = 0), on a disabled entry plays
   the buzzer (sound 3); Circle (pad bit 0x2000) plays sound 2 and goes to step 5 (`cancelled` = 1);
   Left/Right play sound 1 and start the panel swap (step 4, back to 3 when done); 5/6 slide entries
   and arrows out, then step 9; 7 waits for the confirm flash, then step 5; 8 runs the common
   notice dialog and returns to 3; any other step stores the menu result
   (`UiBattleModeSelectApplyResult`) and advances to phase 3 with `phaseStep` 0. */

void UiBattleModeSelectPhaseMain(UiBattleModeSelect *self)
{
    u8 confirm;
    u8 entriesDone;

    switch (self->base.phaseStep) {
    case 0:
        UiBattleModeSelectBeginEntries(self, 0);
        self->base.phaseStep++;
        break;
    case 1:
        if (UiBattleModeSelectStepEntries(self, 0) == 1) {
            UiBattleModeSelectBeginArrows(self, 0);
            self->base.phaseStep++;
        }
        break;
    case 2:
        if (UiBattleModeSelectStepArrows(self, 0) == 1) {
            UiBattleModeSelectShowGlow(self, 1, (u8)self->cursor);
            self->base.phaseStep++;
        }
        break;
    case 3:
        UiBattleModeSelectStepGlow(self, (u8)self->cursor);
        confirm = (u8)UiBattleModeSelectCheckConfirm(self);
        if (confirm == 0) {
            if ((self->base.pad->buttons & 0x2000) != 0) {
                if (SndHasManager()) {
                    SndManagerPlay(SndGetManager(), 2, 0, 0);
                }
                self->cancelled = 1;
                self->base.phaseStep = 5;
                return;
            }
            if (UiBattleModeSelectHandleLeftRight(self) == 1) {
                if (SndHasManager()) {
                    SndManagerPlay(SndGetManager(), 1, 0, 0);
                }
                UiBattleModeSelectBeginSwitch(self);
                self->base.phaseStep = 4;
            }
        } else if (confirm == 1) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0, 0, 0);
            }
            UiBattleModeSelectFlashEntry(self);
            self->cancelled = 0;
            self->base.phaseStep = 7;
            return;
        } else {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 3, 0, 0);
            }
        }
        break;
    case 4:
        if (UiBattleModeSelectStepSwitch(self) == 1) {
            UiBattleModeSelectShowGlow(self, 1, (u8)self->cursor);
            self->base.phaseStep = 3;
        }
        break;
    case 5:
        UiBattleModeSelectBeginEntries(self, 1);
        UiBattleModeSelectBeginArrows(self, 1);
        self->base.phaseStep++;
        break;
    case 6:
        entriesDone = UiBattleModeSelectStepEntries(self, 1);
        if ((u8)(entriesDone + UiBattleModeSelectStepArrows(self, 1)) == 2) {
            self->base.phaseStep = 9;
        }
        break;
    case 7:
        if (UiBattleModeSelectStepFlash(self) == 1) {
            self->base.phaseStep = 5;
        }
        break;
    case 8:
        if (UiCommonNoticeRun() == 1) {
            self->base.phaseStep = 3;
        }
        break;
    default:
        UiBattleModeSelectApplyResult(self);
        self->base.phaseStep = 0;
        self->base.phase++;
        break;
    }
}
