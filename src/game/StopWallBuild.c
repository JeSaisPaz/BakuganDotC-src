// bdc 0x088b3984 StopWallBuild
#include "bdc.h"

/* Builds a stop wall once (skipped when `collider` exists): its box shape
   (`StopWallBuildShape``(wall, id)` when `shape` is still NULL), its two effects
   (`StopWallSpawnEffects` with copies of `cornerB` / `cornerA` and the texture `"StopWallTuto"`,
   whose mip slope/bias are set to -0.15 / -8.0 with `GfxTextureSetMipSlope` /
   `GfxTextureSetMipBias`), full alpha (`color[3] = 1.0`) on both effects for wall 0, and a
   400-byte `CollisionCollider` (`CollisionColliderCtor``(c, 2)`,
   `CollisionColliderInit``(c, shape, 8, 0, 0)`, `flags` bit 0 set, `byte104 = 0`,
   `attachDirty = 1`) stored in `collider`. */

void StopWallBuild(StopWall *self, s32 id)
{
  float cornerB[4] __attribute__((aligned(16)));
  float cornerA[4] __attribute__((aligned(16)));
  bool fromLow;
  GfxTexture *tex;
  CollisionCollider *collider;
  int i;

  if (self->collider == NULL) {
    if (self->shape == NULL) {
      StopWallBuildShape(self, id);
    }
    for (i = 0; i < 4; i++) {
      cornerB[i] = self->cornerB[i];
    }
    for (i = 0; i < 4; i++) {
      cornerA[i] = self->cornerA[i];
    }
    StopWallSpawnEffects(8.0f, self, cornerB, cornerA, "StopWallTuto");
    tex = (GfxTexture *)GfxFindTexture("StopWallTuto");
    tex->picture->mipBias = -8.0f;
    tex->picture->mipSlope = -0.150000006f;
    GfxTextureSetMipSlope(tex->picture->mipSlope, tex);
    GfxTextureSetMipBias(tex->picture->mipBias, tex);
    if (id == 0) {
      for (i = 0; i < 2; i++) {
        if (self->effect[i] != NULL) {
          ((GfxEffect *)self->effect[i])->color[3] = 1.0f;
        }
      }
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    collider = (CollisionCollider *)MemAlloc(sizeof(CollisionCollider), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (collider != NULL) {
      CollisionColliderCtor((CoreNode *)collider, 2);
    }
    self->collider = collider;
    CollisionColliderInit((CoreNode *)collider, (const u32 *)self->shape, 8, NULL, 0);
    ((CollisionCollider *)self->collider)->byte104 = 0;
    ((CollisionCollider *)self->collider)->flags |= 1;
    ((CollisionCollider *)self->collider)->attachDirty = 1;
  }
}
