// bdc 0x088aab18 ActorStageObjIsLandmark
#include "bdc.h"

/* `ActorStageObjKindIsLandmark` for the object's kind `+0x218`. */

int ActorStageObjIsLandmark(ActorStageObjBase *self)
{
  return ActorStageObjKindIsLandmark(self->kind);
}
