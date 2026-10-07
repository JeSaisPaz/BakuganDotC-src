// bdc 0x0881d5d8 GfxEffectCtor
#include "bdc.h"

/* Constructor of the 0x220-byte effect object created by `GfxEffectSpawn` and its variants:
   runs `CoreObjectInit` (no chain), installs the vtable `g_gfxEffectVtbl`, clears the attach
   pointers, key, frame, `f1f4`/`f1f8`, `slotFlags`, `flags`, owner, mesh object, `lastTick`,
   `unk20c`, `model` and `f1f0`, stores the manager `mgr`, the definition `def` and the definition
   id `id`, sets an identity matrix, copies `g_colorWhite` into `color` and `size`, blend mode 1, GE stencil
   0xdcff0001/0xdd000000, alpha 0, resolves the persistent command block of key 0xffff
   (`GfxEffectFindKey`), zeroes `dir`, `offset` and `vec1d0` (VFPU bank C720) and sets
   `vec1e0` to (0, 0, 0, 1) (bank C730), takes the manager's first texture, UV offset 0 / scale 1, quad mode 1
   (`GfxSpriteInitQuadMode`), clears `worldDir[2]` and returns `effect`. */

void *GfxEffectCtor(GfxEffect *effect, GfxEffectMgr *mgr, void *def, int id)

{
  int i;

  CoreObjectInit(&effect->base, NULL);
  effect->base.vtable = g_gfxEffectVtbl;
  effect->attachPos = NULL;
  effect->attachMatrix = NULL;
  effect->key = 0;
  effect->frame = 0;
  effect->f1f4 = 0.0f;
  effect->f1f8 = 0.0f;
  effect->mgr = mgr;
  effect->def = def;
  effect->slotFlags = 0;
  for (i = 0; i < 16; i++) {
    effect->matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
  }
  effect->color[0] = g_colorWhite.x;
  effect->color[1] = g_colorWhite.y;
  effect->color[2] = g_colorWhite.z;
  effect->color[3] = g_colorWhite.w;
  effect->blendMode = 1;
  effect->preDrawCallback = NULL;
  effect->textureSlot = 0;
  effect->geStencilTest = 0xdcff0001;
  effect->geStencilOp = 0xdd000000;
  effect->alpha = 0;
  effect->size[0] = g_colorWhite.x;
  effect->size[1] = g_colorWhite.y;
  effect->size[2] = g_colorWhite.z;
  effect->size[3] = g_colorWhite.w;
  effect->id = id;
  effect->flags = 0;
  effect->persistentBlock = GfxEffectFindKey(effect, 0xffff);
  effect->meshObj = NULL;
  /* Bank constants C720 = (0, 0, 0, 0) and C730 = (0, 0, 0, 1). */
  for (i = 0; i < 4; i++) {
    effect->dir[i] = 0.0f;
  }
  for (i = 0; i < 4; i++) {
    effect->offset[i] = 0.0f;
  }
  for (i = 0; i < 4; i++) {
    effect->vec1d0[i] = 0.0f;
  }
  effect->vec1e0[0] = 0.0f;
  effect->vec1e0[1] = 0.0f;
  effect->vec1e0[2] = 0.0f;
  effect->vec1e0[3] = 1.0f;
  effect->texture = effect->mgr->textures[0];
  effect->ownerBakugan = NULL;
  effect->ownerId = 0;
  effect->uvOffset[0] = 0.0f;
  effect->uvOffset[1] = 0.0f;
  effect->uvScale[0] = 1.0f;
  effect->uvScale[1] = 1.0f;
  GfxSpriteInitQuadMode((GfxSprite *)effect, 1);
  effect->worldDir[2] = 0.0f;
  effect->lastTick = 0;
  effect->unk20c = 0;
  effect->model = NULL;
  effect->f1f0 = 0.0f;
  return effect;
}
