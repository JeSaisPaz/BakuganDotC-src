// bdc 0x089149d8 UiUpgradeResetConfirm
#include "bdc.h"

/* Resets the confirmation sub-state of the Bakugan upgrade screen (`UiUpgradeCtor`, task 490;
   selected Bakugan `+0x16a8`, selected slot `+0x1698`): step `+0x16a4 = 0`, result `+0x16a0 = 0`.
    */

void UiUpgradeResetConfirm(UiUpgrade *self)

{
  self->confirmStep = 0;
  self->confirmChoice = 0;
  return;
}

