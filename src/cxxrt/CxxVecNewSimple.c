// bdc 0x08a02ccc CxxVecNewSimple
#include "bdc.h"

/* Wrapper of `CxxVecNewEx` used by `new T[n]` sites with a constructor but no cleanup:
   `CxxVecNewEx(ptr, count, size, 0, ctor, 0, NULL, 0, 0, 0, 0, cookie)` with the cookie size from
   `g_cxxVecCookieSize`. */

void *CxxVecNewSimple(void *ptr, u32 count, s32 size, void *ctor)

{

  
  return CxxVecNewEx(ptr,count,size,0,ctor,0,(void *)0x0,0,0,0,0,g_cxxVecCookieSize);
}

