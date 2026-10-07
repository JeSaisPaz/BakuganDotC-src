// bdc 0x088876d0 BtlCalcHazardDamage
#include "bdc.h"

/* Damage of a hit that does not come from a unit's own attack tables (attack ids below 0x23 or an
   attacker of another class): power 20 by default, 50 for ids 0x1b/0x1c, 0 for 0x1d/0x1e, 30 for
   0x1f (these five are stage hazards: affinity kind 3 and attacker attribute `id - 0x1a` = 1..5),
   150 for 0xa7, 10 for 0xaa/0xab. Every other id uses kind 0 and attribute 6 (none). Returns
   power × `BtlGetAttributeMultiplier``(kind, attr, special, targetAttr)`. */
float BtlCalcHazardDamage(s32 attackId, s32 special, s32 targetAttr)
{
  float power = 20.0f;
  s32 kind = 0;
  s32 attackerAttr = 6;

  if (attackId < 0x20) {
    if (attackId > 0x1a) {
      kind = 3;
      if (attackId < 0x1d) {
        power = 50.0f;
      } else if (attackId < 0x1f) {
        power = 0.0f;
      } else {
        power = 30.0f;
      }
    }
  } else if (attackId < 0xaa) {
    if (attackId == 0xa7) {
      power = 150.0f;
    }
  } else if (attackId < 0xac) {
    power = 10.0f;
  }
  if (kind == 3) {
    attackerAttr = attackId - 0x1a;
  }
  return power * BtlGetAttributeMultiplier(kind, attackerAttr, special, targetAttr);
}
