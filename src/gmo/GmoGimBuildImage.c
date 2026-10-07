// bdc 0x08a260ac GmoGimBuildImage
#include "bdc.h"

/* Builds an image record from a GIM image (type 4, `kind` 0x80) or palette (type 5, `kind` 0x10)
   block: creates the image from the block header (`GmoImageBuild`: format, pixel order, width,
   height, alignment, levels, frames, level/frame type, user-data size) and copies every
   level × frame into it with `GmoImageSwizzle` (source found through the GIM offset table), then
   copies the user data (NULL source when the header has none). */

void GmoGimBuildImage(void *arena, const void *block, void *tex, u32 kind)

{
  const GmoGimImageHeader *hdr =
      (const GmoGimImageHeader *)((const u8 *)block + ((const GmoGimBlock *)block)->dataOffset);
  GmoImage *img = (GmoImage *)tex;
  const u32 *offsets;
  const void *userData;
  int level;
  int frame;
  int i;
  int idx;
  s32 pitch;
  u32 rows;
  int w;
  int h;
  int mask;
  int srcPitch;
  int srcRows;
  u32 off;
  void *dst;

  GmoImageBuild(img, hdr->format, (u8)hdr->order, hdr->width, hdr->height, hdr->alignW,
                hdr->alignH, hdr->levels, hdr->frames, hdr->levelType, hdr->frameType, 1, kind,
                (int)(hdr->dataEnd - hdr->headerSize), NULL, *(void **)arena);

  for (level = 0; level < hdr->levels; level++) {
    pitch = GmoImageGetPitchBytes(img, GmoImageGetLevelWidth(img, level));
    rows = GmoImageAlignHeight(img, GmoImageGetLevelHeight(img, level));

    /* source row size in bytes, rounded up to the GIM pitch alignment */
    w = hdr->width;
    if (level > 0 && hdr->levelType == 1) {
      for (i = 0; i != level; i++) {
        w = (w + 1) / 2;
      }
    }
    mask = hdr->alignW * 8 - 1;
    srcPitch = ((w * hdr->bpp + mask) & ~mask) / 8;

    h = hdr->height;
    if (level > 0 && hdr->levelType == 1) {
      for (i = 0; i != level; i++) {
        h = (h + 1) / 2;
      }
    }
    srcRows = (h + (hdr->alignH - 1)) & ~(hdr->alignH - 1);

    for (frame = 0; frame < hdr->frames; frame++) {
      dst = GmoImageGetLevelData(img, level, frame);
      off = 0;
      if (level >= 0 && level < hdr->levels) {
        offsets = (const u32 *)((const u8 *)hdr + hdr->dataEnd);
        idx = frame % (int)hdr->frames;
        if (idx < 0) {
          idx += hdr->frames;
        }
        off = offsets[level + idx * hdr->levels];
      }
      GmoImageSwizzle(dst, pitch, rows, img->flags16, (const u8 *)hdr + off, srcPitch, srcRows,
                      hdr->order);
    }
  }

  userData = (hdr->headerSize < hdr->dataEnd) ? (const void *)((const u8 *)hdr + hdr->headerSize)
                                              : NULL;
  memcpy(img->userData, userData, img->aux2a);
}
