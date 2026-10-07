// bdc 0x08888bdc BtlCombatArtSlotCanCharge
#include "bdc.h"

/* Returns 1 when special-art slot `slot` (0/1) of a `BtlCombatState` may gain charge: its
   `listedStatusIds` entry is 0 and its `artCooldown` has run out (<= 0). With `tickCooldown` set
   it also decrements a running cooldown by one frame (clamped at 0); returns 0 then. */
int BtlCombatArtSlotCanCharge(BtlCombatState *combat, int slot, char tickCooldown)
{
  float cooldown;

  if (combat->listedStatusIds[slot] == 0) {
    if (combat->artCooldown[slot] <= 0.0f) {
      return 1;
    }
    if (tickCooldown != 0) {
      cooldown = combat->artCooldown[slot] - 1.0f;
      combat->artCooldown[slot] = cooldown;
      if (cooldown < 0.0f) {
        combat->artCooldown[slot] = 0.0f;
      }
    }
  }
  return 0;
}
