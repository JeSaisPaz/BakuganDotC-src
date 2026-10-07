// bdc 0x088ec594 GameEventAddCamTween
#include "bdc.h"

/* Queues a camera tween action (0x10 bytes, `GameEventCamTweenCtor` on the key record `ev+0x2c`) of `frames`
   frames for `kind` (0 eye, 1 target, 2 field of view, see `GameEventCamTweenBegin`) to the
   action list; in skip mode jumps straight to the end key (`GameEventCamKeysFinish`). */

void GameEventAddCamTween(GameEvent *self, u16 frames, u8 kind)

{
  bool fromLow;
  GameEventCamTween *self_00;
  GameEventCamTween *action;
  GameEventActionList *self_01;
  
  if ((self->flags & 1) == 0) {
    self_01 = self->actions;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    self_00 = MemAlloc(sizeof(GameEventCamTween),(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    action = (GameEventCamTween *)0x0;
    if (self_00 != (GameEventCamTween *)0x0) {
      GameEventCamTweenCtor(self_00,self->camKeys,frames,kind);
      action = self_00;
    }
    GameEventActionListAdd(self_01,&action->base);
    return;
  }
  GameEventCamKeysFinish(self->camKeys,(uint)kind,'\x01');
  return;
}

