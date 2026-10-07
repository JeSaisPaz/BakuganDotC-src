// bdc 0x088f2718 GameEvent470ShowNextIcon
#include "bdc.h"

/* Vtable slot 9 of the task-470 event: steps through the field icon list of the requested message
   `+0x40` (counts `0x08a98fcc`, icon ids `0x08a98fb4`, position `+0x4c`): hides the previous icons
   and shows the next one on the field task (`GameFieldSetLocationSprite`). */

void GameEvent470ShowNextIcon(GameEvent470 *self)

{
  CoreTask *task;
  int msg;
  u32 pos;
  
  task = CoreTaskFind(500);
  if (task != (CoreTask *)0x0) {
    if ((self->base).iconPos == 0) {
      msg = (self->base).msgRequest;
      pos = 0;
    }
    else {
      GameFieldHideLocationSprites(task);
      pos = (self->base).iconPos;
      msg = (self->base).msgRequest;
    }
    if (pos != g_gameEvent470IconCounts[msg]) {
      GameFieldSetLocationSprite(task,(u8)g_gameEvent470IconIds[pos + msg * 2]);
      (self->base).iconPos = (self->base).iconPos + 1;
    }
  }
  return;
}

