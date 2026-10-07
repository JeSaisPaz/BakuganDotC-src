// bdc 0x089dc784 GmoDlWriteWorldMatrix
#include "bdc.h"

/* Writes the world matrix of the current node: the model matrix (`model+0x80`, or the identity
   `0x08aa5240` with model flag 1), composed with the node's local matrix and scale when the node is
   transformed (flag 0x10000, `GmoMat4ComposeLocal`) into `ctx+0x60`; then
   `GfxDlWriteWorldMatrix`. */

void GmoDlWriteWorldMatrix(GmoDlContext *self)

{
  ScePspFMatrix4 *parent;
  ScePspFMatrix4 *dst;
  GmoModel *model;

  model = self->model;
  parent = (ScePspFMatrix4 *)model->rootMatrix;
  if ((model->flags28 & 1) != 0) {
    parent = &g_gmoIdentityMatrix;
  }
  dst = parent;
  if ((self->node->flags & 0x10000) != 0) {
    dst = &self->worldScratch;
    GmoMat4ComposeLocal(dst, parent, (ScePspFMatrix4 *)self->node->localMatrix,
                        (ScePspFVector4 *)model->scaleVec);
  }
  self->worldMatrix = dst;
  GfxDlWriteWorldMatrix(&self->cur, (float *)dst);
  return;
}
