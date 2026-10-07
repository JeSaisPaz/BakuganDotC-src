// bdc 0x089dc3e8 GmoDlWriteWorldMatrixIfDirty
#include "bdc.h"

/* If bit 0x10000 (world matrix) is dirty, writes the node's world matrix
   (`GmoDlWriteWorldMatrix`) or, when the 0x34-byte block does not fit, only advances the write
   pointer (overflow accounting). */

void GmoDlWriteWorldMatrixIfDirty(GmoDlContext *self, u32 dirty)

{
  if ((dirty & 0x10000) != 0) {
    if (self->end < self->cur + 0xd) {
      self->cur = self->cur + 0xd;
      return;
    }
    GmoDlWriteWorldMatrix(self);
  }
  return;
}

