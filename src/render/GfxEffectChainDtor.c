// bdc 0x089e7cd8 GfxEffectChainDtor
#include "bdc.h"

/* Destructor of an effect chain: frees the buffer `+0x30`, the point buffer and the link-length
   array, then the chain when `flags & 1`. */

void GfxEffectChainDtor(GfxEffectChain *chain, u32 flags)

{
  void *ptr;
  ScePspFVector4 *ptr_00;
  float *ptr_01;
  
  if (chain != (GfxEffectChain *)0x0) {
    ptr = chain->extraBuffer;
    if (ptr == (void *)0x0) {
      ptr_00 = chain->points;
    }
    else {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
      chain->extraBuffer = (void *)0x0;
      ptr_00 = chain->points;
    }
    if (ptr_00 == (ScePspFVector4 *)0x0) {
      ptr_01 = chain->linkLengths;
    }
    else {
      MemLock();
      MemFree(ptr_00,(char *)0x0,0);
      MemUnlock();
      chain->points = (ScePspFVector4 *)0x0;
      ptr_01 = chain->linkLengths;
    }
    if (ptr_01 != (float *)0x0) {
      MemLock();
      MemFree(ptr_01,(char *)0x0,0);
      MemUnlock();
      chain->linkLengths = (float *)0x0;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(chain,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

