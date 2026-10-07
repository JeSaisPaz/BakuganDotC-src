// bdc 0x088ac950 ActorStageObjIsAttrLandmark
#include "bdc.h"

/* `ActorStageObjKindIsAttrLandmark` for the object's kind `+0x218`. */

int ActorStageObjIsAttrLandmark(ActorStageObjBase *self)
{
  return ActorStageObjKindIsAttrLandmark(self->kind);
}
