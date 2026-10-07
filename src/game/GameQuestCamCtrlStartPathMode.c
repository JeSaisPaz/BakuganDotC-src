// bdc 0x088f9e3c GameQuestCamCtrlStartPathMode
#include "bdc.h"

/* Switches the controller to the default path-following mode (`GameQuestCamCtrlSwitchMode`).
   First allocates (0x10 bytes, low heap, under `MemLock`) a small look-spring factory
   (`g_gameQuestCamLookFactoryVtbl`, `{vtbl, ctrl, nodeCursor, &look}`; its only virtual is
   `GameQuestCamLookFactoryBuild`). Then replaces the mode slot (`mode`, +0x38) with a fresh path
   camera mode (`GameQuestCamPathModeCtor`, 0x150 bytes, low heap): the descriptor gets
   `ctrl = self`, `followed = nodeCursor`, start/goal from the old mode's `vel`/`goal` and the point
   returned by the old mode's vtable entry 2 (without an old mode: start/goal are the `zero`
   constant of `g_gameQuestCamCtrlAxisConsts` and the point is the VFPU bank zero C720),
   `&look` and the factory. The old mode is destroyed through its vtable entry 1 (flag 3 = delete);
   on allocation failure `mode` is left NULL. Finally a look-at spring is created
   (`GameQuestCamCtrlCreateLookSpring`). */

/* Same object as the one GameQuestCamLookFactoryBuild works on. */
typedef struct CamPathLookFactory {
  const VtblEntry *vtbl;         /* +0x00 g_gameQuestCamLookFactoryVtbl */
  GameQuestCamCtrl *ctrl;        /* +0x04 */
  void *followed;                /* +0x08 ctrl->nodeCursor */
  GameQuestCamSpring **lookSlot; /* +0x0c &ctrl->look */
} CamPathLookFactory;

typedef struct CamPathModeDesc {
  GameQuestCamSpringDesc spring; /* +0x00 */
  ScePspFVector4 point;          /* +0x30 */
  GameQuestCamSpring **lookSlot; /* +0x40 &ctrl->look */
  u8 _unk44[0xc];
  CamPathLookFactory *factory;   /* +0x50 (GameQuestCamPathModeDesc.pathSet) */
} CamPathModeDesc;

void GameQuestCamCtrlStartPathMode(GameQuestCamCtrl *self)
{
  CamPathModeDesc desc;
  CamPathLookFactory *factory;
  CamPathLookFactory *alloc;
  GameQuestCamSpring *old;
  GameQuestCamPathMode *mode;
  const ScePspFVector4 *point;
  bool fromLow;

  factory = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = MemAlloc(sizeof(CamPathLookFactory), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (alloc != NULL) {
    alloc->vtbl = g_gameQuestCamLookFactoryVtbl;
    alloc->lookSlot = &self->look;
    factory = alloc;
  }
  /* no NULL check here: an allocation failure writes through NULL */
  factory->ctrl = self;
  factory->followed = self->nodeCursor;

  desc.spring.ctrl = NULL;
  desc.spring.start = g_gameQuestCamCtrlAxisConsts.zero;
  desc.spring.goal = g_gameQuestCamCtrlAxisConsts.zero;
  /* Bank constant C720 = (0, 0, 0, 0). */
  desc.point.x = 0.0f;
  desc.point.y = 0.0f;
  desc.point.z = 0.0f;
  desc.point.w = 0.0f;
  desc.spring.ctrl = self;
  desc.lookSlot = &self->look;
  desc.factory = factory;
  desc.spring.followed = self->nodeCursor;
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

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mode = MemAlloc(sizeof(GameQuestCamPathMode), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mode != NULL) {
    GameQuestCamPathModeCtor(mode, &desc);
  }
  self->mode = (GameQuestCamSpring *)mode;
  GameQuestCamCtrlCreateLookSpring(self);
}
