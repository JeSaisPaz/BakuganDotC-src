// bdc 0x088f02dc GameEvent470AddActorTween
#include "bdc.h"

/* Queues an event-actor tween (0xc bytes, `GameEventActorTweenCtor`; kinds 0 move, 1 rotate) of `frames`
   frames for actor record `index` to the event's action list; does nothing when `index` is not below the
   placed character count of `g_gameFieldCharSet`. In skip mode (`flags` bit0) applies the end state at
   once (`GameEventActorRecordFinish`) instead. A failed allocation still adds a NULL action. */

void GameEvent470AddActorTween(GameEvent470 *self, u16 frames, u8 kind, u8 index)
{
  bool fromLow;
  GameEventActorTween *mem;
  GameEventActorTween *action;
  GameEventActionList *list;

  if (index >= g_gameFieldCharSet->placedCount) {
    return;
  }
  if ((self->base.flags & 1) != 0) {
    GameEventActorRecordFinish(&self->actors[index], kind, 1);
    return;
  }
  list = self->base.actions;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0xc, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  action = NULL;
  if (mem != NULL) {
    GameEventActorTweenCtor(mem, &self->actors[index], (u8)frames, kind, index);
    action = mem;
  }
  GameEventActionListAdd(list, &action->base);
}
