// bdc 0x08a01a74 GfxFabObjectCtor
#include "bdc.h"

/* Constructor of a placed `.fab` object (0xf0 bytes, vtable `0x08af5a6c`): base node ctor linking
   it into `list` (the clip's object list), clears `+0x20`. Returns `obj`. Initialised afterwards by
   `GfxFabObjectInit`. */

GfxFabObject *GfxFabObjectCtor(GfxFabObject *obj, void *list)

{
  CoreObjectInitInList(&obj->base, list);
  obj->base.vtable = g_gfxFabObjectVtbl;
  obj->link = NULL;
  return obj;
}
