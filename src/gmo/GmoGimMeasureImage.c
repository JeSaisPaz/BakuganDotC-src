// bdc 0x08a268d4 GmoGimMeasureImage
#include "bdc.h"

/* Measure pass for one GIM image/palette block: reserves the image storage by passing the block
   header's geometry to `GmoImageMeasure`. */

void GmoGimMeasureImage(void *arena, const void *block, u32 kind)

{
  const GmoGimImageHeader *img =
      (const GmoGimImageHeader *)((const u8 *)block + ((const GmoGimBlock *)block)->dataOffset);

  GmoImageMeasure(NULL, img->format, img->order, img->width, img->height, img->alignW, img->alignH,
                  img->levels, img->frames, img->levelType, img->frameType, 1, kind,
                  (int)(img->dataEnd - img->headerSize), 0, *(void **)arena);
}
