// bdc 0x0880f710 ScriptOpJumpIfUnitStat
#include "bdc.h"

/* Conditional jump on a statistic of the `idx`-th battle unit (`BtlGetNthCrystal`,
   `BtlBakuganListFind`). Operands: u16 `idx`, u16 `stat`, u16 `cmp`, float `value`, u16 `target`.
   `stat` 0: byte `cpuEntryDone` (`+0xa3e`); 1: byte `mode2Done` (`+0xa3f`); 2: HP ratio
   `BtlCombatGetHpRatio` of the unit's combat state (value scaled by 0.01); 3: byte `fadeDone` (`+0xa79`);
   other stats read as 0. `cmp` 1 `<=`, 2 `==`, 3 `!=`, 4 `>`, 5 `>=`, 0 `<` (stat OP value).
   Jumps (returns 3) when the comparison holds or when the unit or the player is missing (the
   comparison is then made against 0); a `cmp` of 6 or more only jumps on a missing unit/player. */

int ScriptOpJumpIfUnitStat(Script *script)
{
  int idx;
  int stat;
  u32 cmp;
  float value;
  u32 target;
  ActorCrystal *unit;
  void *player;
  int jump;
  float cur;

  idx = (short)ScriptReadU16(script);
  stat = (int)ScriptReadU16(script);
  cmp = ScriptReadU16(script);
  value = ScriptReadFloat(script);
  target = ScriptReadU16(script);
  unit = (ActorCrystal *)BtlBakuganListFind((BtlBakugan *)BtlGetNthCrystal(idx));
  jump = 0;
  if (unit == NULL) {
    jump = 1;
  }
  player = BtlGetPlayerBakugan();
  if (player == NULL) {
    jump = 1;
  }
  cur = 0.0f;
  if (player != NULL && unit != NULL) {
    switch (stat) {
    case 0:
      cur = (float)(int)unit->cpuEntryDone;
      break;
    case 1:
      cur = (float)(int)unit->mode2Done;
      break;
    case 2:
      cur = BtlCombatGetHpRatio(&unit->base.combat);
      value = value * 0.01f;
      break;
    case 3:
      cur = (float)(int)unit->fadeDone;
      break;
    default:
      break;
    }
  }
  if (cmp < 6) {
    switch (cmp) {
    case 1:
      if (cur <= value) {
        jump = 1;
      }
      break;
    case 2:
      if (cur == value) {
        jump = 1;
      }
      break;
    case 3:
      if (!(cur == value)) {
        jump = 1;
      }
      break;
    case 4:
      if (!(cur <= value)) {
        jump = 1;
      }
      break;
    case 5:
      if (!(cur < value)) {
        jump = 1;
      }
      break;
    default:
      if (cur < value) {
        jump = 1;
      }
      break;
    }
  }
  if (jump) {
    script->curTrack->pc = (u16)target;
    return 3;
  }
  return 0;
}
