// bdc 0x088c20c4 GameFieldActivateGimmick
#include "bdc.h"

/* Finds the first placed gimmick of the list `+0x658` whose virtual `+0x5c` test passes and whose
   record parameter (`GameGimmickGetRecordParam`) equals `id`, and calls its virtual `+0x84` with
   0. */

void GameFieldActivateGimmick(CoreTask *task, s16 id)

{
  GameGimmick *gimmick;
  const VtblEntry *e;

  for (gimmick = ((GameFieldTask *)task)->gimmicks; gimmick != NULL;
       gimmick = (GameGimmick *)gimmick->base.base.next) {
    e = &((const VtblEntry *)gimmick->base.base.vtable)[11];
    if (((int (*)(void *))e->fn)((char *)gimmick + e->delta) == 0) {
      continue;
    }
    if (gimmick->active == 0) {
      continue;
    }
    if (GameGimmickGetRecordParam(gimmick) == id) {
      e = &((const VtblEntry *)gimmick->base.base.vtable)[16];
      ((void (*)(void *, int))e->fn)((char *)gimmick + e->delta, 0);
      return;
    }
  }
}
