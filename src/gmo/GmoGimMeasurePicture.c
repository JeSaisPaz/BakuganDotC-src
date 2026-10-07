// bdc 0x08a26958 GmoGimMeasurePicture
#include "bdc.h"

/* Measure pass for a GIM picture block: reserves the records and storage of the pixel image
   (image block 4) and palette (block 5) (`GmoGimMeasureImage`); when neither has more than one
   frame, 0x80 bytes in pool 1 for the GE command list; per sequence block (6) its frame, time and
   data tables; finally `sequences + 1` image records (1 if animated without sequences, else 0). */

void GmoGimMeasurePicture(void *arena, const void *picture)

{
  const GmoGimBlock *image = GmoGimFindChild(picture, 4, 0);
  const GmoGimBlock *palette = GmoGimFindChild(picture, 5, 0);
  const GmoGimBlock *child;
  const GmoGimBlock *end;
  const GmoGimSequenceHeader *seq;
  int animated = 0;
  int sequences;
  u16 times;

  if (image != NULL) {
    GmoImagePlanReservePalettes(1, *(void **)arena);
    GmoGimMeasureImage(arena, image, 0x80);
    if (((const GmoGimImageHeader *)((const u8 *)image + image->dataOffset))->frames >= 2) {
      animated = 1;
    }
  }
  if (palette != NULL) {
    GmoImagePlanReserveTracks(1, *(void **)arena);
    GmoGimMeasureImage(arena, palette, 0x10);
    if (((const GmoGimImageHeader *)((const u8 *)palette + palette->dataOffset))->frames >= 2) {
      animated = 1;
    }
  }
  if (!animated) {
    GmoImagePlanReserve(*(void **)arena, 1, 4, 0x80);
  }

  end = (const GmoGimBlock *)((const u8 *)picture + ((const GmoGimBlock *)picture)->size);
  child = (const GmoGimBlock *)((const u8 *)picture + ((const GmoGimBlock *)picture)->firstChild);
  sequences = 0;
  for (; child < end; child = (const GmoGimBlock *)((const u8 *)child + child->size)) {
    if (child->type != 6) {
      continue;
    }
    seq = (const GmoGimSequenceHeader *)((const u8 *)child + child->dataOffset);
    sequences++;
    times = seq->timeCount;
    GmoImagePlanReserve(*(void **)arena, 0, 4, seq->frameCount * 4);
    GmoImagePlanReserve(*(void **)arena, 0, 4, times * 8);
    GmoImagePlanReserve(*(void **)arena, 0, 4, (int)(seq->dataEnd - seq->dataStart));
  }

  if (sequences != 0 || animated) {
    GmoImagePlanReserveImages(sequences + 1, *(void **)arena);
  } else {
    GmoImagePlanReserveImages(0, *(void **)arena);
  }
}
