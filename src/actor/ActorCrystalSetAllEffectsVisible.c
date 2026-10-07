// bdc 0x08859d50 ActorCrystalSetAllEffectsVisible
#include "bdc.h"

/* Applies `ActorCrystalSetEffectsVisible` to every crystal (vtable predicate `+0x58/+0x5c`) in
   `g_btlBakuganList`. */

void ActorCrystalSetAllEffectsVisible(bool visible)

{
  CoreObject **list;
  CoreObject *obj;

  list = (CoreObject **)BtlGetBakuganList();
  if (list != NULL && (obj = *list) != NULL) {
    do {
      const VtblEntry *isCrystal = &((const VtblEntry *)obj->vtable)[11];
      if (((s32 (*)(void *))isCrystal->fn)((u8 *)obj + isCrystal->delta) != 0) {
        ActorCrystalSetEffectsVisible((ActorCrystal *)obj, visible);
      }
      obj = obj->next;
    } while (obj != NULL);
  }
}
