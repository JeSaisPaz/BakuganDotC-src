// bdc 0x088853e4 BtlShadowDtor
#include "bdc.h"

/* Destroys a ground-shadow object: releases its `billboard` from `g_worldSpriteLayer`
   (`UiSpriteLayerRelease`) or, without a billboard, sets the smoke-column mesh object's `step`
   to 2 (sub-state 2 of `GfxMeshObjStateStencilSmoke` hides it and its two stencil masks); frees
   the object when `flags & 1`. Does nothing for NULL. Called by `BtlBakuganDtor` and
   `ActorDtor`. */

void BtlShadowDtor(BtlShadow *shadow, u32 flags)
{
  GfxMeshObj *mesh;

  if (shadow != NULL) {
    if (shadow->billboard != NULL) {
      UiSpriteLayerRelease(g_worldSpriteLayer, shadow->billboard);
    } else {
      mesh = shadow->mesh;
      mesh->step = 2;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(shadow, NULL, 0);
      MemUnlock();
    }
  }
}
