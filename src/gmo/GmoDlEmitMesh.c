// bdc 0x089dc298 GmoDlEmitMesh
#include "bdc.h"

/* Emits one mesh into the GMO display-list context `self`: masks the dirty bits with the active
   mask `stateMask`, lets the dirty-mask callback `postDrawCallback` (called as
   `(self, dirty, 0, callbackArg)`) consume some (its return value is cleared from the mask and is
   also the function's return value), writes the world matrix (`GmoDlWriteWorldMatrixIfDirty`),
   bone matrices (`GmoDlWriteBonesIfDirty`), material state (`GmoDlWriteMaterialState`) and
   texture (`GmoDlWriteTextureIfDirty`), the per-mesh render state
   (`GmoDlWriteMeshRenderState`, result stored in `g_gmoMeshFixupFlags`), and, when the mesh has
   a prebuilt primitive list `mesh->data`, a GE `BASE`/`CALL` (`0x10`/`0x0a`) to it (only when the
   two words fit; otherwise only the write pointer advances) followed by the material callback
   `materialCallback` (called with `self`, return ignored; `GmoDlDrawModel` installs it from
   `g_gmoMaterialCallback`, the known one is `GmoViewMaterialCallback`). */

typedef u32 (*GmoDlPostDrawFn)(GmoDlContext *ctx, u32 dirty, s32 final, void *arg);
typedef void (*GmoDlMaterialFn)(GmoDlContext *ctx);

u32 GmoDlEmitMesh(GmoDlContext *self, GmoModel *model, u32 dirty)
{
  u32 *dl;
  u32 addr;
  u32 consumed;
  GmoDlPostDrawFn cb;
  GmoDlMaterialFn matCb;

  dirty &= self->stateMask;
  consumed = 0;
  cb = (GmoDlPostDrawFn)self->postDrawCallback;
  if (cb != NULL) {
    consumed = cb(self, dirty, 0, self->callbackArg);
    dirty &= ~consumed;
  }
  GmoDlWriteWorldMatrixIfDirty(self, dirty);
  GmoDlWriteBonesIfDirty(self, dirty);
  GmoDlWriteMaterialState(self, dirty);
  GmoDlWriteTextureIfDirty(self, dirty);
  g_gmoMeshFixupFlags = GmoDlWriteMeshRenderState(self, self->drawMaterial, model, self->node, false);
  if (self->mesh->data != NULL) {
    addr = PspAddr(self->mesh->data); /* GE address of the primitive list */
    if (self->end < self->cur + 2) {
      self->cur = self->cur + 2;
    } else {
      dl = self->cur;
      self->cur = dl + 1;
      *dl = (addr >> 24) << 16 | 0x10000000;
      dl = self->cur;
      self->cur = dl + 1;
      *dl = (addr & 0xffffff) | 0x0a000000;
    }
    matCb = (GmoDlMaterialFn)self->materialCallback;
    if (matCb != NULL) {
      matCb(self);
    }
  }
  return consumed;
}
