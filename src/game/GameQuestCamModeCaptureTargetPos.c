// bdc 0x088fd4dc GameQuestCamModeCaptureTargetPos
#include "bdc.h"

/* Stores the followed object's position (vtable slot 2, `+0x10`, of `followed`; vtable at its `+4`) into the
   quest camera mode's `targetPos`. */

typedef struct {
  void *pad;
  VtblEntry *vtbl;
} GameQuestCamFollowed;

void GameQuestCamModeCaptureTargetPos(GameQuestCamModeBase *self)
{
  GameQuestCamFollowed *obj = (GameQuestCamFollowed *)(self->base).base.followed;
  VtblEntry *vt = obj->vtbl;
  ScePspFVector4 *p = ((ScePspFVector4 *(*)(void *))vt[2].fn)((u8 *)obj + vt[2].delta);

  self->targetPos = *p;
}
