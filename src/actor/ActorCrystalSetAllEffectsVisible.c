// bdc 0x08859d50 ActorCrystalSetAllEffectsVisible
#include "bdc.h"

/* Applies `ActorCrystalSetEffectsVisible` to every crystal (vtable predicate `+0x58/+0x5c`) in
   `g_btlBakuganList`. */

typedef struct CrystalVtView {
  u8 _pad[0x58];
  s16 thisAdj;
  s16 _pad5a;
  s32 (*isCrystal)(void *);
} CrystalVtView;

void ActorCrystalSetAllEffectsVisible(bool visible)

{
  CoreObject **list;
  CoreObject *obj;

  list = (CoreObject **)BtlGetBakuganList();
  if (list != NULL && (obj = *list) != NULL) {
    do {
      const CrystalVtView *vt = (const CrystalVtView *)obj->vtable;
      if (vt->isCrystal((u8 *)obj + vt->thisAdj) != 0) {
        ActorCrystalSetEffectsVisible((ActorCrystal *)obj, visible);
      }
      obj = obj->next;
    } while (obj != NULL);
  }
}
