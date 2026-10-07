// bdc 0x08811038 ScriptOpJumpIfStopWallIdle
#include "bdc.h"

/* Conditional jump on a stop wall's state. Operands: u16 `id`, u16 `mode`, u16 `cmp`, float `f`,
   u16 `target`. Walks `g_stopWallList` until the wall whose `id` matches or the tail (so with
   no match the last wall is used); takes `v = 1.0` when that wall's `shown` flag is clear, 0.0
   when set or when the list is empty/missing. A non-zero `mode` with a wall forces `cmp` to 10
   (never). Compares with `cmp`: 0 `v < f`, 1 `v <= f`, 2 `==`, 3 `!=`, 4 `!(v <= f)`,
   5 `!(v < f)`, 6 and up never; on success sets the track pc to `target` and returns 3,
   otherwise 0. */

int ScriptOpJumpIfStopWallIdle(Script *script)
{
  u32 id;
  u32 mode;
  u32 cmp;
  float f;
  u32 target;
  int jump;
  StopWall *wall;
  float v;

  id = ScriptReadU16(script);
  mode = ScriptReadU16(script);
  cmp = ScriptReadU16(script);
  f = ScriptReadFloat(script);
  target = ScriptReadU16(script);
  jump = 0;
  wall = NULL;
  if (g_stopWallList != NULL && (wall = (StopWall *)g_stopWallList->head) != NULL) {
    while ((u32)wall->id != id && wall->base.next != NULL)
      wall = (StopWall *)wall->base.next;
  }
  v = 0.0f;
  if (wall != NULL) {
    if (mode != 0)
      cmp = 10;
    else
      v = (float)(int)(wall->shown == 0);
  }
  if (cmp < 6) {
    switch (cmp) {
    case 1:
      if (v <= f)
        jump = 1;
      break;
    case 2:
      if (v == f)
        jump = 1;
      break;
    case 3:
      if (!(v == f))
        jump = 1;
      break;
    case 4:
      if (!(v <= f))
        jump = 1;
      break;
    case 5:
      if (!(v < f))
        jump = 1;
      break;
    default:
      if (v < f)
        jump = 1;
      break;
    }
  }
  if (!jump)
    return 0;
  script->curTrack->pc = (u16)target;
  return 3;
}
