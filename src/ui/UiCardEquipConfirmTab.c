// bdc 0x0896e808 UiCardEquipConfirmTab
#include "bdc.h"

/* Runs the action of the confirmed item of `UiCardEquip`. In row 0 the tab cursor
   maps to an action: a Bakugan tab (small layout, `bakuganCount < 3`: tabs 0/3 = Bakugan 0/1; large
   layout: tabs 0/1/4/5 = Bakugan 0..3) sets `selBakugan` and opens its card grid (row 1, card
   cursor 0, row focus moved via `UiCardEquipSetRowFocus`, card name/help shown, step 6); Done
   (small tab 1, large tab 2) sets step 3; Options (small tab 2, large tab 3) enters phase 3 step 0
   (`UiCardEquipPhaseOptions`). Other cursor values open the card grid without changing
   `selBakugan`. In row 1 it returns to step 6; in row 2 it resets the gauge arrows
   (`UiCardEquipGreyGaugeLimits`) and sets step 7. */

void UiCardEquipConfirmTab(UiCardEquip *self)
{
  s32 action; /* 0 open card grid, 1 Done, 2 Options */
  s8 row;
  s8 cursor;

  row = self->row;
  action = 0;
  if (row > 0) {
    if (row < 2) {
      self->base.phaseStep = 6;
    } else if (row < 3) {
      UiCardEquipGreyGaugeLimits(self, (u8)self->selBakugan);
      self->base.phaseStep = 7;
    }
    return;
  }
  if (row < 0) {
    return;
  }
  cursor = self->rowCursor[0];
  if (self->bakuganCount < 3) {
    if (cursor < 2) {
      if (cursor >= 0) {
        if (cursor > 0) {
          action = 1;
        } else {
          action = 0;
          self->selBakugan = 0;
        }
      }
    } else if (cursor < 3) {
      action = 2;
    } else if (cursor < 4) {
      action = 0;
      self->selBakugan = 1;
    }
  } else if ((u32)cursor < 6) {
    switch (cursor) {
    case 1:
      action = 0;
      self->selBakugan = 1;
      break;
    case 2:
      action = 1;
      break;
    case 3:
      action = 2;
      break;
    case 4:
      action = 0;
      self->selBakugan = 2;
      break;
    case 5:
      action = 0;
      self->selBakugan = 3;
      break;
    default:
      action = 0;
      self->selBakugan = 0;
      break;
    }
  }
  if (action > 0) {
    if (action < 2) {
      self->base.phaseStep = 3;
    } else if (action < 3) {
      self->base.phase = 3;
      self->base.phaseStep = 0;
    }
  } else if (action >= 0) {
    UiCardEquipSetRowFocus(self, 0, 0);
    self->row = 1;
    row = self->row;
    self->rowCursor[1] = 0;
    UiCardEquipSetRowFocus(self, (u8)row, 1);
    UiCardEquipShowCardName(self, 1);
    UiCardEquipShowCardHelp(self, 1);
    self->base.phaseStep = 6;
  }
}
