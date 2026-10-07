// bdc 0x08a2c8b0 GameQuestCamLookFactoryBuild
#include "bdc.h"

/* Only virtual (vtable `0x08af6ed0` entry 1) of the small look-spring factory that
   `GameQuestCamCtrlStartPathMode` creates (`{vtable, ctrl, ctrl->nodeCursor, &ctrl->look}`):
   `args[0]` points to a pair of path nodes; when both have type 1 (`+0x5c`), destroys the current
   look spring (virtual entry 1, flags 3) and allocates a rail spring (0xa0 bytes, low heap,
   `GameQuestCamRailSpringCtor`) into the look slot (NULL when the allocation fails); otherwise,
   when the current look spring's `flag80` is set, calls `GameQuestCamCtrlCreateLookSpring`. */

typedef struct GameQuestCamLookFactory {
  const VtblEntry *vtbl;         /* +0x00 0x08af6ed0 */
  GameQuestCamCtrl *ctrl;        /* +0x04 */
  void *followed;                /* +0x08 ctrl->nodeCursor */
  GameQuestCamSpring **lookSlot; /* +0x0c &ctrl->look */
} GameQuestCamLookFactory;

typedef struct CamRailSpringDesc {
  GameQuestCamSpringDesc base;
  ScePspFVector4 point;
  void *pathSet;
} CamRailSpringDesc;

void GameQuestCamLookFactoryBuild(void *factory, void **args)

{
  GameQuestCamLookFactory *self = (GameQuestCamLookFactory *)factory;
  GameQuestPathNode **nodes = (GameQuestPathNode **)args[0];
  CamRailSpringDesc desc;
  GameQuestCamSpring *old;
  void *spring;
  void *result;
  bool fromLow;

  if ((nodes[0]->type != 1) || (((GameQuestPathNode **)args[0])[1]->type != 1)) {
    if (((GameQuestCamLookSpring *)*self->lookSlot)->flag80 != 0) {
      GameQuestCamCtrlCreateLookSpring(self->ctrl);
    }
    return;
  }
  desc.base.ctrl = NULL;
  desc.base.start = g_gameQuestCamCtrlAxisConsts.zero;
  desc.base.goal = g_gameQuestCamCtrlAxisConsts.zero;
  desc.point = (ScePspFVector4){0.0f, 0.0f, 0.0f, 0.0f}; /* bank C720, overwritten below */
  desc.pathSet = args[0];
  desc.base.ctrl = self->ctrl;
  desc.base.goal = (*self->lookSlot)->goal;
  desc.point = self->ctrl->lookAt;
  desc.base.start = (*self->lookSlot)->vel;
  desc.base.followed = self->followed;
  if (*self->lookSlot != NULL) {
    if (*self->lookSlot != NULL) {
      old = *self->lookSlot;
      if (old != NULL) {
        ((void (*)(void *, int))old->vtbl[1].fn)((u8 *)old + old->vtbl[1].delta, 3);
      }
      *self->lookSlot = NULL;
    }
  }
  result = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  spring = MemAlloc(0xa0, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (spring != NULL) {
    GameQuestCamRailSpringCtor(spring, &desc);
    result = spring;
  }
  *self->lookSlot = (GameQuestCamSpring *)result;
  return;
}
