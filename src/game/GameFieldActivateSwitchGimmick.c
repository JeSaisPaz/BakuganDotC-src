// bdc 0x088c2000 GameFieldActivateSwitchGimmick
#include "bdc.h"

/* Finds the placed gimmick of type 0xbc7 or 0xbd1 (`+0x142`) in the gimmick list `+0x658` whose
   record parameter (`GameGimmickGetRecordParam`) equals `id` and calls its virtual `+0x84` with 0
   (activate). */

void GameFieldActivateSwitchGimmick(CoreTask *task, s16 id)
{
  GameGimmick *gimmick;
  const VtblEntry *e;

  for (gimmick = ((GameFieldTask *)task)->gimmicks; gimmick != NULL;
       gimmick = (GameGimmick *)gimmick->base.base.next) {
    if (gimmick->typeId != 0xbc7 && gimmick->typeId != 0xbd1) {
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
