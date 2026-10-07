// bdc 0x08a19550 GmoModelBuildCachedDl
#include "bdc.h"

/* Display-list path for models with a prebuilt list (`dlCache`, see `GmoModelBuildDl`): with more
   than one cached copy (`copyCount`) it rotates to the next copy (`curCopy`), patches it
   (`GmoDlCachePatch`), widens the dirty range and emits a call to it (`GmoDlCacheEmitCall`);
   with a single copy it inlines the list (`GmoDlCacheEmitInline`). Returns the callee's byte
   count, or 0 for a NULL model or a model without a cache. */

u32 GmoModelBuildCachedDl(GmoModel *self, u32 **buf, u32 *size)

{
  GmoDlCache *cache;
  u32 count;
  s16 slot;
  s16 emit;
  u32 *dst;

  if (self == NULL) {
    return 0;
  }
  cache = (GmoDlCache *)self->dlCache;
  if (cache == NULL) {
    return 0;
  }
  count = cache->copyCount;
  if (count < 2) {
    return GmoDlCacheEmitInline(self, buf, size, -1);
  }
  slot = (s16)((cache->curCopy + 1) % (s32)count);
  cache->curCopy = slot;
  emit = slot;
  if (slot >= 0 && slot < (s32)count) {
    dst = cache->lists + slot * cache->copyWords;
    if (dst != NULL) {
      GmoDlCachePatch(dst, NULL, self, slot);
      if (slot < cache->dirtyFirst) {
        cache->dirtyFirst = slot;
      }
      emit = cache->curCopy;
      if (cache->dirtyEnd < slot + 1) {
        cache->dirtyEnd = (s16)(slot + 1);
      }
    }
  }
  return GmoDlCacheEmitCall(self, buf, size, emit);
}
