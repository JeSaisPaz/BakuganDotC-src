// bdc 0x088103c8 ScriptOpJumpIfRefDistanceToPlayer
#include "bdc.h"

/* Same test as `ScriptOpJumpIfDistanceToPlayer` but the other unit comes from a ref
   (`BtlBakuganListFind`) instead of an index. Operands: ref, u16 `cmp` (0 `<`, 1 `<=`, 2 `==`,
   3 `!=`, 4 `!(<=)`, 5 `!(<)`, i.e. `>`/`>=` but true for NaN), float `dist` (squared before
   comparison), u16 `target`. The squared distance is that of the xyz parts of player pos minus unit
   pos. Jumps (returns 3) when
   the comparison holds or when either unit is missing; a `cmp` >= 6 with both units present never
   jumps (returns 0). */

int ScriptOpJumpIfRefDistanceToPlayer(Script *script)
{
  u32 *ref;
  BtlBakugan *self;
  u32 cmp;
  float dist;
  u32 target;
  BtlBakugan *unit;
  BtlBakugan *player;
  int jump;
  float d2;

  ref = ScriptReadRef(script, 2);
  self = (BtlBakugan *)PspPtrOrNull(*ref);
  cmp = ScriptReadU16(script);
  dist = ScriptReadFloat(script);
  target = ScriptReadU16(script);
  jump = 0;
  unit = (BtlBakugan *)BtlBakuganListFind(self);
  if (unit == NULL) {
    jump = 1;
  }
  player = (BtlBakugan *)BtlGetPlayerBakugan();
  if (player == NULL) {
    jump = 1;
  }
  if (player != NULL && unit != NULL && cmp < 6) {
    dist = dist * dist;
    {
      const float *a = player->base.pos;
      const float *b = unit->base.pos;
      float dx = a[0] - b[0];
      float dy = a[1] - b[1];
      float dz = a[2] - b[2];
      d2 = dx * dx + dy * dy + dz * dz;
    }
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
