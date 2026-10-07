// bdc 0x088c0c0c GameFieldResetGimmickStates
#include "bdc.h"

/* Walks the gimmick list `+0x658` of the field task (id 500, `GameFieldCtor`) and, for every
   placed gimmick (`+0x15e`) whose virtual `+0x7c` test passes, resets its state through
   `GameGimmickResetState``(gimmick, force)`. */

void GameFieldResetGimmickStates(CoreTask *task, bool force)

{
  GameGimmick *obj;
  const VtblEntry *entry;

  for (obj = ((GameFieldTask *)task)->gimmicks; obj != NULL; obj = (GameGimmick *)obj->base.base.next) {
    if (obj->active == 0) {
      continue;
    }
    entry = &((const VtblEntry *)obj->base.base.vtable)[15];
    if (((s32 (*)(void *))entry->fn)((u8 *)obj + entry->delta) != 0) {
      GameGimmickResetState((GameGimmickIrSensor *)obj, force);
    }
  }
}
