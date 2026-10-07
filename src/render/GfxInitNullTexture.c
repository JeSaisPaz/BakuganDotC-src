// bdc 0x089f6b08 GfxInitNullTexture
#include "bdc.h"

/* Creates the 'NonTexture' placeholder texture once (does nothing when `g_nullTexture` is set):
   takes the LZSS-packed TIM2 `g_nullTextureBlob`, allocates `CoreLzssGetSize` bytes (rounded
   down to a multiple of 4) from the low heap, unpacks it (`CoreLzssDecompress`), allocates a
   0x140-byte texture object from the low heap and, if that succeeded, constructs it as
   `GfxTextureCtor(tex, "NonTexture", data, 1)` (which runs `GfxTextureInitFromTim2`). Stores the
   object (NULL when the allocation failed) in `g_nullTexture` and finally calls
   `GfxTextureListDetach`, which empties the texture list so the fallback texture is not found by
   name. Callers: `CoreTaskManagerInit`, `GfxGetNullTexture`. */

void GfxInitNullTexture(void)
{
  bool fromLow;
  u32 size;
  u8 *pixels;
  CoreObject *tex;

  if (g_nullTexture == NULL) {
    size = CoreLzssGetSize(g_nullTextureBlob);
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    pixels = MemAlloc(size & ~3u, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    CoreLzssDecompress(g_nullTextureBlob, pixels);
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    tex = MemAlloc(0x140, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (tex != NULL) {
      GfxTextureCtor(tex, "NonTexture", pixels, 1);
    }
    g_nullTexture = tex;
    GfxTextureListDetach();
  }
}
