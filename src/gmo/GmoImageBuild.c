// bdc 0x08a12064 GmoImageBuild
#include "bdc.h"

/* (Re)creates an image record's storage: releases the previous level buffers, level table and user
   data, stores the geometry, carves the `levels * frames` level table and `auxSize` bytes of user
   data from `plan` (`GmoImagePlanTakeArray`), then gives every level of every frame a buffer of
   `GmoImageLevelSize` bytes, either consecutive slices of `pixels` or fresh blocks from `plan`
   (`GmoImagePlanTake`). Returns 1. */

int GmoImageBuild(GmoImage *self, u32 fmt, u8 flags16, int width, int height, int alignBytes,
                  int heightAlign, int levels, int frames, int mipmapMode, int kind, int pool,
                  u32 align, int auxSize, void *pixels, void *plan)

{
  int count;
  int i;
  int bpp;
  int rowUnits;
  int frame;
  int level;
  int size;
  void *buf;
  u8 *next;

  count = (int)self->levelCount * (int)self->frameCount;
  if (count > 0) {
    i = 0;
    do {
      GmoImageHeapReleaseThunk(1, self->levels[i]);
      i++;
    } while (count != i);
  }
  frame = 0;
  GmoImageHeapReleaseThunk(0, self->levels);
  GmoImageHeapReleaseThunk(0, self->userData);
  bpp = GmoImageFormatBits(fmt);
  self->flags16 = flags16;
  self->bpp = (u8)bpp;
  self->height = (u16)height;
  self->heightAlign = (u8)heightAlign;
  self->format = (u16)fmt;
  self->width = (u16)width;
  self->widthAlign = (u8)((alignBytes << 3) / bpp);
  rowUnits = levels * ((bpp * width + 0xff) / 0x100);
  if (rowUnits > 0x20) {
    rowUnits = 0x20;
  }
  self->flags1a = (u8)rowUnits;
  self->levels = (void **)GmoImagePlanTakeArray(plan, 0, (u32)__alignof__(void *), (int)sizeof(void *), levels * frames, NULL);
  self->kind29 = (u8)kind;
  self->levelCount = (u16)levels;
  self->mipmapMode = (u8)mipmapMode;
  self->frameCount = (u16)frames;
  self->aux2a = (u16)auxSize;
  self->userData = GmoImagePlanTakeArray(plan, 0, 0x10, 1, auxSize, NULL);
  if (frames > 0) {
    next = (u8 *)pixels;
    do {
      if (levels > 0) {
        level = 0;
        do {
          size = GmoImageLevelSize(bpp, width, height, alignBytes, heightAlign,
                                   (mipmapMode == 1) ? level : 0);
          if (next == NULL) {
            buf = GmoImagePlanTake(plan, pool, align, size);
          }
          else {
            buf = next;
            next = next + size;
          }
          self->levels[frame * levels + level] = buf;
          level++;
        } while (levels != level);
      }
      frame++;
    } while (frames != frame);
  }
  return 1;
}
