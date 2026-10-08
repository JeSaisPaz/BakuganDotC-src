// bdc 0x08a193d0 GmoDlCacheEmitCall
#include "bdc.h"

/* Multi-buffer path of `GmoModelBuildCachedDl`: when `slot` lies in the dirty copy range
   (`dirtyFirst..dirtyEnd`) writes that range back (`sceKernelDcacheWritebackRange`) and clears it,
   then emits a 2-word GE `BASE` + `CALL` (`0x10`, `0x0a`) to copy `slot` into `*buf`, charging 4
   to `*size` (when given). Returns 4; also 4 when `buf` is NULL (size query); 0 on failure. */

u32 GmoDlCacheEmitCall(GmoModel *model, u32 **buf, u32 *size, s32 slot)
{
  GmoDlCache *cache;
  s32 first;
  u32 *list;
  u32 *dl;
  u32 addr;

  if (model == NULL) {
    return 0;
  }
  cache = model->dlCache;
  if (cache == NULL) {
    return 0;
  }
  if (buf == NULL) {
    return 4;
  }
  if (*buf == NULL) {
    return 0;
  }
  if (size != NULL) {
    if (*size < 4) {
      return 0;
    }
    *size -= 4;
  }

  first = cache->dirtyFirst;
  if (first <= slot && slot < cache->dirtyEnd) {
    if (first < 0 || first >= cache->copyCount) {
      list = NULL;
    } else {
      list = cache->lists + first * cache->copyWords;
    }
    sceKernelDcacheWritebackRange(list, cache->copyWords * (cache->dirtyEnd - first) * 4);
    cache->dirtyFirst = 0;
    cache->dirtyEnd = 0;
  }

  if (slot < 0 || slot >= cache->copyCount) {
    return 0;
  }
  list = cache->lists + slot * cache->copyWords;
  if (list == NULL) {
    return 0;
  }
  addr = PspAddr(list);
  dl = *buf;
  *buf = dl + 2;
  dl[0] = (addr >> 24) << 16 | 0x10000000;
  dl[1] = (addr & 0xffffff) | 0x0a000000;
  return 4;
}
