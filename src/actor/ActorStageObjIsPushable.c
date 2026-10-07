// bdc 0x088a978c ActorStageObjIsPushable
#include "bdc.h"

/* `ActorStageObjCategoryIsPushable` for the object's category `+0x208`. Used by
   `ActorStageObjDropItem`, `ActorStageObjBaseInit` and `ActorStageObjRaycastAll`. */

int ActorStageObjIsPushable(ActorStageObjBase *self)
{
  return ActorStageObjCategoryIsPushable(self->category);
}
