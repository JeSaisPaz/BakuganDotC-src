// bdc 0x089fec10 UiWindowFrameApplyStyle
#include "bdc.h"

/* Applies a style record (0xa0 bytes) to an existing a 9-slice window frame (`UiWindowFrame`, 0x130
   bytes: a `GfxSpriteLayer` with vtable `0x08af5954` at `+0x74`, plus a
   `CoreObject` at `+0x80`): border colour (`style+0x30`), fill colour (`+0x80`), user word,
   border texture (name at `+0`, slot `+0x20`, `UiWindowFrameSetBorderTexture`), fill texture
   (name `+0x40`, slot `+0x60`, `UiWindowFrameSetFillTexture`), colour/offset vectors (`+0x70`,
   `+0x90`) and `+0x64`. */

static void UiWindowFrameCopyVec(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

void UiWindowFrameApplyStyle(UiWindowFrame *self, char *style, u32 arg)

{
  const UiWindowFrameStyle *st = (const UiWindowFrameStyle *)style;
  const VtblEntry *vt;

  UiWindowFrameCopyVec(self->borderColor, st->borderColor);
  UiWindowFrameCopyVec(self->fillColor, st->fillColor);
  *(u32 *)self->userWord = arg;
  UiWindowFrameSetBorderTexture(self, GfxFindTexture(st->borderName), st->borderSlot);
  UiWindowFrameSetFillTexture(self, GfxFindTexture(st->fillName), st->fillSlot);
  UiWindowFrameCopyVec(self->fillParams, st->fillParams);
  self->fillMode = st->fillMode;
  UiWindowFrameCopyVec(self->fillInset, st->fillInset);
  vt = &((GfxSpriteLayer *)self)->vtbl[7];
  ((void (*)(void *))vt->fn)((u8 *)self + vt->delta);
}
