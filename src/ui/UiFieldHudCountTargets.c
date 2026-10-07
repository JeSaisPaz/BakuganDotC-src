// bdc 0x088cf0b0 UiFieldHudCountTargets
#include "bdc.h"

/* Counts the target gimmicks of the field's gimmick list (`GameFieldTask.gimmicks`) for the field
   HUD, starting at the list head saved in `targetMark`: `targetCount` (total) and `targetsDone`
   both grow by 2 for type 0xbd9 and by 1 for types 0xbc7, 0x1778 and 0xbdf; an unbroken breakable
   container (type 0xbdc, `+0x1e0` clear) adds to `targetCount` only, 1 for record contents kind
   0..2 and 3 for kinds 3..5 (larger kinds add nothing). Returns early when there is no field
   task gimmick list. */

void UiFieldHudCountTargets(UiFieldHud *self)

{
  GameGimmick **head;
  GameGimmick *g;
  GameGimmickRecord *rec;
  u8 kind;

  /* the original tests the list-head address (task + 0x658) against NULL, not the task */
  head = &((GameFieldTask *)GameFieldFindTask())->gimmicks;
  if (head == NULL) {
    return;
  }
  g = ((GameFieldTask *)GameFieldFindTask())->gimmicks;
  *(GameGimmick **)self->targetMark = g;
  if (g == NULL) {
    return;
  }
  g = *(GameGimmick **)self->targetMark;
  self->targetCount = 0;
  self->targetsDone = 0;
  while (g != NULL) {
    if (g->typeId == 0xbd9) {
      self->targetCount = self->targetCount + 2;
      self->targetsDone = self->targetsDone + 2;
    }
    else if (g->typeId == 0xbc7) {
      self->targetCount = self->targetCount + 1;
      self->targetsDone = self->targetsDone + 1;
    }
    else if (g->typeId == 0x1778) {
      self->targetCount = self->targetCount + 1;
      self->targetsDone = self->targetsDone + 1;
    }
    else if (g->typeId == 0xbdf) {
      self->targetCount = self->targetCount + 1;
      self->targetsDone = self->targetsDone + 1;
    }
    else if (g->typeId == 0xbdc && ((GameGimmickCollidable *)g)->broken == 0) {
      rec = (GameGimmickRecord *)g->record;
      kind = rec->contentKind;
      switch (kind) {
      case 1:
      case 2:
        self->targetCount = self->targetCount + 1;
        break;
      case 3:
      case 4:
      case 5:
        self->targetCount = self->targetCount + 3;
        break;
      case 0:
        self->targetCount = self->targetCount + 1;
        break;
      default:
        break;
      }
    }
    g = (GameGimmick *)g->base.base.next;
  }
  return;
}
