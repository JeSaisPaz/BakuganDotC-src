// bdc 0x08961fb0 UiEquipResetHelpPopup
#include "bdc.h"

/* Resets the help pop-up state of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): step byte `+0x5114` = 0 and the two words `+0x510c/+0x5110` = 0, before
   `UiEquipShowHelpPopup` runs. */

void UiEquipResetHelpPopup(UiEquip *self)

{
  self->popupStep = '\0';
  self->popupT = 0.0f;
  self->popupFade = 0.0f;
  return;
}

