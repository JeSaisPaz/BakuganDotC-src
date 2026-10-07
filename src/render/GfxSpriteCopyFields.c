// bdc 0x089f3bbc GfxSpriteCopyFields
#include "bdc.h"

/* Copies the drawable state of one `GfxSprite` into another: the list node words
   (`+0x00..0x13`), everything from `matrix` to `uvScaleV` (`+0x20..0x143`) and `toCamera`
   (`+0x150..0x15f`), skipping the vtable and pad words at `+0x14..0x1f` and the pad at
   `+0x144..0x14f`. The 16-byte blocks are moved with lv.q/sv.q. Returns `dst`. */

GfxSprite *GfxSpriteCopyFields(GfxSprite *dst, const GfxSprite *src)
{
  int i;

  dst->prev = src->prev;
  dst->next = src->next;
  dst->poolIndex = src->poolIndex;
  dst->serial = src->serial;
  dst->list = src->list;
  for (i = 0; i < 16; i++) {
    dst->matrix[i] = src->matrix[i];
  }
  dst->posX = src->posX;
  dst->posY = src->posY;
  dst->posZ = src->posZ;
  dst->posW = src->posW;
  dst->width = src->width;
  dst->height = src->height;
  dst->depth = src->depth;
  dst->maybe_sizeW = src->maybe_sizeW;
  for (i = 0; i < 4; i++) {
    dst->maybe_billboardParams80[i] = src->maybe_billboardParams80[i];
  }
  dst->scaleX = src->scaleX;
  dst->scaleY = src->scaleY;
  dst->scaleZ = src->scaleZ;
  dst->angle = src->angle;
  for (i = 0; i < 4; i++) {
    dst->maybe_billboardParamsA0[i] = src->maybe_billboardParamsA0[i];
  }
  dst->tint[0] = src->tint[0];
  dst->tint[1] = src->tint[1];
  dst->tint[2] = src->tint[2];
  dst->alpha = src->alpha;
  for (i = 0; i < 4; i++) {
    dst->addColor[i] = src->addColor[i];
  }
  dst->flags = src->flags;
  dst->texture = src->texture;
  dst->textureSlot = src->textureSlot;
  dst->blendMode = src->blendMode;
  dst->billboardMode = src->billboardMode;
  dst->quadMode = src->quadMode;
  dst->alphaRef = src->alphaRef;
  dst->preDrawCallback = src->preDrawCallback;
  for (i = 0; i < 4; i++) {
    dst->vertexBuf[i] = src->vertexBuf[i];
  }
  dst->vertices = src->vertices;
  dst->slotFlags = src->slotFlags;
  dst->layerMask = src->layerMask;
  dst->geStencilTest = src->geStencilTest;
  dst->geStencilOp = src->geStencilOp;
  dst->uvOffsetU = src->uvOffsetU;
  dst->uvOffsetV = src->uvOffsetV;
  dst->uvScaleU = src->uvScaleU;
  dst->uvScaleV = src->uvScaleV;
  for (i = 0; i < 4; i++) {
    dst->toCamera[i] = src->toCamera[i];
  }
  return dst;
}
