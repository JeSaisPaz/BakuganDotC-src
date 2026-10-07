// bdc 0x08a1fa00 GfxGuUpdateStall
#include "bdc.h"

/* libgu internal stall update called by `sceGuFinishId`: unless the library is in a deferred mode
   (`g_guDeferredMode`) or the current context (`g_guCurrentContext`) is a call/send list
   (`isSubList`), moves the GE stall address of the running list (`g_guListId`) to the context's
   write pointer with `sceGeListUpdateStallAddr`. Returns 0, or the negative GE error. */

s32 GfxGuUpdateStall(void)
{
  s32 ret = 0;

  if (g_guDeferredMode == 0 && g_guCurrentContext->isSubList == 0) {
    ret = sceGeListUpdateStallAddr(g_guListId, g_guCurrentContext->listCurrent);
    if (ret >= 0) {
      ret = 0;
    }
  }
  return ret;
}
