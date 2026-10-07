// bdc 0x08914fec UiUpgradeCanAfford
#include "bdc.h"

/* Returns 1 when upgrade slot `slot` of the selected Bakugan (`bakugan`) of the Bakugan upgrade
   screen (`UiUpgradeCtor`, task 490) can be bought: not yet owned (profile `upgradeOwned`), its
   predecessor slot owned or no prerequisite needed (`g_upgradePrereqFlags`), and the profile
   points minus save word 0x2d cover the cost (`g_upgradeInfo`) of the upgrade at the focused
   slot (`focus`); else 0. */

int UiUpgradeCanAfford(UiUpgrade *self, int slot)
{
  int points;
  u32 reserved;
  u8 id;

  if (SaveGetProfile()->data->upgradeOwned[self->bakugan][slot] != 0) {
    return 0;
  }
  if (slot > 0 && g_upgradePrereqFlags[g_upgradeClassTable[self->bakugan] * 6 + slot] != 0 &&
      SaveGetProfile()->data->upgradeOwned[self->bakugan][slot - 1] == 0) {
    return 0;
  }
  points = SaveGetProfile()->data->points;
  reserved = SaveProfileGetWord(SaveGetProfile(), 0x2d);
  points = points - (int)reserved;
  id = UiUpgradeGetUpgradeId(self->bakugan, self->focus);
  if (points < g_upgradeInfo[id].cost) {
    return 0;
  }
  return 1;
}
