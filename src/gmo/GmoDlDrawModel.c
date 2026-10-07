// bdc 0x089db244 GmoDlDrawModel
#include "bdc.h"

/* Draws the nodes of a GMO model into the display-list context: derives the active state mask
   `stateMask` from the model's mask `flags30` and the context flags (bits 1/2/4/8 enable the
   colour, transform, … groups), resets the cached state, installs the callbacks —
   `g_gmoMaterialCallback` as `materialCallback` (only when the model flags `flags28` have bit
   0x100, otherwise NULL), `g_gmoPostDrawCallback`/`g_gmoPostDrawCallbackArg` as
   `postDrawCallback`/`callbackArg` — then draws each listed node (indices into `model->nodes`,
   `stride` bytes apart, default 2; when `nodes` is NULL the model's `drawNodes` groups selected
   by context flags 0x100/0x200) with `GmoDlDrawNode` and finally calls the post-draw callback
   (if set) as `(ctx, dirty, 1, callbackArg)`, OR-ing its result into the dirty bits. Returns the
   accumulated dirty-state bits. */

typedef u32 (*GmoDlPostDrawFn)(GmoDlContext *ctx, u32 dirty, s32 final, void *arg);

u32 GmoDlDrawModel(GmoDlContext *self, GmoModel *model, u32 dirty, s16 *nodes, s32 count, s32 stride)
{
  u32 flags;
  u32 mask;
  u32 first;
  u32 second;
  GmoDlPostDrawFn cb;

  flags = self->flags;
  mask = model->flags30;
  if ((flags & 2) == 0) {
    mask &= 0xfff0ffff;
  }
  if ((flags & 4) == 0) {
    mask &= 0xf00f0000;
  }
  if ((flags & 8) == 0) {
    mask &= 0x7fffffff;
  }
  if ((flags & 1) == 0) {
    mask &= 0xbfffffff;
  }
  self->model = model;
  self->boundMaterial = NULL;
  self->lastEnables = 0xffffffff;
  self->stateMask = mask;
  self->nodeFlags = 0;
  self->meshState = 0;
  self->lastMeshState = 0;
  self->texMapDirty = 0;
  self->materialCallback = (model->flags28 & 0x100) != 0 ? g_gmoMaterialCallback : NULL;
  self->postDrawCallback = g_gmoPostDrawCallback;
  self->worldMatrix = NULL;
  self->callbackArg = g_gmoPostDrawCallbackArg;
  self->color = model->color;
  if (nodes == NULL) {
    first = 0;
    if ((flags & 0x100) != 0) {
      first = model->drawCount0;
    }
    second = 0;
    if ((flags & 0x200) != 0) {
      second = model->drawCount1;
    }
    count = first + second;
    nodes = model->drawNodes + (model->drawCount0 - first);
    stride = 0;
  }
  if (stride == 0) {
    stride = 2;
  }
  while (count > 0) {
    self->node = &model->nodes[*nodes];
    nodes = (s16 *)((u8 *)nodes + stride);
    dirty = GmoDlDrawNode(self, model, dirty);
    count--;
  }
  cb = (GmoDlPostDrawFn)self->postDrawCallback;
  if (cb != NULL) {
    dirty |= cb(self, dirty, 1, self->callbackArg);
  }
  return dirty;
}
