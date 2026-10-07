// bdc 0x089dc43c GmoDlWriteBonesIfDirty
#include "bdc.h"

/* If bit 0x20000 (skinning) is dirty and the node is skinned, writes the bone matrices
   (`GmoDlWriteBoneMatrices`), or only advances the write pointer by their size when they do not
   fit. */

void GmoDlWriteBonesIfDirty(GmoDlContext *self, u32 dirty)

{
  int size;
  u32 hasSkin;
  u32 *next;

  if (((dirty & 0x20000) != 0) && ((self->node->flags & 0x20000) != 0)) {
    size = (uint)self->node->boneCount * 0xd + 0x13;
    if (self->end < self->cur + size) {
      hasSkin = (uint)(self->mesh->skinData != (void *)0x0);
      if ((hasSkin == 0) || (self->boneList == (void *)0x0)) {
        size = hasSkin + size + -0x12;
      }
      else {
        size = 0;
      }
      if (hasSkin != 0) {
        size = (uint)self->mesh->skinCount * 3 + size + 1;
      }
      next = self->cur + size;
      if (self->end < next) {
        self->cur = next;
        self->boneList = next;
        return;
      }
    }
    GmoDlWriteBoneMatrices(self);
  }
  return;
}
