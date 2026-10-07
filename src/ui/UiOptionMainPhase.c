// bdc 0x089726fc UiOptionMainPhase
#include "bdc.h"

/* Offline main phase (entry 2 of the phase table `0x08a9d9f8`) of the battle-options screen (task
   304, `maybe_UiScreen304Ctor`; `"option_battle_t_%02d"` / `"option_sol00"` sprites; edits the
   battle rules stored in profile words 0x18..0x1b): builds all rows, opens them, then handles row
   movement (`UiOptionMoveCursorVertical`, `UiOptionMoveCursorButtons`), left/right value changes (`UiOptionChangeValue` +
   `UiOptionAnimateValueChange`), confirm on the OK/default buttons (`UiOptionCheckConfirm`; defaults
   reload the profile with `UiEquipResetBattleOptions` + `UiOptionSyncProfile`) and cancel (pad `pressed`
   bit 0x2000, sets `netDirty`); closes and advances `phase`. */

/* Plays UI sound `id` when the sound manager exists. */
static inline void UiOptionMainPlaySound(u32 id)
{
    if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), id, 0, 0);
    }
}

void UiOptionMainPhase(UiOption *self)
{
    u8 slideDone;

    switch (self->base.phaseStep) {
    case 0: /* build every sprite group, then start opening */
        UiOptionInitTitle(self);
        UiOptionInitHeader(self);
        UiOptionInitRowArrows(self);
        UiOptionInitRowPanels(self);
        UiOptionInitRowPlates(self);
        UiOptionInitRowNames(self);
        UiOptionInitRowFrames(self);
        UiOptionInitButtons(self);
        UiOptionInitButtonLabels(self);
        UiOptionRefreshValues(self);
        UiOptionRefreshAllRowArrows(self);
        UiOptionStartFade(self, 0);
        UiOptionStartSlide(self, 0);
        self->base.phaseStep = self->base.phaseStep + 1;
        break;
    case 1: /* opening: wait for slide and fade */
        slideDone = UiOptionUpdateSlide(self, 0);
        if ((u8)(slideDone + UiOptionUpdateFade(self, 0)) == 2) {
            UiOptionResetCursor(self);
            UiOptionShowHelp(self, true);
            self->base.phaseStep = self->base.phaseStep + 1;
        }
        break;
    case 2: /* input */
        UiOptionPulseCursor(self);
        UiOptionZoomButton(self);
        UiOptionBlinkButtonGlow(self);
        UiPulseStep(((GfxSprite **)self->base.data)[0x3a], &self->slots[0x3a].pulse);
        if (UiOptionCheckConfirm(self) == 1) {
            UiOptionMainPlaySound(0);
            UiOptionResetCursor(self);
            UiOptionStartButtonPress(self);
            self->base.phaseStep = 6;
        } else if ((self->base.pad->pressed & 0x2000) != 0) {
            UiOptionMainPlaySound(2);
            self->netDirty = 1;
            self->base.phaseStep = 4;
        } else if (UiOptionMoveCursorVertical(self) == 1) {
            UiOptionMainPlaySound(1);
            UiOptionResetCursor(self);
        } else if (UiOptionChangeValue(self) == 1) {
            UiOptionMainPlaySound(1);
            UiOptionStartArrowPress(self);
            UiOptionUpdateValueSprite(self);
            self->base.phaseStep = 3;
        } else if (UiOptionMoveCursorButtons(self) == 1) {
            UiOptionMainPlaySound(1);
            UiOptionResetCursor(self);
        }
        break;
    case 3: /* value-change animation */
        UiOptionPulseCursor(self);
        if (UiOptionAnimateValueChange(self) == 1) {
            UiOptionRefreshRowArrows(self, self->cursor);
            self->base.phaseStep = 2;
        }
        break;
    case 4: /* start closing */
        UiOptionShowHelp(self, false);
        UiOptionHideButtonGlow(self);
        UiOptionStartFade(self, 1);
        UiOptionStartSlide(self, 1);
        self->base.phaseStep = self->base.phaseStep + 1;
        break;
    case 5: /* closing: wait for slide and fade */
        slideDone = UiOptionUpdateSlide(self, 1);
        if ((u8)(slideDone + UiOptionUpdateFade(self, 1)) == 2) {
            self->base.phaseStep = 7;
        }
        break;
    case 6: /* button press animation */
        if (UiOptionWaitPress(self) == 1) {
            if ((s8)self->cursor == 4) { /* OK: close */
                UiOptionHideButtonGlow(self);
                self->base.phaseStep = 4;
            } else { /* Defaults: reload the default rules */
                UiEquipResetBattleOptions();
                UiOptionSyncProfile(self, false);
                UiOptionRefreshValues(self);
                UiOptionRefreshAllRowArrows(self);
                self->base.phaseStep = 2;
            }
        }
        break;
    default:
        UiOptionApply(self);
        self->base.phaseStep = 0;
        self->base.phase = self->base.phase + 1;
        break;
    }
}
