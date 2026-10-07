// bdc 0x089fe5d4 UiWindowFrameCtor
#include "bdc.h"

/* Constructor of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `g_uiWindowFrameVtable` at `+0x74`, plus a `CoreObject`
   at `+0x80`): runs `GfxSpriteLayerCtor` (2D), `CoreObjectInit` at `+0x80` (vtable
   `g_uiWindowFrameObjectVtable`), installs the frame vtable, enables sorting, clears state (`+0x98..+0xac`),
   zeroes the position, fill inset and fill params vec4s (`+0xb0`, `+0xc0`, `+0xe0`; stored from the
   zero bank C720), clears the open state (`+0xf0..+0xf8`), sets alpha/brightness/progress
   `+0xfc..+0x104` to 1.0, zeroes the border and fill colours (`+0x110`, `+0x120`) and sets the
   highlight flag (`UiWindowFrameSetHighlight`). */

static void UiWindowFrameZeroVec(float *dst)
{
  dst[0] = 0.0f;
  dst[1] = 0.0f;
  dst[2] = 0.0f;
  dst[3] = 0.0f;
}

UiWindowFrame *UiWindowFrameCtor(UiWindowFrame *self)

{
  GfxSpriteLayerCtor((GfxSpriteLayer *)self, 0);
  CoreObjectInit(&self->object, (CoreObject *)0x0);
  ((GfxSpriteLayer *)self)->vtbl = g_uiWindowFrameVtable;
  self->object.vtable = g_uiWindowFrameObjectVtable;
  ((GfxSpriteLayer *)self)->sorted = 1;
  *(u32 *)self->unk98 = 0;
  self->flags = 0;
  *(u32 *)self->userWord = 0;
  self->borderTexture = NULL;
  self->fillTexture = NULL;
  self->fill = NULL;
  UiWindowFrameZeroVec(self->rect);
  UiWindowFrameZeroVec(self->fillInset);
  self->fillMode = 0;
  UiWindowFrameZeroVec(self->fillParams);
  self->depth = 0.0f;
  self->state = 0;
  self->step = 0.0f;
  self->alpha = 1.0f;
  self->brightness = 1.0f;
  self->progress = 1.0f;
  UiWindowFrameZeroVec(self->borderColor);
  UiWindowFrameZeroVec(self->fillColor);
  UiWindowFrameSetHighlight(self);
  return self;
}
