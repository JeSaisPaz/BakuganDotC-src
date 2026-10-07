// bdc 0x088042b8 GfxInitBootResources
#include "bdc.h"

/* Start-up resource loader called once by `BootMainThread`. First clears the FPU exception-enable
   bits (FCSR bits 7..11). Sums the unpacked sizes of the 14 embedded TIM2 textures of
   `g_bootTextureTable` (`CoreLzssGetSize`), allocates one block for them from the low end of
   the heap (`g_bootTextureData`), allocates the 14 texture objects (0x140 bytes each, 0x1190
   bytes with the array cookie) and constructs them with
   `CxxVecNew``(block + g_cxxVecCookieSize, 14, 0x140, GfxTextureCtorEmpty, 0)`, then for each
   entry decompresses it into the data block (`CoreLzssDecompress`), writes the data cache back
   (`sceKernelDcacheWritebackInvalidateRange`) and builds the texture with
   `GfxTextureInitFromTim2` (no NULL check on the texture array). It finishes by creating the
   shared UI panel set (`UiLoadingInitShared`), the palette backup used for effect tinting
   (`GfxSetEffectTint``(0xffffffff)`) and the 'now loading' icon, which it immediately hides
   (`UiLoadIconShow`, `UiLoadIconHide`). The texture array pointer is not kept. */

void GfxInitBootResources(void)
{
  bool fromLow;
  u32 size;
  s32 total;
  s32 offset;
  s32 i;
  u8 *data;
  void *block;
  GfxTexture *tex;
  u8 *dst;

  PlatformFpuSetControl(PlatformFpuGetControl() & ~0xf80u);

  total = 0;
  for (i = 0; i < 14; i++) {
    total += CoreLzssGetSize(g_bootTextureTable[i * 2]);
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(total, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_bootTextureData = data;

  tex = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(0x1190, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (block != NULL) {
    tex = CxxVecNew((u8 *)block + g_cxxVecCookieSize, 14, 0x140, GfxTextureCtorEmpty, 0);
  }

  offset = 0;
  for (i = 0; i < 14; i++) {
    size = CoreLzssGetSize(g_bootTextureTable[i * 2]);
    dst = data + offset;
    CoreLzssDecompress(g_bootTextureTable[i * 2], dst);
    sceKernelDcacheWritebackInvalidateRange(dst, size);
    GfxTextureInitFromTim2(tex, g_bootTextureTable[i * 2 + 1], dst, 1);
    offset += size;
    tex++;
  }
  UiLoadingInitShared();
  GfxSetEffectTint(0xffffffff);
  UiLoadIconShow();
  UiLoadIconHide();
}
