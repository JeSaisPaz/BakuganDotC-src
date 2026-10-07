// bdc 0x088f2230 GameEvent470ApplyAxisOffset
#include "bdc.h"

/* Override (vtable slot 13, `+0x6c`) of `GameEventApplyAxisOffset` in the task-470 event class
   (derived from the field event base `GameEventCtor`, vtable `0x08af425c`, constructor
   `GameEvent470Ctor`). `amount` is converted to 20.12 fixed point (`(s64)amount << 24` divided by
   `g_gameFixedScale`). Only in coordinate mode 4 (`coordMode`) does it act: axis 1 sets `vec[1]`;
   axes 0 and 2 add the offset to `vec[0]`/`vec[2]` along the heading `rot[1]` of the class's own
   event-actor record `actorIdx` (`actors`), turned by -0x4000 (axis 0) or -0x8000 (axis 2), with the
   fixed-point sin/cos `GameEvent470SinFixed`/`GameEvent470CosFixed` and rounding
   (`g_gameFixedHalf`). Other modes and axes leave `vec` untouched (no call to the base). */

void GameEvent470ApplyAxisOffset(GameEvent470 *self, s32 axis, s16 amount, s32 *vec, u8 actorIdx)
{
  s32 offset;
  s32 angle;
  s64 half;
  s64 prod;

  offset = (s32)__divdi3((s64)amount << 24, g_gameFixedScale);
  if (self->base.coordMode != 4) {
    return;
  }
  switch (axis) {
  case 0:
    angle = (u16)(self->actors[actorIdx].rot[1] - 0x4000);
    break;
  case 1:
    vec[1] = offset;
    return;
  case 2:
    angle = (u16)(self->actors[actorIdx].rot[1] - 0x8000);
    break;
  default:
    return;
  }
  prod = __muldi3(GameEvent470SinFixed(angle), offset);
  half = g_gameFixedHalf;
  vec[0] += (s32)((prod + half) >> 12);
  vec[2] += (s32)((__muldi3(GameEvent470CosFixed(angle), offset) + half) >> 12);
}
