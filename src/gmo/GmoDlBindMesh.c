// bdc 0x089dc1b4 GmoDlBindMesh
#include "bdc.h"

/* Selects the mesh `ctx+0x24` as current: when it changed, merges its state bits, resolves its
   texture (index into `model+0x10` or `GmoHookFindTextureById(id)`) and flags 0x80000000 (texture
   dirty) if the texture differs from `ctx+0x28`; then emits the mesh (`GmoDlEmitMesh`), returning its result. */

u32 GmoDlBindMesh(GmoDlContext *self, GmoModel *model, u32 dirty)

{
  GmoAttr *mat;
  GmoAttr *ref;
  void *tex;
  u32 state;
  u32 idx;

  mat = self->material;
  if (self->boundMaterial != mat) {
    self->boundMaterial = mat;
    state = self->meshState;
    self->meshState = mat->flags;
    idx = mat->layerRef;
    dirty = dirty | state | mat->flags;
    ref = (GmoAttr *)(uintptr_t)idx;
    if (((idx + 1U) & 0xffff0000) == 0) {
      if ((idx & 0xffff) < (u32)model->textureCount) {
        ref = (GmoAttr *)((GmoLayer *)model->textures + idx);
      }
      else {
        ref = (GmoAttr *)0;
      }
    }
    if (ref == (GmoAttr *)0) {
      tex = GmoHookFindTextureById(mat->type);
    }
    else {
      tex = ((GmoLayer *)ref)->texture;
    }
    if (self->texture != tex) {
      self->texture = tex;
      dirty = dirty | 0x80000000;
    }
  }
  return GmoDlEmitMesh(self, model, dirty);
}
