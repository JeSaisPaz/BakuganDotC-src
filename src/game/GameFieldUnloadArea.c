// bdc 0x088bf3a8 GameFieldUnloadArea
#include "bdc.h"

/* Unloads the current area of the field task (id 500, `GameFieldCtor`): waits for the GE
   (`GfxWaitGeIdle`), closes the HUD (`GameFieldCloseHud`), resets the `+0x790` manager
   (`GameEvent470Release`), optionally clears the gimmick and `+0x670` lists (`CoreObjectListDeleteAll`, when
   `clearLists`), deletes every non-player actor of the actor list (clearing the player's `+0x350`)
   and releases the party manager `+0x78c` (`GameFieldCharSetClearForReload`). */

void GameFieldUnloadArea(GameFieldTask *task, bool clearLists)
{
  Actor **list;
  Actor *obj;
  Actor *next;

  GfxWaitGeIdle();
  GameFieldEmptyHookA((CoreTask *)task);
  GameFieldCloseHud();
  GameEvent470Release(task->events);
  if (clearLists) {
    CoreObjectListDeleteAll((CoreObjectList *)&task->gimmicks);
    CoreObjectListDeleteAll(&task->objList670);
  }
  list = (Actor **)ActorGetList();
  obj = *list;
  while (obj != NULL) {
    if (obj->isPlayer == 0) {
      next = (Actor *)CoreObjectUnlink(&obj->base.base);
      if (obj != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)obj->base.base.vtable)[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
      }
    } else {
      next = (Actor *)obj->base.base.next;
      obj->placement = NULL;
    }
    obj = next;
  }
  GameFieldCharSetClearForReload(task->charSet);
}
