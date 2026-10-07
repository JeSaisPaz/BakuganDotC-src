// bdc 0x088e5f4c ActorNpcFreeze
#include "bdc.h"

/* Vtable slot 16 of the field NPC/guard classes (base `ActorNpcCtor`): saves the AI state in
   `+0x42c` (unless already 10), stores the placement (`ActorSavePlacement`), enters state 10 with
   the idle motion, resets the placed motion index and remembers which head effect was showing (0x29
   -> 1, 0x2a -> 2 in `+0x428`, `GfxEffectFindAttached`) before clearing it. Undone by
   `ActorNpcResume`. */

void ActorNpcFreeze(ActorNpc *self)

{
  void *found;
  float *attach;
  
  if (self->aiState != 10) {
    self->savedState = self->aiState;
  }
  ActorSavePlacement(&self->base);
  self->aiState = 10;
  ActorPlayMotion(0.2f,self,0,'\x01','\0');
  attach = (self->base).mtx + 0xc;
  ((ActorNpcPlacement *)self->base.placement)->frozenFlag = 0;
  if (self->head != (void *)0x0) {
    attach = self->headPos;
  }
  if (attach != (float *)0x0) {
    self->freezeKind = 0;
    found = GfxEffectFindAttached(g_worldEffectMgr,0x29,attach);
    if (found == (void *)0x0) {
      found = GfxEffectFindAttached(g_worldEffectMgr,0x2a,attach);
      if (found != (void *)0x0) {
        self->freezeKind = 2;
      }
    }
    else {
      self->freezeKind = 1;
    }
    if (self->freezeKind != 0) {
      ActorNpcShowHeadEffect(self,-1,'\x01','\x01');
    }
  }
  return;
}

