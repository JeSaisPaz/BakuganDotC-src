// bdc 0x08823c50 GfxEffectMgrDtor
#include "bdc.h"

/* Destructor of the effect manager (`GfxEffectMgrCtor`): restores its vtable `g_gfxEffectMgrVtbl`, frees the texture
   table `+0x90`, frees the particle data `+0x80` when it owns it (`+0x94`), runs
   `GfxSpriteLayerDtor` and frees the object when `flags & 1`. */

void GfxEffectMgrDtor(GfxEffectMgr *mgr, u32 flags)

{
  void **ptr;
  s32 *ptr_00;
  
  if (mgr != (GfxEffectMgr *)0x0) {
    ptr = mgr->textures;
    (mgr->base).vtbl = g_gfxEffectMgrVtbl;
    if (ptr != (void **)0x0) {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
      mgr->textures = (void **)0x0;
    }
    if ((mgr->ownsData != '\0') && (ptr_00 = mgr->data, ptr_00 != (s32 *)0x0)) {
      MemLock();
      MemFree(ptr_00,(char *)0x0,0);
      MemUnlock();
      mgr->data = (s32 *)0x0;
    }
    GfxSpriteLayerDtor(&mgr->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(mgr,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

