// bdc 0x08887310 BtlCombatGetAttackFactor
#include "bdc.h"

/* Effective attack multiplier of a `BtlCombatState`: `BtlCombatGetAttackLevelFactor`, × 2.0
   while status 0 (attack up) is active, × 0.75 while status 1 (attack down) is active, then
   clamped: below 0 gives 0, at most 100 is returned as is, anything else (above 100 or NaN)
   gives 100. */
float BtlCombatGetAttackFactor(BtlCombatState *combat)
{
  float factor = BtlCombatGetAttackLevelFactor(combat);

  if (combat->status[0].active != 0) {
    factor = factor * 2.0f;
  }
  if (combat->status[1].active != 0) {
    factor = factor * 0.75f;
  }
  if (factor < 0.0f) {
    return 0.0f;
  }
  if (factor <= 100.0f) {
    return factor;
  }
  return 100.0f;
}
