// bdc 0x08a02f80 CxxVecDelete
#include "bdc.h"

/* Wrapper that forwards its arguments to `CxxVecDeleteEx`; the `delete[]` entry point used by
   compiled code. */

void CxxVecDelete(void *ptr, u32 count, u32 size, void *dtor, int doFree, int cookie)

{
  CxxVecDeleteEx(ptr,count,size,dtor,doFree,(void *)0x0,0,g_cxxVecCookieSize);
  return;
}

