// bdc 0x088fd43c GameQuestCamModeSyncNode
#include "bdc.h"

/* Copies the followed object's current path node (first `s16` of the followed object) into `node` and, when
   it is not -1, calls slot 3 (`+0x18`) of the attached object `state` (vtable at its `+4`). */

typedef struct {
  void *pad;
  VtblEntry *vtbl;
} GameQuestCamStateObj;

void GameQuestCamModeSyncNode(GameQuestCamModeBase *self)
{
  GameQuestCamStateObj *st;
  VtblEntry *vt;

  self->node = *(s16 *)(self->base).base.followed;
  if (self->node != -1) {
    st = (GameQuestCamStateObj *)self->state;
    vt = st->vtbl;
    ((void (*)(void *))vt[3].fn)((u8 *)st + vt[3].delta);
  }
}
