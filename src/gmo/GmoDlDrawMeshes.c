// bdc 0x089dbea8 GmoDlDrawMeshes
#include "bdc.h"

/* Draws the attribute records of the current draw item's material (`self->drawMaterial`, a
   `GmoMaterial` set by `GmoDlDrawItem`): with fog active (`g_gfxFogParams` non-null) first
   re-emits the fog colour command (`g_gfxFogColorCmd`, or the fog record's colour as `0xcf` when
   that is 0). If the material is hidden (`info[4]` bit 0x20 clear) returns `dirty` unchanged.
   Otherwise binds and emits each 0x40-byte `GmoAttr` (`attrs`, `attrCount`) through
   `GmoDlBindMesh`, threading the dirty mask through its results; when `info[3]` has any of bits
   2..7 set it writes a second pass with `GmoDlWriteMeshRenderState``(..., true)` (OR-ing its
   result into `g_gmoMeshFixupFlags`), a `BASE`/`CALL` to the mesh's prebuilt list `mesh->data`
   when present, then `0xc0000000`, `g_gfxLightSelectCmd`, `0xde000007`, and sets dirty bit
   0x80000000. Pending fix-ups are then undone: bit 0 re-emits the model's `uvTransform[0..1]` as
   `0x4a`/`0x4b` plus `0xde000007`/`0xc0000000`, bit 1 sets model `flags30` bit 0x200000
   (`GmoModelSetFlags30`) and emits `0x580000ff`, and any fix-up re-emits the ambient `0x5c`
   (`g_gfxModelAmbient`) and the lighting command (`g_gfxLightingCmd`). Finally it always
   re-emits `g_gfxLightingCmd` and the `0x90` colour (`g_gfxModelEmissive`). Returns the
   updated dirty mask. */

u32 GmoDlDrawMeshes(GmoDlContext *self, GmoModel *model, u32 dirty)

{
  GmoMaterial *material;
  u8 *info;
  u32 cmd;
  u32 fixup;
  u32 list;
  u32 *cur;
  s32 n;
  bool billboard;
  union {
    float f;
    u32 u;
  } bits;

  if (g_gfxFogParams != (BtlArenaFog *)0) {
    cmd = g_gfxFogColorCmd;
    if (cmd == 0) {
      cmd = (g_gfxFogParams->color & 0xffffff) | 0xcf000000;
    }
    *self->cur = cmd;
    self->cur = self->cur + 1;
  }
  material = (GmoMaterial *)self->drawMaterial;
  info = material->info;
  if ((info[4] & 0x20) == 0) {
    return dirty;
  }
  self->material = material->attrs;
  for (n = material->attrCount; n > 0; n--) {
    dirty = GmoDlBindMesh(self, model, dirty);
    self->material = self->material + 1;
  }
  billboard = ((info[3] & 0x1c) != 0) || ((info[3] & 0xe0) != 0);
  if (billboard) {
    fixup = GmoDlWriteMeshRenderState(self, self->drawMaterial, model, self->node, true);
    g_gmoMeshFixupFlags = g_gmoMeshFixupFlags | fixup;
    list = PspAddr(self->mesh->data);
    if (list != 0) {
      cur = self->cur;
      self->cur = cur + 1;
      *cur = ((list >> 24) << 16) | 0x10000000;
      cur = self->cur;
      self->cur = cur + 1;
      *cur = (list & 0xffffff) | 0x0a000000;
    }
    self->cur[0] = 0xc0000000;
    self->cur[1] = g_gfxLightSelectCmd;
    self->cur[2] = 0xde000007;
    dirty = dirty | 0x80000000;
    self->cur = self->cur + 3;
  }
  cur = self->cur;
  if (g_gmoMeshFixupFlags != 0) {
    if ((g_gmoMeshFixupFlags & 1) != 0) {
      bits.f = model->uvTransform[0];
      cur[0] = (bits.u >> 8) | 0x4a000000;
      bits.f = model->uvTransform[1];
      cur[1] = (bits.u >> 8) | 0x4b000000;
      cur[2] = 0xde000007;
      cur[3] = 0xc0000000;
      cur = cur + 4;
    }
    if ((g_gmoMeshFixupFlags & 2) != 0) {
      GmoModelSetFlags30(model, 0x200000, 0x200000);
      *cur = 0x580000ff;
      cur = cur + 1;
    }
    cur[0] = (g_gfxModelAmbient & 0xffffff) | 0x5c000000;
    cur[1] = g_gfxLightingCmd;
    cur = cur + 2;
    self->cur = cur;
  }
  *cur = g_gfxLightingCmd;
  self->cur[1] = (g_gfxModelEmissive & 0xffffff) | 0x90000000;
  self->cur = self->cur + 2;
  return dirty;
}
