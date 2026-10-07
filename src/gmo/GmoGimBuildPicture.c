// bdc 0x08a26450 GmoGimBuildPicture
#include "bdc.h"

/* Build pass for a GIM picture block: builds the texture's image (`images`, from the image block
   type 4, `GmoGimBuildImage` kind 0x80) and palette (`palettes`, from block type 5, kind 0x10),
   allocates the animation tracks (`tracks`/`trackCount`: one GmoTexTrack per sequence block 6
   plus one, none for a still picture without sequences) and fills them from the sequences
   (key/time tables and sequence data copied into pool 0), then computes the sampling flags
   (`GmoTextureComputeScale`) and, unless the image or palette has several frames, emits the GE
   command list (`GmoTextureWriteDl` into 0x80 pool-1 bytes, RET-terminated) and makes it the
   current image. */

void GmoGimBuildPicture(void *arena, const void *picture, void *img)
{
  GmoTexture *tex = (GmoTexture *)img;
  const GmoGimBlock *imageBlock;
  const GmoGimBlock *paletteBlock;
  const GmoGimBlock *block;
  const GmoGimBlock *end;
  const GmoGimSequenceHeader *seq;
  const u16 *srcKey;
  const GmoGimTimeEntry *srcTime;
  GmoImage *image;
  GmoTexTrack *track;
  u16 *keys;
  GmoTexChannel *frames;
  u8 *data;
  void *list;
  u32 *dl;
  u32 dlSize;
  s32 count;
  int animated;
  int keyCount;
  int frameCount;
  u32 dataSize;
  int i;

  imageBlock = GmoGimFindChild(picture, 4, 0);
  paletteBlock = GmoGimFindChild(picture, 5, 0);
  if (imageBlock == NULL) {
    animated = 0;
  } else {
    animated = 1;
    image = GmoImagePlanTakePalettes(1, *(void **)arena);
    tex->images = image;
    GmoGimBuildImage(arena, imageBlock, image, 0x80);
    if (image->frameCount < 2) {
      animated = 0;
    }
  }
  if (paletteBlock != NULL) {
    image = GmoImagePlanTakePalettesThunk(1, *(void **)arena);
    tex->palettes = image;
    GmoGimBuildImage(arena, paletteBlock, image, 0x10);
    if (!(image->frameCount < 2)) {
      animated = 1;
    }
  }

  count = GmoGimCountChildren(picture, 6);
  track = GmoImagePlanTakeImages((count != 0 || animated != 0) ? count + 1 : 0,
                                 *(void **)arena);
  tex->trackCount = (u8)count;
  tex->tracks = track;

  end = (const GmoGimBlock *)((const u8 *)picture + ((const GmoGimBlock *)picture)->size);
  for (block = (const GmoGimBlock *)((const u8 *)picture +
                                     ((const GmoGimBlock *)picture)->firstChild);
       block < end; block = (const GmoGimBlock *)((const u8 *)block + block->size)) {
    if (block->type != 6) {
      continue;
    }
    seq = (const GmoGimSequenceHeader *)((const u8 *)block + block->dataOffset);
    keyCount = seq->frameCount;
    frameCount = seq->timeCount;
    track->mode = (u8)seq->mode;
    track->keyCount = (u8)keyCount;
    track->frameCount = (u8)frameCount;
    dataSize = seq->dataEnd - seq->dataStart;
    track->startFrame = (float)seq->startFrame;
    track->endFrame = (float)seq->endFrame;
    track->frameRate = (float)seq->frameRate;
    keys = GmoImagePlanTake(*(void **)arena, 0, 4, keyCount * 4);
    frames = GmoImagePlanTake(*(void **)arena, 0, 4, frameCount * 8 + dataSize);
    data = (u8 *)frames + frameCount * 8;
    track->frames = frames;
    track->keys = keys;
    memcpy(data, (const u8 *)seq + seq->dataStart, dataSize);

    srcKey = (const u16 *)((const u8 *)seq + seq->frameTable);
    for (i = 0; i < keyCount; i++) {
      keys[0] = srcKey[0];
      keys[1] = srcKey[1];
      srcKey += 2;
      keys += 2;
    }

    srcTime = (const GmoGimTimeEntry *)((const u8 *)seq + seq->timeTable);
    for (i = 0; i < frameCount; i++) {
      frames->kind = srcTime->kind;
      frames->seq = (short *)(data + (srcTime->offset - seq->dataStart));
      frames->flags = srcTime->flags;
      srcTime++;
      frames++;
    }
    track++;
  }

  GmoTextureComputeScale(tex);
  if (animated == 0) {
    dl = GmoImagePlanTake(*(void **)arena, 1, 4, 0x80);
    tex->palette = dl;
    dlSize = 0x80;
    GmoTextureWriteDl(tex, &dl, &dlSize, 0x80000001);
    list = tex->palette;
    *dl++ = 0x0B000000; /* GE RET */
    GmoTextureSetImage(tex, list);
  }
}
