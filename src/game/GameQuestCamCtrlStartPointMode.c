// bdc 0x088f9c9c GameQuestCamCtrlStartPointMode
#include "bdc.h"

/* Switches the controller to a type-2 camera entry `entry` with the transformed point `pos`
   (`GameQuestCamCtrlSwitchMode`): first replaces the look spring
   (`GameQuestCamCtrlCreateLookSpring`), then replaces the mode slot (`mode`, +0x38) with a fresh
   point camera mode (`GameQuestCamPointModeCtor`). The descriptor gets `ctrl = self`,
   `followed = nodeCursor`, the look slot `&self->look`, `entry`, the point `pos` (+0x60), and
   start/goal from the old mode's `vel`/`goal` plus the point returned by the old mode's vtable
   entry 2 (without an old mode: start/goal are the `zero` constant of
   `g_gameQuestCamCtrlAxisConsts` and the point is (0, 0, 0, 0), the VFPU bank constant C720). The
   old mode is destroyed through its vtable entry 1 (flag 3 = delete). The 0x150-byte allocation is
   made from the low end of the heap (`MemSetAllocFromLow`) under `MemLock`; on allocation
   failure `mode` is left NULL. */

typedef struct CamPointModeDesc {
  GameQuestCamSpringDesc spring; /* +0x00 */
  ScePspFVector4 point;          /* +0x30 */
  GameQuestCamSpring **lookSlot; /* +0x40 &ctrl->look */
  u8 _unk44[0xc];
  void *entry;                   /* +0x50 */
  u8 _unk54[0xc];
  ScePspFVector4 pos;            /* +0x60 */
} CamPointModeDesc;

void GameQuestCamCtrlStartPointMode(GameQuestCamCtrl *self, void *entry, const float *pos)
{
  CamPointModeDesc desc;
  GameQuestCamSpring *old;
  GameQuestCamPointMode *mode;
  GameQuestCamPointMode *result;
  const ScePspFVector4 *point;
  bool fromLow;

  desc.spring.ctrl = NULL;
  desc.spring.start = g_gameQuestCamCtrlAxisConsts.zero;
  desc.spring.goal = g_gameQuestCamCtrlAxisConsts.zero;
  /* Bank constant C720 = (0, 0, 0, 0) is the default point and pos. */
  desc.point.x = 0.0f;
  desc.point.y = 0.0f;
  desc.point.z = 0.0f;
  desc.point.w = 0.0f;
  desc.pos.x = 0.0f;
  desc.pos.y = 0.0f;
  desc.pos.z = 0.0f;
  desc.pos.w = 0.0f;
  GameQuestCamCtrlCreateLookSpring(self);
  if (self->mode != NULL) {
    desc.spring.start = self->mode->vel;
    desc.spring.goal = self->mode->goal;
    old = self->mode;
    point = ((const ScePspFVector4 *(*)(void *))old->vtbl[2].fn)((u8 *)old + old->vtbl[2].delta);
    desc.point = *point;
    if (self->mode != NULL) {
      old = self->mode;
      if (old != NULL) {
        ((void (*)(void *, int))old->vtbl[1].fn)((u8 *)old + old->vtbl[1].delta, 3);
      }
      self->mode = NULL;
    }
  }
  desc.entry = entry;
  desc.spring.ctrl = self;
  desc.lookSlot = &self->look;
  desc.spring.followed = self->nodeCursor;
  desc.pos = *(const ScePspFVector4 *)pos;
  result = NULL;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mode = MemAlloc(sizeof(GameQuestCamPointMode), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mode != NULL) {
    GameQuestCamPointModeCtor(mode, &desc);
    result = mode;
  }
  self->mode = (GameQuestCamSpring *)result;
}
