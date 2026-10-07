// bdc 0x08816598 UiMsgWindowCtor
#include "bdc.h"

/* Constructor of the 0x48-byte message window `g_uiMsgWindow`: allocates its 0x800-byte text
   buffer (`+0xc`), sets the text area to 240×136 (`UiMsgWindowSetTextPos`), clears the text, sets state
   `+0x0 = 999` (closed), depth `+0x4 = 1900.0`, choice `+0x20 = -1`, and builds its parts: a
   `UiTextBoxCtor` text box at `+0x2c`, a 0x50-byte object `GfxRectCtor(…, 1)` at `+0x30`, a
   sprite pool (`GfxSpriteLayerCtor`, `+0x38`) with an 11-sprite frame (`UiLayoutCreateSprites`, array at `+0x3c`,
   all hidden) whose sprite 8 gets the `co_bo_ita_1_01` board texture. Returns `self`. */

UiMsgWindow *UiMsgWindowCtor(UiMsgWindow *self)
{
  bool fromLow;
  char *text;
  UiTextBox *box;
  void *rect;
  GfxSpriteLayer *layer;
  GfxSprite **sprites;
  GfxSprite *board;
  int i;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  text = MemAlloc(0x800, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->text = text;
  UiMsgWindowSetTextPos(self, 240, 136);
  UiMsgWindowSetText(self, NULL);
  self->depth = 1900.0f;
  self->delay = 0;
  self->state = 999;
  self->alpha = 0.0f;
  self->closeRequested = 0;
  self->unk1d = 0;
  self->choice = -1;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  box = MemAlloc(0x10, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (box != NULL) {
    UiTextBoxCtor(box);
  }
  self->textBox = box;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  rect = MemAlloc(0x50, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (rect != NULL) {
    GfxRectCtor(rect, 1);
  }
  self->rect = rect;
  self->pad = NULL;
  self->mode = 0;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  layer = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (layer != NULL) {
    GfxSpriteLayerCtor(layer, 0);
  }
  self->spriteLayer = layer;
  layer->sorted = 1;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprites = MemAlloc(11 * sizeof(GfxSprite *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->sprites = sprites;
  UiLayoutCreateSprites(self->spriteLayer, sprites, 9);
  for (i = 0; i < 11; i++) {
    self->sprites[i]->flags &= ~1u;
  }
  board = self->sprites[8];
  board->texture = GfxFindTexture("co_bo_ita_1_01");
  self->hasChoice = 0;
  return self;
}
