// bdc 0x0894c1d4 UiBattleRecordTotalListPhase
#include "bdc.h"

/* Phase 5 of `UiBattleRecord` (totals list), stepped by `phaseStep`:
   0 fades the list in (`UiBattleRecordFadeTotalList` dir 0) then goes to 1 with `animFrame` 0;
   1 polls `UiBattleRecordListScrollInput` (scroll → `UiBattleRecordStartTotalScroll`, step 2)
   or Circle (SE 2, step 3); 2 runs `UiBattleRecordUpdateTotalScroll` back to step 1; 3 fades
   the list out and then returns to the menu (`UiBattleRecordReturnToMenu`). */

void UiBattleRecordTotalListPhase(UiBattleRecord *self)
{
  switch (self->base.phaseStep) {
  case 0:
    if (UiBattleRecordFadeTotalList(self, 0) != 0) {
      self->base.phaseStep = 1;
      self->animFrame = 0;
    }
    break;
  case 1:
    if (UiBattleRecordListScrollInput(self) != 0) {
      UiBattleRecordStartTotalScroll(self);
      self->base.phaseStep = 2;
    }
    else if ((self->base.pad->pressed & 0x2000) != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      self->base.phaseStep = 3;
    }
    break;
  case 2:
    if (UiBattleRecordUpdateTotalScroll(self) != 0) {
      self->base.phaseStep = 1;
    }
    break;
  case 3:
    if (UiBattleRecordFadeTotalList(self, 1) != 0) {
      UiBattleRecordReturnToMenu(self);
    }
    break;
  }
}
