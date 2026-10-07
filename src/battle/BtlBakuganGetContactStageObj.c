// bdc 0x08860520 BtlBakuganGetContactStageObj
#include "bdc.h"

/* Returns the stage object the unit's contact collider (`contactObj`, `+0x210`) is touching: when
   that collider exists, its `layer` is 9 and its `owner` is non-NULL and still a live stage object
   (`ActorStageObjValidate`), returns `owner` (re-read from `self->contactObj` after the call);
   otherwise NULL. */

void *BtlBakuganGetContactStageObj(BtlBakugan *self)
{
  CollisionCollider *contact = self->contactObj;
  void *stageObj = (void *)0;

  if (contact != (CollisionCollider *)0 && contact->layer == 9 && contact->owner != (void *)0 &&
      ActorStageObjValidate(contact->owner) != (void *)0) {
    stageObj = self->contactObj->owner;
  }
  return stageObj;
}
