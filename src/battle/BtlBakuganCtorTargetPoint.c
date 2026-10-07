// bdc 0x0885e14c BtlBakuganCtorTargetPoint
#include "bdc.h"

/* Model-less variant of `BtlBakuganCtor` used by the TargetPoint unit base constructor
   `BtlTargetPointCtor`: builds an empty model (`GfxModelCtorEmpty`), installs the Bakugan
   vtable `0x08af1fa4` and the two embedded shape records (`+0x270` tag 4 with vtables
   `0x08af5624`/`0x08af5564`, `+0x2c0` tag 3 with `0x08af55c4`), constructs the combat state
   (`BtlCombatCtor` at `+0x434`), default-initialises the fields (`BtlBakuganInitFields`), sets
   kind `+8 = 9999`, takes the next serial number (`g_btlNextUnitId++` into `+0xc`), binds the
   combat state to species 0x15 (`BtlBakuganInitCombat`), sets `height` (`+0x17c`) to 100.0, names
   the model `"TargetPoint"` (`GfxModelSetName`) and appends it to `g_btlBakuganList`. Returns
   `self`. */

void *BtlBakuganCtorTargetPoint(BtlBakugan *self)
{
  GfxModelCtorEmpty(&self->base);
  self->base.base.vtable = g_btlBakuganVtbl;
  self->bodyShape.vtbl = g_collisionCapsuleVtbl;
  ((SegmentShape *)self->bodyShape.segmentHead)->info = (void *)g_collisionSegmentVtbl;
  ((SegmentShape *)self->bodyShape.segmentHead)->type = 2;
  self->bodyShape.type = 4;
  self->pushShape.vtbl = g_collisionSphereVtbl;
  self->pushShape.type = 3;
  BtlCombatCtor(&self->combat);
  BtlBakuganInitFields(self);
  self->base.base.unk08 = 9999;
  self->base.base.id = g_btlNextUnitId++;
  BtlBakuganInitCombat(self, 0x15);
  self->height = 100.0f;
  GfxModelSetName(&self->base, "TargetPoint");
  CoreObjectListAppend(&self->base.base, (CoreObjectList *)g_btlBakuganList);
  return self;
}
