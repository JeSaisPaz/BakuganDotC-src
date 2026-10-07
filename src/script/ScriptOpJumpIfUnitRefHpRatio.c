// bdc 0x0880f00c ScriptOpJumpIfUnitRefHpRatio
#include "bdc.h"

/* Conditional jump on the HP ratio of a battle unit given by ref. Operands: ref (unit, validated
   with `BtlBakuganListFind`), u16 `cmp`, float `percent` (scaled by 0.01), u16 `target`. The
   ratio is `BtlCombatGetHpRatio` of the unit's `combat`. `cmp`: 0 `ratio < v`, 1 `ratio <= v`,
   2 `==`, 3 `!=`, 4 `!(ratio <= v)`, 5 `!(ratio < v)`; 6 and up never. Jumps (track pc =
   `target`, returns 3) when the comparison holds or when the unit is missing; returns 0
   otherwise. */

int ScriptOpJumpIfUnitRefHpRatio(Script *script)
{
  u32 *ref;
  BtlBakugan *unit;
  u32 cmp;
  float value;
  u32 target;
  int jump;

  ref = ScriptReadRef(script, 2);
  unit = (BtlBakugan *)(uintptr_t)*ref;
  cmp = ScriptReadU16(script);
  value = ScriptReadFloat(script);
  target = ScriptReadU16(script);
  unit = (BtlBakugan *)BtlBakuganListFind(unit);
  jump = unit == NULL;
  value = value * 0.01f;
  if (unit != NULL && cmp < 6) {
    BtlCombatState *combat = &unit->combat;
    switch (cmp) {
    case 1:
      if (BtlCombatGetHpRatio(combat) <= value)
        jump = 1;
      break;
    case 2:
      if (BtlCombatGetHpRatio(combat) == value)
        jump = 1;
      break;
    case 3:
      if (!(BtlCombatGetHpRatio(combat) == value))
        jump = 1;
      break;
    case 4:
      if (!(BtlCombatGetHpRatio(combat) <= value))
        jump = 1;
      break;
    case 5:
      if (!(BtlCombatGetHpRatio(combat) < value))
        jump = 1;
      break;
    default:
      if (BtlCombatGetHpRatio(combat) < value)
        jump = 1;
      break;
    }
  }
  if (jump) {
    script->curTrack->pc = (u16)target;
    return 3;
  }
  return 0;
}
