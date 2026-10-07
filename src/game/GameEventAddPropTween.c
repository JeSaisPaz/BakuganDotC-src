// bdc 0x088ed5b0 GameEventAddPropTween
#include "bdc.h"

/* Queues a prop tween action (0xc bytes, `GameEventPropTweenCtor`; kinds 0 move, 1 rotate, 2 scale, 3 motion
   wait) of `frames` frames for prop slot `slot` to the action list; in skip mode (`flags` bit0) every kind
   except 3 instead applies the end state at once (`GameEventPropRecordFinish`) and queues nothing. */

void GameEventAddPropTween(GameEvent *self, u16 frames, u8 kind, u8 slot)
{
  bool fromLow;
  GameEventPropTween *tween;
  GameEventPropTween *action;
  GameEventActionList *list;

  if ((self->flags & 1) != 0 && kind != 3) {
    GameEventPropRecordFinish(&self->props[slot], kind, 1);
    return;
  }
  list = self->actions;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  tween = MemAlloc(sizeof(GameEventPropTween), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  action = NULL;
  if (tween != NULL) {
    GameEventPropTweenCtor(tween, &self->props[slot], (u8)frames, kind, slot);
    action = tween;
  }
  GameEventActionListAdd(list, &action->base);
}
