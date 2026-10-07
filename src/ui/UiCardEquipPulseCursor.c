// bdc 0x0896c26c UiCardEquipPulseCursor
#include "bdc.h"

/* Runs the pulse animation (`UiPulseStepTint`, 20 frames) of the current cursor sprite of
   `UiCardEquip`: the tab highlight in row 0 or the card cursor in row 1. */

void UiCardEquipPulseCursor(UiCardEquip *self)

{
  s8 row = self->row;
  s32 idx;

  if (row > 0) {
    if (row < 2) {
      idx = self->groups[0x12][0];
      UiPulseStepTint(20.0f, ((GfxSprite **)self->base.data)[idx], &self->pulses[idx]);
    }
  } else if (row >= 0) {
    idx = self->groups[3][0] + self->rowCursor[0];
    UiPulseStepTint(20.0f, ((GfxSprite **)self->base.data)[idx], &self->pulses[idx]);
  }
  return;
}
