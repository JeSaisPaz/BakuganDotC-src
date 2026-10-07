// bdc 0x0880f468 ScriptOpJumpIfDistanceToPlayer
#include "bdc.h"

/* Conditional jump on the squared distance between the player's battle unit
   (`BtlGetPlayerBakugan`) and the `idx`-th unit (`BtlGetNthCrystal`, validated with
   `BtlBakuganListFind`). Operands: u16 `idx`, u16 `cmp` (1 `<=`, 2 `==`, 3 `!=`, 4 `>`, 5 `>=`,
   0 `<`), float `dist` (squared before comparison), u16 `target`. Positions are the model `pos`
   (xyz of player.pos - unit.pos, dotted with itself). Jumps (returns 3) when the comparison holds or when either
   unit is missing; a `cmp` of 6 or more only jumps when a unit is missing. */

int ScriptOpJumpIfDistanceToPlayer(Script *script)
{
  int idx;
  u32 cmp;
  float dist;
  u32 target;
  BtlBakugan *unit;
  BtlBakugan *player;
  int jump;
  float dx;
  float dy;
  float dz;
  float d2;

  idx = (short)ScriptReadU16(script);
  cmp = ScriptReadU16(script);
  dist = ScriptReadFloat(script);
  target = ScriptReadU16(script);
  unit = (BtlBakugan *)BtlBakuganListFind((BtlBakugan *)BtlGetNthCrystal(idx));
  jump = 0;
  if (unit == NULL) {
    jump = 1;
  }
  player = (BtlBakugan *)BtlGetPlayerBakugan();
  if (player == NULL) {
    jump = 1;
  }
  if (player != NULL && unit != NULL && cmp < 6) {
    dist = dist * dist;
    dx = player->base.pos[0] - unit->base.pos[0];
    dy = player->base.pos[1] - unit->base.pos[1];
    dz = player->base.pos[2] - unit->base.pos[2];
    d2 = dx * dx + dy * dy + dz * dz;
    switch (cmp) {
    case 1:
      if (d2 <= dist) {
        jump = 1;
      }
      break;
    case 2:
      if (d2 == dist) {
        jump = 1;
      }
      break;
    case 3:
      if (!(d2 == dist)) {
        jump = 1;
      }
      break;
    case 4:
      if (!(d2 <= dist)) {
        jump = 1;
      }
      break;
    case 5:
      if (!(d2 < dist)) {
        jump = 1;
      }
      break;
    default:
      if (d2 < dist) {
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
