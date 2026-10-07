// bdc 0x08a02c84 CxxVecNew
#include "bdc.h"

/* Convenience wrapper of `CxxVecNewEx` for constructing an already allocated array: calls it with
   `ptr`, `count`, element `size`, the per-element constructor `ctor`, no allocator and the cookie
   size from `g_cxxVecCookieSize`. `GfxInitBootResources` uses it to build its 14 texture objects
   (`CxxVecNew(block + 0x10, 14, 0x140, ctor, 0)`); nine callers in total. */

void *CxxVecNew(void *ptr, s32 count, s32 size, void *ctor, s32 unwindFlag)

{

  
  return CxxVecNewEx(ptr,count,size,0,ctor,unwindFlag,(void *)0x0,0,0,1,0,g_cxxVecCookieSize);
}

