// bdc 0x0894b014 UiBattleRecordModeListPhase
#include "bdc.h"

/* Per-mode list phase (entry 4 of the phase table `0x08a9d19c` (load `UiBattleRecordLoadPhase`, finish
   `UiBattleRecordFinishPhase`, menu, open `UiBattleRecordOpenPhase`, mode list, totals list `UiBattleRecordTotalListPhase`)) of the
   battle-record screen (task 3005, `UiBattleRecordCtor`; per-Bakugan win/loss statistics from the
   save profile, layout package `"data/2d/%s/record.lzs"`): opens the table (`UiBattleRecordFadeModeTable(screen,
   0)`), scrolls it with the pad (`UiBattleRecordListScrollInput`, `UiBattleRecordStartModeScroll`,
   `UiBattleRecordUpdateModeScroll`), and on cancel (pad `pressed` bit 0x2000, sound 2) closes it and
   returns to the menu (`UiBattleRecordReturnToMenu`). */

void UiBattleRecordModeListPhase(UiBattleRecord *self)
{
  switch (self->base.phaseStep) {
  case 0:
    if (UiBattleRecordFadeModeTable(self, 0) != 0) {
      self->base.phaseStep = 1;
      self->animFrame = 0;
    }
    break;
  case 1:
    if (UiBattleRecordListScrollInput(self) != 0) {
      UiBattleRecordStartModeScroll(self);
      self->base.phaseStep = 2;
    } else if ((self->base.pad->pressed & 0x2000) != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      self->base.phaseStep = 3;
    }
    break;
  case 2:
    if (UiBattleRecordUpdateModeScroll(self) != 0) {
      self->base.phaseStep = 1;
    }
    break;
  case 3:
    if (UiBattleRecordFadeModeTable(self, 1) != 0) {
      UiBattleRecordReturnToMenu(self);
    }
    break;
  }
}
