// bdc 0x08a24f2c GmoImageCopy
#include "bdc.h"

/* Build pass for copying one image record: re-initialises `dst` (`GmoImageDestroyContents`); when
   `flags & (kindFlag | 1)` it re-creates the image with the same geometry (`GmoImageBuild`) and
   copies every level × frame with `GmoImageSwizzle` (per-level sizes from
   `GmoImageGetLevelWidth`/`GmoImageGetLevelHeight`, pitch `GmoImageGetPitchBytes`, aligned
   height `GmoImageAlignHeight`, data `GmoImageGetLevelData`) plus the user data block and
   flushes the data cache; otherwise it copies the whole record except `refCount`, clears `next`
   and shares the level table, user data and level buffers (`GmoImageHeapAddRef`).
   Returns `dst`, unchanged when `dst == src`, or NULL when `dst`, `src` or `arena` is NULL. */

GmoImage *GmoImageCopy(GmoImage *dst, const GmoImage *src, u32 flags, void *arena, u32 kindFlag, u32 gpuFlag)
{
  s16 refCount;
  s32 alignBytes;
  s32 dstPitch, srcPitch;
  u32 dstHeight, srcHeight;
  void *dstData, *srcData;
  s32 level, frame, count, i;

  if (dst == NULL || src == NULL || arena == NULL) {
    return NULL;
  }
  if (dst == src) {
    return dst;
  }
  if (((kindFlag | 1) & flags) == 0) {
    flags = 0;
  }
  GmoImageDestroyContents(dst);
  if (flags != 0) {
    dst->id = src->id;
    dst->frameMask = src->frameMask;
    dst->auxFlags = src->auxFlags;
    alignBytes = GmoImageGetAlignBytes(src);
    GmoImageBuild(dst, src->format, src->flags16, src->width, src->height, alignBytes,
                  src->heightAlign, src->levelCount, src->frameCount, src->mipmapMode,
                  src->kind29, ((s32)flags < 0) ? 2 : 1, gpuFlag, src->aux2a, NULL, arena);
    for (level = 0; level < src->levelCount; level++) {
      dstPitch = GmoImageGetPitchBytes(dst, GmoImageGetLevelWidth(dst, level));
      dstHeight = GmoImageAlignHeight(dst, GmoImageGetLevelHeight(dst, level));
      srcPitch = GmoImageGetPitchBytes(src, GmoImageGetLevelWidth(src, level));
      srcHeight = GmoImageAlignHeight(src, GmoImageGetLevelHeight(src, level));
      for (frame = 0; frame < src->frameCount; frame++) {
        dstData = GmoImageGetLevelData(dst, level, frame);
        srcData = GmoImageGetLevelData(src, level, frame);
        GmoImageSwizzle(dstData, dstPitch, dstHeight, dst->flags16, srcData, srcPitch, srcHeight,
                        src->flags16);
      }
    }
    memcpy(dst->userData, src->userData, dst->aux2a);
    sceKernelDcacheWritebackAll();
    return dst;
  }
  refCount = dst->refCount;
  *dst = *src;
  dst->refCount = refCount;
  dst->next = NULL;
  GmoImageHeapAddRef(0, dst->levels);
  GmoImageHeapAddRef(0, dst->userData);
  count = dst->levelCount * dst->frameCount;
  for (i = 0; i < count; i++) {
    GmoImageHeapAddRef(1, dst->levels[i]);
  }
  return dst;
}
