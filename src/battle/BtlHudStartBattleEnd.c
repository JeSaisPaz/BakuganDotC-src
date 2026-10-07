// bdc 0x0883f2dc BtlHudStartBattleEnd
#include "bdc.h"

/* Switches the battle HUD (`BtlHudCtor`) to phase 3 (battle end, `BtlHudPhaseBattleEnd`):
   sets `phase` = 3 and `phaseStep` = 0, activates UI window 7 and, when `g_btlBattleOver` is
   set, also window 4, 5 or 6 for battle outcome (`g_btlBattleOutcome`) 1, 2 or 4 (no extra
   window for other outcomes). */

void BtlHudStartBattleEnd(BtlHud *self)
{
  self->phase = 3;
  self->phaseStep = 0;
  UiSetWindowActive(7, 1);
  if (g_btlBattleOver != 0) {
    switch (g_btlBattleOutcome) {
    case 1:
      UiSetWindowActive(4, 1);
      break;
    case 2:
      UiSetWindowActive(5, 1);
      break;
    case 4:
      UiSetWindowActive(6, 1);
      break;
    default:
      break;
    }
  }
}
