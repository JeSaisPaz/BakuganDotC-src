// bdc 0x089dc524 GmoDlWriteMaterialState
#include "bdc.h"

/* Writes the dirty material-related GE state of the GMO display-list context `self`, one helper
   per dirty bit: 0x40000 morph weights (`GmoDlWriteMorphWeights`), 0x80000 patch division
   (`GmoDlWritePatchDivision`), 0x100000 material colours (`GmoDlWriteMaterialColors`),
   0x200000 blend (`GmoDlWriteBlendMode`), 0x400000 shading (`GmoDlWriteShadeModel`), 0x800000
   UV transform (`GmoDlWriteUvTransform`), 0x1000000 texture mapping mode
   (`GmoDlWriteTexMapMode`), 0x2000000 texture matrix (`GmoDlWriteTexMatrix`) and the low 16
   bits as enables (`GmoDlWriteEnables`); nothing when none of the bits in 0x7ffcffff is set.
   When fewer than 64 words are left in the list it writes into the context's scratch area
   `stateScratch` instead, then advances the real write pointer by the words written and copies
   them back; if they no longer fit it clears `end` (overflow marker) without copying. */

void GmoDlWriteMaterialState(GmoDlContext *self, u32 dirty)
{
  intptr_t delta;
  u32 *dst;

  dirty &= 0x7ffcffff;
  if (dirty == 0) {
    return;
  }
  delta = 0;
  if (self->end < self->cur + 64) {
    delta = self->cur - self->stateScratch; /* words from scratch to the real write pointer */
    self->cur = self->stateScratch;
  }
  if ((dirty & 0x40000) != 0) {
    GmoDlWriteMorphWeights(self);
  }
  if ((dirty & 0x80000) != 0) {
    GmoDlWritePatchDivision(self);
  }
  if ((dirty & 0x100000) != 0) {
    GmoDlWriteMaterialColors(self);
  }
  if ((dirty & 0x200000) != 0) {
    GmoDlWriteBlendMode(self);
  }
  if ((dirty & 0x400000) != 0) {
    GmoDlWriteShadeModel(self);
  }
  if ((dirty & 0x800000) != 0) {
    GmoDlWriteUvTransform(self);
  }
  if ((dirty & 0x1000000) != 0) {
    GmoDlWriteTexMapMode(self);
  }
  if ((dirty & 0x2000000) != 0) {
    GmoDlWriteTexMatrix(self);
  }
  if ((dirty & 0xffff) != 0) {
    GmoDlWriteEnables(self);
  }
  if (delta != 0) {
    self->cur = self->cur + delta;
    if (self->end < self->cur) {
      self->end = NULL;
      return;
    }
    dst = self->stateScratch + delta; /* the real write pointer before the redirect */
    memcpy(dst, self->stateScratch, (size_t)(self->cur - dst) * sizeof(u32));
  }
}
