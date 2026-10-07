// bdc 0x0886065c BtlBakuganResetGravity
#include "bdc.h"

/* Resets a battle unit's gravity state: clears the gravity suspension timer `+0x1c4` and the
   current gravity value `+0x2f8` (float), both driven by `BtlBakuganApplyGravity`, which then
   eases `+0x2f8` back toward the unit's gravity stat. */

void BtlBakuganResetGravity(BtlBakugan *bakugan)
{
  bakugan->gravityHold = 0;
  bakugan->gravity = 0.0f;
}
