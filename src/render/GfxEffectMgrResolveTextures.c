// bdc 0x088239b4 GfxEffectMgrResolveTextures
#include "bdc.h"

/* (Re)builds the texture table of an effect manager (`GfxEffectMgrCtor`): frees the old table
   `+0x90`, allocates `count(+0x8c)` pointers from the low heap and looks every name up
   (`GfxEffectMgrGetTextureName` → `GfxFindTexture`). */

void GfxEffectMgrResolveTextures(GfxEffectMgr *mgr)
{
  bool fromLow;
  void **old;
  void **table;
  int count;
  int i;

  old = mgr->textures;
  if (old != (void **)0) {
    MemLock();
    MemFree(old, (char *)0, 0);
    MemUnlock();
    mgr->textures = (void **)0;
  }
  count = mgr->textureCount;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  table = MemAlloc(count << 2, (char *)0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  mgr->textures = table;
  for (i = 0; i < mgr->textureCount; i++) {
    mgr->textures[i] = GfxFindTexture(GfxEffectMgrGetTextureName(mgr, i));
  }
}
