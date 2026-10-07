// bdc 0x088a7b1c ActorStageObjAttrLandmarkAura3AllUnits
#include "bdc.h"

/* Aura handler for type 3 (entry 3 of the table `0x08a83f44`): applies the aura
   (`ActorStageObjAttrLandmarkApplyAura`) to every unit of `g_btlBakuganList` for which virtual
   `+0x74` is false. */

void ActorStageObjAttrLandmarkAura3AllUnits(ActorStageObjAttrLandmark *self)
{
  CoreObjectList *list = (CoreObjectList *)BtlGetBakuganList();
  CoreObject *unit;

  if (list != (CoreObjectList *)0x0 && (unit = list->head) != (CoreObject *)0x0) {
    do {
      const VtblEntry *ent = &((const VtblEntry *)unit->vtable)[14];

      if (((int (*)(void *))ent->fn)((char *)unit + ent->delta) == 0) {
        ActorStageObjAttrLandmarkApplyAura(self, unit);
      }
      unit = unit->next;
    } while (unit != (CoreObject *)0x0);
  }
}
