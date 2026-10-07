// bdc 0x08a1e6dc sceGuFinishId
#include "bdc.h"

/* libgu `sceGuFinishId(id)`: closes the current context's list. Returns `0x80000021` without
   writing anything if the current context's `isSubList` flag is set, and `0x80000107` if the
   context id (`g_guDeferredMode`, used here as pspsdk `gu_curr_context`) is not 0..4. Context 0
   gets FINISH(`id & 0xffff`) + END and then `GfxGuUpdateStall` (its negative error is returned
   as is); contexts 1/3 get SIGNAL(0x12)+END when `g_guCallMode` is 1, else RET; contexts 2/4 get
   FINISH+END. It then pops the context (interrupts suspended): the context id becomes the
   finished context's `parentContext` (reset to -1), and `g_guCurrentContext` that context or
   NULL. Returns the list length in bytes (`listCurrent - listStart`). */

s32 sceGuFinishId(u32 id)
{
  GuContext *ctx;
  u32 *p;
  s32 ret;
  s32 intr;
  s32 parent;

  id &= 0xffff;
  if (g_guCurrentContext->isSubList != 0) {
    return (s32)0x80000021;
  }
  switch (g_guDeferredMode) {
  case 0:
    ctx = g_guCurrentContext;
    p = (u32 *)ctx->listCurrent;
    p[0] = id | 0x0f000000;
    ctx->listCurrent = (u8 *)(p + 2);
    p[1] = 0x0c000000;
    ret = GfxGuUpdateStall();
    if (ret < 0) {
      return ret;
    }
    break;
  case 1:
  case 3:
    if (g_guCallMode == 1) {
      ctx = g_guCurrentContext;
      p = (u32 *)ctx->listCurrent;
      p[0] = 0x0e120000;
      ctx->listCurrent = (u8 *)(p + 2);
      p[1] = 0x0c000000;
    }
    else {
      ctx = g_guCurrentContext;
      p = (u32 *)ctx->listCurrent;
      p[0] = 0x0b000000;
      ctx->listCurrent = (u8 *)(p + 1);
    }
    break;
  case 2:
  case 4:
    ctx = g_guCurrentContext;
    p = (u32 *)ctx->listCurrent;
    p[0] = id | 0x0f000000;
    ctx->listCurrent = (u8 *)(p + 2);
    p[1] = 0x0c000000;
    break;
  default:
    return (s32)0x80000107;
  }

  ctx = g_guCurrentContext;
  ret = ctx->listCurrent - ctx->listStart;
  intr = sceKernelCpuSuspendIntr();
  parent = g_guContexts[g_guDeferredMode].parentContext;
  g_guContexts[g_guDeferredMode].parentContext = -1;
  g_guDeferredMode = parent;
  if (parent < 0) {
    g_guCurrentContext = NULL;
  }
  else {
    g_guCurrentContext = &g_guContexts[parent];
  }
  sceKernelCpuResumeIntr(intr);
  return ret;
}
