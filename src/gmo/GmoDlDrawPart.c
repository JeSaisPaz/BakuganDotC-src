// bdc 0x089db524 GmoDlDrawPart
#include "bdc.h"

/* Draws the 0x30-byte draw items of a GMO part (`ctx+0x18`, items at `+4`, count `+8`), skipping
   those whose material does not match the current pass `g_gfxModelDrawPass` (1 opaque, 2
   translucent, by the material's translucency byte `+6`); each kept item goes to `GmoDlDrawItem`.
    */

u32 GmoDlDrawPart(GmoDlContext *self, GmoModel *model, u32 dirty)

{
  s32 pass;
  GmoMaterial *material;
  u32 count;
  u8 translucent;

  self->mesh = self->part->meshes;
  count = self->part->meshCount;
  if (count != 0) {
    do {
      pass = g_gfxModelDrawPass;
      if (g_gfxModelDrawPass == 0) {
        goto draw;
      }
      material = (GmoMaterial *)GmoModelGetMaterial(model, self->mesh->materialIndex);
      translucent = material->info[6];
      if ((pass & 1U) == 0) {
        if (translucent == 0) goto draw;
      } else {
        if (translucent != 0) goto draw;
      }
      goto next;
draw:
      dirty = GmoDlDrawItem(self, model, dirty);
next:
      count = count - 1;
      self->mesh = self->mesh + 1;
    } while (0 < (int)count);
  }
  return dirty;
}
