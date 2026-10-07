// bdc 0x089dc800 GmoDlWriteBoneMatrices
#include "bdc.h"

/* Writes the skinning matrices of a skinned node (node flag 0x20000; otherwise nothing): for each
   bone (`boneCount`, bone node indices `block10`, inverse bind matrices `block14`) composes the
   bone's matrix (`GmoMat4ComposeLocal` of the bone node's `localMatrix`, the inverse bind matrix
   and the model's `scaleVec`, into `boneScratch`) and writes its 12 data words
   (`GfxDlWriteViewMatrix`). Without a mesh remap table (`mesh->skinData`) each matrix is
   preceded by `BONEMATRIXNUMBER` (`0x2a`, index*12) after a leading `0xff000000` word. With one, the
   matrices are written once (when `boneList` is still NULL) as `RET`-terminated (`0x0b`) 13-word
   sub-lists jumped over by a `BASE`/`JUMP` pair, `boneList` remembers their start, and then, after
   a `0xff000000` word, each of the mesh's `skinCount` used bones gets `BONEMATRIXNUMBER` + a
   `BASE`/`CALL` to its sub-list. */

void GmoDlWriteBoneMatrices(GmoDlContext *self)

{
  GmoNode *node;
  GmoModel *model;
  GmoNode *nodes;
  s16 *remap;
  u32 *list;
  u8 *target;
  s32 count;
  s32 boneIndex;
  const ScePspFMatrix4 *invBind;
  s32 i;

  node = self->node;
  if ((node->flags & 0x20000) == 0) {
    return;
  }
  remap = (s16 *)self->mesh->skinData;
  if (remap == NULL || self->boneList == NULL) {
    count = node->boneCount;
    if (remap == NULL) {
      *self->cur++ = 0xff000000;
    }
    list = self->cur;
    if (remap != NULL) {
      /* jump over the sub-lists: 2 jump words + 13 words per bone */
      target = (u8 *)(list + 2 + count * 13);
      *self->cur++ = 0x10000000 | (((u32)(uintptr_t)target >> 24) << 16);
      *self->cur++ = 0x08000000 | ((u32)(uintptr_t)target & 0xffffff);
      list = self->cur;
    }
    self->boneList = list;
    for (i = 0; i < count; i++) {
      node = self->node;
      model = self->model;
      nodes = model->nodes;
      boneIndex = ((s16 *)node->block10)[i];
      invBind = (const ScePspFMatrix4 *)node->block14 + i;
      if (remap == NULL) {
        *self->cur++ = 0x2a000000 | (u32)(i * 0xc);
      }
      GmoMat4ComposeLocal(&self->boneScratch, (const ScePspFMatrix4 *)nodes[boneIndex].localMatrix,
                          invBind, (const ScePspFVector4 *)model->scaleVec);
      GfxDlWriteViewMatrix(&self->cur, (const float *)&self->boneScratch);
      if (remap != NULL) {
        *self->cur++ = 0x0b000000;
      }
    }
  }
  if (remap != NULL) {
    count = self->mesh->skinCount;
    *self->cur++ = 0xff000000;
    for (i = 0; i < count; i++) {
      *self->cur++ = 0x2a000000 | (u32)(i * 0xc);
      target = (u8 *)((u32 *)self->boneList + remap[i] * 13);
      *self->cur++ = 0x10000000 | (((u32)(uintptr_t)target >> 24) << 16);
      *self->cur++ = 0x0a000000 | ((u32)(uintptr_t)target & 0xffffff);
    }
  }
}
