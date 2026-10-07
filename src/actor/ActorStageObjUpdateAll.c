// bdc 0x088a9538 ActorStageObjUpdateAll
#include "bdc.h"

/* Per-frame step of the stage-object system: clears the attribute aura flags
   (`ActorStageObjAttrLandmarkClearAuraFlags`), updates the motion of every model in the chain
   `g_actorStageObjList` (`GfxModelChainUpdateMotion`) and increments the frame counter `g_actorStageObjFrame`.
   Called from the battle, demo and field update loops. */

void ActorStageObjUpdateAll(void)

{
  CoreObject *first;

  ActorStageObjAttrLandmarkClearAuraFlags();
  first = (CoreObject *)0x0;
  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    first = g_actorStageObjList->head;
  }
  GfxModelChainUpdateMotion(first);
  g_actorStageObjFrame = g_actorStageObjFrame + 1;
  return;
}

