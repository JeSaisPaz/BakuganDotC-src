// bdc 0x08a191e0 GmoDlCacheEmitInline
#include "bdc.h"

/* Single-buffer path of `GmoModelBuildCachedDl`: copies the model's cached display list
   (`listWords` words, plus 3 padding words, 16-byte aligned) into the caller's buffer `*buf` —
   from the ready copy `slot` when valid (whole 64-byte blocks, each source line written back and
   invalidated with `cache 0x1b`, the rest via `memcpy`), otherwise patching it fresh
   (`GmoDlCachePatch`). Decrements `*size` and advances `*buf`; returns the bytes used (also when
   `buf` is NULL, then nothing is written), or 0 when there is no model/cache, `*buf` is NULL or
   `*size` is too small. No VFPU value crosses its calls. */

u32 GmoDlCacheEmitInline(GmoModel *model, u32 **buf, u32 *size, s32 slot)

{
  GmoDlCache *cache;
  u32 words;
  u32 bytes;
  u32 *out;
  u32 *end;
  u32 *dst;
  u32 *src;
  u32 *base;
  u32 *stop;
  s32 i;
  size_t n;

  if (model == NULL) {
    return 0;
  }
  cache = (GmoDlCache *)model->dlCache;
  if (cache == NULL) {
    return 0;
  }
  words = cache->listWords;
  bytes = words * 4 + 0xc;
  if (buf == NULL) {
    return bytes;
  }
  out = *buf;
  if (out == NULL) {
    return 0;
  }
  if (size != NULL) {
    if (*size < bytes) {
      return 0;
    }
    *size = *size - bytes;
  }
  end = (u32 *)((u8 *)out + bytes);
  /* The asm creates the whole 64-byte lines strictly inside the buffer with `cache 0x18`
     (cached addresses only): a hint with no C effect, dropped. */
  dst = (u32 *)(((uintptr_t)out + 0xf) & ~(uintptr_t)0xf);
  out[2] = 0;
  out[1] = 0;
  out[0] = 0;
  end[-1] = 0;
  end[-2] = 0;
  end[-3] = 0;
  if (slot >= 0 && slot < (s32)cache->copyCount) {
    src = cache->lists + slot * cache->copyWords;
    if (src != NULL) {
      n = words << 2;
      if (n >= 0x40) {
        /* lv.q/sv.q C000-C030 block copy, 64 bytes per pass */
        base = src;
        stop = (u32 *)((u8 *)src + (n & ~(size_t)0x3f));
        do {
          for (i = 0; i < 0x10; i++) {
            dst[i] = src[i];
          }
          src += 0x10;
          dst += 0x10;
        } while (src != stop);
        /* cache 0x1b on every copied source line */
        PlatformDcacheWriteback(base, (unsigned)((u8 *)stop - (u8 *)base));
        n &= 0x3f;
      }
      memcpy(dst, src, n);
      *buf = end;
      return bytes;
    }
    slot = 0;
  }
  GmoDlCachePatch(dst, cache->lists, model, slot);
  *buf = end;
  return bytes;
}
