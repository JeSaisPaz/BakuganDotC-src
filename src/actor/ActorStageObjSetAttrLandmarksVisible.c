// bdc 0x088b2e2c ActorStageObjSetAttrLandmarksVisible
#include "bdc.h"

/* Shows or hides all attribute landmarks (virtual `+0x64`): visibility `+0xb8` and their effects
   (`ActorStageObjAttrLandmarkSetEffectsVisible`). Called by `BtlSetFieldEffectsActive`. */

void ActorStageObjSetAttrLandmarksVisible(char visible)

{
  ActorStageObjAttrLandmark *self;
  VtblEntry *slot;

  self = (ActorStageObjAttrLandmark *)0x0;
  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    self = (ActorStageObjAttrLandmark *)g_actorStageObjList->head;
  }
  if (self != (ActorStageObjAttrLandmark *)0x0) {
    do {
      slot = (VtblEntry *)(self->base.base.base.vtable) + 12;
      if (((s32 (*)(void *))slot->fn)((char *)self + slot->delta) != 0) {
        self->base.base.visible = visible;
        ActorStageObjAttrLandmarkSetEffectsVisible(self,visible);
      }
      self = (ActorStageObjAttrLandmark *)self->base.base.base.next;
    } while (self != (ActorStageObjAttrLandmark *)0x0);
  }
  return;
}
