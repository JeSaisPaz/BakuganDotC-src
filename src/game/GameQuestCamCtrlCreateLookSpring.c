// bdc 0x088f99dc GameQuestCamCtrlCreateLookSpring
#include "bdc.h"

/* Replaces the controller's look spring (`look`, +0x3c) with a fresh look-at spring
   (`GameQuestCamLookSpringCtor`). The descriptor gets `ctrl = self`, `followed = nodeCursor`,
   start/goal from the old spring's `vel`/`goal` (or the `zero` constant of
   `g_gameQuestCamCtrlAxisConsts` when there is none) and the look-at point `self->lookAt`. The old
   spring is destroyed through its vtable entry 1 (flag 3 = delete). The 0x90-byte allocation is
   made from the low end of the heap (`MemSetAllocFromLow`) under `MemLock`; on allocation
   failure `look` is left NULL. */

typedef struct CamLookSpringDesc {
  GameQuestCamSpringDesc spring;
  ScePspFVector4 look;
} __attribute__((aligned(16))) CamLookSpringDesc;

void GameQuestCamCtrlCreateLookSpring(GameQuestCamCtrl *self)
{
  CamLookSpringDesc desc __attribute__((aligned(16)));
  GameQuestCamSpring *old;
  GameQuestCamLookSpring *spring;
  bool fromLow;

  desc.spring.ctrl = NULL;
  desc.spring.start = g_gameQuestCamCtrlAxisConsts.zero;
  desc.spring.goal = g_gameQuestCamCtrlAxisConsts.zero;
  /* The listing also stores the live-in VFPU column C720 into desc.look here; it is overwritten
     below before any use. */
  desc.spring.ctrl = self;
  if (self->look != NULL) {
    desc.spring.goal = self->look->goal;
    desc.spring.start = self->look->vel;
    if (self->look != NULL) {
      old = self->look;
      if (old != NULL) {
        ((void (*)(void *, int))old->vtbl[1].fn)((u8 *)old + old->vtbl[1].delta, 3);
      }
      self->look = NULL;
    }
  }
  desc.look = self->lookAt;
  desc.spring.followed = self->nodeCursor;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  spring = MemAlloc(sizeof(GameQuestCamLookSpring), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (spring != NULL) {
    GameQuestCamLookSpringCtor(spring, &desc);
  }
  self->look = (GameQuestCamSpring *)spring;
}
