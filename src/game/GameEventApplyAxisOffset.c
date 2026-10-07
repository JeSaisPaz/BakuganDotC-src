// bdc 0x088ec6b4 GameEventApplyAxisOffset
#include "bdc.h"

/* Moves the fixed-point vector `vec` (0 = x, 1 = y, 2 = z) by `amount` along axis `axis`, in the
   event's coordinate mode `coordMode`. `amount` is first converted to 20.12 fixed point
   (`((s64)amount << 24) / g_gameEventFixedScale` with `__divdi3`). Mode 0 (world axes) stores the
   offset into `vec[axis]`. Mode 1 (camera-relative) stores it into `vec[1]` for axis 1; for axes 0 and
   2 it treats `g_gfxActiveCamera->up[1]` as an angle in radians, wraps it into [0, 2pi), turns it into
   a 16-bit angle `pi/2 - a` (+0x4000 for axis 0) and adds `offset * sin` to `vec[0]` and
   `offset * cos` to `vec[2]` (`GameEventSinFixed`/`GameEventCosFixed`, 64-bit products via
   `__muldi3`, rounded with `g_gameEventFixedHalf`, >> 12). Mode 2 (actor-relative) does the same
   with the heading `rot[1]` of prop record `actorIdx` (`props`), turned by -0x4000 (axis 0) or
   -0x8000 (axis 2). Mode 3 does nothing; modes 4 and up call the class's override through vtable
   slot 13 (`+0x6c`, e.g. `GameEvent470ApplyAxisOffset`). Other axes leave `vec` untouched. */

#define AXIS_PI 3.1415927f
#define AXIS_TWO_PI 6.2831855f

static float GameEventAxisWrapPi(float a)
{
  if (!(a <= AXIS_PI)) {
    a = a - AXIS_TWO_PI;
  } else if (a <= -AXIS_PI) {
    a = a + AXIS_TWO_PI;
  }
  return a;
}

/* Camera angle (radians) to the 16-bit angle of the event sin/cos helpers, without the axis turn. */
static s16 GameEventAxisCamAngle(void)
{
  float a;

  a = GameEventAxisWrapPi(g_gfxActiveCamera->up[1]);
  if (a < 0.0f) {
    a = a + AXIS_TWO_PI;
  }
  a = GameEventAxisWrapPi(-a + 1.5707964f);
  return (s16)(s32)(a * 65535.0f * 0.15915494f);
}

void GameEventApplyAxisOffset(GameEvent *self, s32 axis, s16 amount, s32 *vec, u8 actorIdx)
{
  s32 offset;
  s32 angle;
  s64 half;
  s64 prod;
  const VtblEntry *entry;

  offset = (s32)__divdi3((s64)(amount << 12) << 12, g_gameEventFixedScale);
  switch (self->coordMode) {
  case 0:
    if (axis == 0) {
      vec[0] = offset;
    } else if (axis == 1) {
      vec[1] = offset;
    } else if (axis == 2) {
      vec[2] = offset;
    }
    return;
  case 1:
    if (axis == 0) {
      half = g_gameEventFixedHalf;
      angle = (u16)(GameEventAxisCamAngle() + 0x4000);
    } else if (axis == 1) {
      vec[1] = offset;
      return;
    } else if (axis == 2) {
      half = g_gameEventFixedHalf;
      angle = (u16)GameEventAxisCamAngle();
    } else {
      return;
    }
    vec[0] += (s32)((__muldi3(GameEventSinFixed(angle), offset) + half) >> 12);
    vec[2] += (s32)((__muldi3(GameEventCosFixed(angle), offset) + half) >> 12);
    return;
  case 2:
    if (axis == 0) {
      angle = (u16)(self->props[actorIdx].rot[1] - 0x4000);
    } else if (axis == 1) {
      vec[1] = offset;
      return;
    } else if (axis == 2) {
      angle = (u16)(self->props[actorIdx].rot[1] - 0x8000);
    } else {
      return;
    }
    prod = __muldi3(GameEventSinFixed(angle), offset);
    half = g_gameEventFixedHalf;
    vec[0] += (s32)((prod + half) >> 12);
    vec[2] += (s32)((__muldi3(GameEventCosFixed(angle), offset) + half) >> 12);
    return;
  case 3:
    return;
  default:
    entry = &((const VtblEntry *)self->base.vtable)[13];
    ((void (*)(void *, s32, s16, s32 *, u8))entry->fn)((u8 *)self + entry->delta, axis, amount, vec,
                                                         actorIdx);
    return;
  }
}
