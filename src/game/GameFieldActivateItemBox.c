// bdc 0x088c2324 GameFieldActivateItemBox
#include "bdc.h"

/* Finds the placed item box gimmick (kind `+0x168 == 9`, `EW_GMKOBJ_ITEMBOX`) of the list `+0x658`
   whose record parameter equals `id` and calls its virtual `+0x84` with 0. */

void GameFieldActivateItemBox(CoreTask *task, s16 id)
{
    GameGimmick *gimmick;

    for (gimmick = ((GameFieldTask *)task)->gimmicks; gimmick != NULL;
         gimmick = (GameGimmick *)gimmick->base.base.next) {
        if (gimmick->kind == 9 && gimmick->active != 0 &&
            GameGimmickGetRecordParam(gimmick) == (s32)id) {
            const VtblEntry *entry = &((const VtblEntry *)gimmick->base.base.vtable)[16];

            ((void (*)(void *, int))entry->fn)((u8 *)gimmick + entry->delta, 0);
            return;
        }
    }
}
