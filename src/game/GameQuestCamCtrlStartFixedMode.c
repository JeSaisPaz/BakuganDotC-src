// bdc 0x088f9b18 GameQuestCamCtrlStartFixedMode
#include "bdc.h"

/* Switches the controller to a type-1 camera entry (`GameQuestCamCtrlSwitchMode`): replaces the
   mode slot (`mode`, +0x38) with a fresh fixed-camera mode (`GameQuestCamFixedModeCtor`). The
   descriptor gets `ctrl = self`, the path set `entry`, start/goal from the old mode's `vel`/`goal`
   and the point returned by the old mode's vtable entry 2 (without an old mode: start/goal are the
   `zero` constant of `g_gameQuestCamCtrlAxisConsts` and the point is the zero vector, the VFPU
   bank constant C720). The old mode is destroyed through its vtable entry 1 (flag 3 = delete). The 0x90-byte
   allocation is made from the low end of the heap (`MemSetAllocFromLow`) under `MemLock`; on
   allocation failure `mode` is left NULL. Then, when `entry+0x5c` is 1, an eye spring is created
   (`GameQuestCamCtrlCreateEyeSpring`), otherwise a look-at spring
   (`GameQuestCamCtrlCreateLookSpring`). */

typedef struct CamFixedModeDesc {
  GameQuestCamSpringDesc spring; /* +0x00 */
  ScePspFVector4 point;          /* +0x30 */
  void *entry;                   /* +0x40 */
} __attribute__((aligned(16))) CamFixedModeDesc;

/* Partial view of the camera table entry. */
typedef struct CamFixedEntry {
  u8 _unk00[0x5c];
  s32 type; /* +0x5c 1 = eye spring */
} CamFixedEntry;

void GameQuestCamCtrlStartFixedMode(GameQuestCamCtrl *self, void *entry)
{
  CamFixedModeDesc desc __attribute__((aligned(16)));
  GameQuestCamSpring *old;
  GameQuestCamFixedMode *mode;
  const ScePspFVector4 *point;
  bool fromLow;

  desc.spring.ctrl = NULL;
  desc.spring.start = g_gameQuestCamCtrlAxisConsts.zero;
  desc.spring.goal = g_gameQuestCamCtrlAxisConsts.zero;
  desc.point.x = 0.0f; /* bank constant C720 = (0, 0, 0, 0) */
  desc.point.y = 0.0f;
  desc.point.z = 0.0f;
  desc.point.w = 0.0f;
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
  desc.spring.ctrl = self;
  desc.entry = entry;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mode = MemAlloc(sizeof(GameQuestCamFixedMode), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mode != NULL) {
    GameQuestCamFixedModeCtor(mode, &desc);
  }
  self->mode = (GameQuestCamSpring *)mode;
  if (((CamFixedEntry *)entry)->type == 1) {
    GameQuestCamCtrlCreateEyeSpring(self, entry);
  } else {
    GameQuestCamCtrlCreateLookSpring(self);
  }
}
