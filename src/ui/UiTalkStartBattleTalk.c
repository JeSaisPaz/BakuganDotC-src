// bdc 0x0882c2b4 UiTalkStartBattleTalk
#include "bdc.h"

/* Starts a battle talk on the `BtlHud` talk/HUD task (`UiGetTalkTask`, id 0x6e): unless `unit`
   is dead (`combat.dead`, `+0x4c1`), stores it in `abilityCutInUnit` (`+0x68`) and sets
   `cutInState` (`+0x394`) to 1; `BtlMainPhaseTalk` then waits for `cutInState` to drop back to 0.
    */

void UiTalkStartBattleTalk(BtlHud *hud, BtlBakugan *unit)

{
  if ((unit->combat).dead == '\0') {
    hud->cutInState = 1;
    hud->abilityCutInUnit = unit;
  }
  return;
}

