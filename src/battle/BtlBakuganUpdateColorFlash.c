// bdc 0x08865e4c BtlBakuganUpdateColorFlash
#include "bdc.h"

/* Fades out the unit's colour flash: while the strength (`flashColor[3]`) is positive it drops by
   0.05 per frame; once it reaches 0 or below it is set to 0 and `fogEnabled` is cleared, otherwise
   the flash colour saturated to [0, 1] is passed to vtable entry 4 (`GfxModelSetFog` in the
   base class). */
void BtlBakuganUpdateColorFlash(BtlBakugan *self)
{
  ScePspFVector4 colour;
  const VtblEntry *setFog;
  float strength;

  if (self->flashColor[3] <= 0.0f) {
    return;
  }
  strength = self->flashColor[3] - 0.05f;
  self->flashColor[3] = strength;
  if (strength <= 0.0f) {
    self->flashColor[3] = 0.0f;
    self->base.fogEnabled = 0;
    return;
  }
  colour.x = self->flashColor[0];
  colour.y = self->flashColor[1];
  colour.z = self->flashColor[2];
  colour.w = self->flashColor[3];
  setFog = &((const VtblEntry *)self->base.base.vtable)[4];
  colour.x = VfSat0(colour.x);
  colour.y = VfSat0(colour.y);
  colour.z = VfSat0(colour.z);
  colour.w = VfSat0(colour.w);
  ((void (*)(void *, const ScePspFVector4 *))setFog->fn)((u8 *)self + setFog->delta, &colour);
}
