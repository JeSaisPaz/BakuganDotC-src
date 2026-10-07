// bdc 0x088ebf38 GameEventAddFade
#include "bdc.h"

/* Queues a screen-fade action (0xc bytes from the low heap, `GameEventFadeActionCtor` with skip 0,
   record `fades[index]`, duration `frames`, target `(u8)index`) on the action list `actions`
   (`GameEventActionListAdd`, NULL is passed if the allocation failed); in skip mode (`flags` bit0)
   instead jumps `fades[index]` to its end state (`GameEventFadeRecordFinish`). */

void GameEventAddFade(GameEvent *self, u8 frames, s32 index)
{
  bool fromLow;
  GameEventFadeAction *mem;
  GameEventFadeAction *action;
  GameEventActionList *list;

  if ((self->flags & 1) != 0) {
    GameEventFadeRecordFinish(&self->fades[index]);
    return;
  }
  list = self->actions;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(GameEventFadeAction), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  action = NULL;
  if (mem != NULL) {
    GameEventFadeActionCtor(mem, 0, &self->fades[index], frames, (u8)index);
    action = mem;
  }
  GameEventActionListAdd(list, &action->base);
}
