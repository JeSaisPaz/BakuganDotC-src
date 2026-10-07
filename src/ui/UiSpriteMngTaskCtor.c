// bdc 0x089ee1bc UiSpriteMngTaskCtor
#include "bdc.h"

/* Constructor of the 2D sprite manager task (task id 10090 / 0x276a, object size 0x24, vtable
   `g_uiSpriteMngVtable`) created by `CoreTaskNewById`: base `CoreTaskInit`, a 0x80-byte sprite
   layer at `layer` (+0x10, heap low end, `GfxSpriteLayerCtor` when the allocation succeeded, then a
   pool of 0x20 sprites via `GfxSpriteLayerInitPool`), a zeroed 0x80-byte slot array `sprites`
   (+0x14, 0x20 pointers), `visible` = 0, `count` = 0 and draw `depth` = 1000.0. Returns `self`. */

UiSpriteMng *UiSpriteMngTaskCtor(UiSpriteMng *self)
{
  bool fromLow;
  GfxSpriteLayer *mem;
  GfxSpriteLayer *layer;
  GfxSprite **sprites;

  CoreTaskInit(&self->base);
  self->base.vtable = g_uiSpriteMngVtable;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  layer = NULL;
  if (mem != NULL) {
    GfxSpriteLayerCtor(mem, 0);
    layer = mem;
  }
  self->layer = layer;
  GfxSpriteLayerInitPool(layer, 0x20);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprites = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->sprites = sprites;
  memset(sprites, 0, 0x80);

  self->visible = 0;
  self->count = 0;
  self->depth = 1000.0f;
  return self;
}
