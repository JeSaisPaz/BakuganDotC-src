// bdc 0x08a1e474 sceGuStart
#include "bdc.h"

/* libgu `sceGuStart`: begins recording into context `cid` (0..4, `g_guContexts`) with display list
   memory `list` of `size` bytes. Returns `0x80000001` if libgu is not initialised
   (`g_guInitialized`) and `0x80000021` if the context is already active (`parentContext >= 0`);
   both clear `g_guCurrentContext`. Otherwise, with interrupts suspended, it pushes the context
   (`parentContext` = the previous `g_guDeferredMode` id, which becomes `cid`) and makes it current,
   then sets `listStart`/`listCurrent` to the physical address of `list` (contexts 3 and 4) or its
   uncached `0x4…` alias; a cached `list` gets its first data-cache line written back/invalidated
   (`cache 0x1b`) and `listCurrent` is zero-padded up to the next 64-byte boundary. When the context
   is the main one (id 0) the list is enqueued with `sceGeListEnQueue` (`g_guListArgs`,
   `g_guGeCallbackId`); a negative result is returned as is, else it becomes `g_guListId`. On
   first use (`g_guDefaultsSent`) it writes the default state: `sceGuSetDither`
   (`g_guDefaultDither`), `sceGuPatchDivide`(16, 16), `sceGuColorMaterial`(7),
   `sceGuSpecular`(1.0) and `sceGuTexScale`(1, 1). For the main context with a draw buffer set
   (`g_guDispBufWidth` != 0) it copies the pixel format and display size into the context and
   re-emits PSM (`0xd2`), FBW (`0x9d`) and FBP (`0x9c`). Returns 0 on success. */

s32 sceGuStart(s32 cid, void *list, s32 size)
{
  GuContext *ctx;
  u8 *start;
  u8 *cur;
  u32 *dl;
  s32 intr;
  s32 id;

  if (g_guInitialized == 0) {
    g_guCurrentContext = NULL;
    return (s32)0x80000001;
  }
  intr = sceKernelCpuSuspendIntr();
  if (g_guContexts[cid].parentContext >= 0) {
    sceKernelCpuResumeIntr(intr);
    g_guCurrentContext = NULL;
    return (s32)0x80000021;
  }
  g_guContexts[cid].parentContext = g_guDeferredMode;
  g_guCurrentContext = &g_guContexts[cid];
  g_guDeferredMode = cid;
  sceKernelCpuResumeIntr(intr);

  if ((u32)(cid - 3) < 2) {
    ctx = g_guCurrentContext;
    start = (u8 *)((uintptr_t)list & 0x1fffffff);
    ctx->listSize = size;
    ctx->listCurrent = start;
    ctx->listStart = start;
  } else {
    ctx = g_guCurrentContext;
    start = (u8 *)(((uintptr_t)list & 0x1fffffff) | 0x40000000);
    ctx->listSize = size;
    ctx->listCurrent = start;
    ctx->listStart = start;
    if (((uintptr_t)list & 0x40000000) == 0) {
      intr = sceKernelCpuSuspendIntr();
      PlatformDcacheWriteback(list, 64); /* cache 0x1b: write back + invalidate one 64-byte line */
      ctx = g_guCurrentContext;
      while (((uintptr_t)(cur = ctx->listCurrent) & 0x3f) != 0) {
        ctx->listCurrent = cur + 4;
        *(u32 *)cur = 0;
      }
      sceKernelCpuResumeIntr(intr);
    }
  }

  if (g_guDeferredMode == 0) {
    g_guState4c = 0;
    id = sceGeListEnQueue(g_guCurrentContext->listStart, g_guCurrentContext->listCurrent,
                          g_guGeCallbackId, (SceGeListArgs *)&g_guListArgs);
    if (id < 0) {
      return id;
    }
    g_guListId = id;
  }
  if (g_guDefaultsSent == 0) {
    sceGuSetDither(g_guDefaultDither);
    sceGuPatchDivide(0x10, 0x10);
    sceGuColorMaterial(7);
    sceGuSpecular(1.0f);
    sceGuTexScale(1.0f, 1.0f);
    g_guDefaultsSent = 1;
  }
  if (g_guDeferredMode != 0) {
    return 0;
  }
  if (g_guDispBufWidth == 0) {
    return 0;
  }
  ctx = g_guCurrentContext;
  ctx->psm = g_guPixelFormat;
  ctx->dispWidth = g_guDispWidth;
  ctx->dispHeight = g_guDispHeight;
  dl = (u32 *)ctx->listCurrent;
  dl[0] = (u32)g_guPixelFormat | 0xd2000000;
  ctx->listCurrent = (u8 *)(dl + 3);
  dl[1] = ((((u32)g_guDrawBufOffset >> 24) & 0xf) << 16) | 0x9d000000 | (u32)g_guDispBufWidth;
  dl[2] = ((u32)g_guDrawBufOffset & 0xffffff) | 0x9c000000;
  return 0;
}
