// bdc 0x08914508 UiUpgradeStopSlotPulse
#include "bdc.h"

/* Stops the highlight pulse (`UiCursorGlowStep`) on the selected slot's sprite of the Bakugan upgrade
   screen (`UiUpgradeCtor`, task 490; selected Bakugan `+0x16a8`, selected slot `+0x1698`): sprite
   `+0xcc` for slot 6, otherwise the slot sprite `+0xb0 + 4*slot` when the slot is unavailable or
   its prerequisite (profile byte `+0x53f + id*6 + slot`) is owned. */

void UiUpgradeStopSlotPulse(UiUpgrade *self)

{
  SaveProfile *profile;
  int slot;

  slot = self->focus;
  if (slot == 6) {
    UiCursorGlowStep(((GfxSprite **)(self->base).data)[0xcc / 4]);
  }
  else {
    if (slot > 0 && g_upgradePrereqFlags[g_upgradeClassTable[self->bakugan] * 6 + slot] != 0) {
      profile = SaveGetProfile();
      slot = self->focus;
      if (profile->data->upgradeOwned[self->bakugan][slot - 1] == 0) {
        return;
      }
    }
    UiCursorGlowStep(((GfxSprite **)(self->base).data)[0xb0 / 4 + slot]);
  }
}
