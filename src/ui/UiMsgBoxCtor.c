// bdc 0x089ebcf8 UiMsgBoxCtor
#include "bdc.h"

/* Initialises the message box object (0x80 bytes): text slot cleared via `UiMsgBoxSetText`,
   extents (240.0, 136.0 pairs), state = 1, id = 999, the two default colours copied from
   `g_colorWhite`/`g_colorBlack`, a 0x10-byte text box helper `textBox` (`UiTextBoxCtor`),
   the shadow flag = 1 and the 0x50-byte choice highlight rect `highlight` (`GfxRectCtor`,
   `GfxRectSetSize`) coloured {0.5, 0.5, 0.5, 1}. Returns `self`. */

UiMsgBox *UiMsgBoxCtor(UiMsgBox *self)
{
  bool wasLow;
  UiTextBox *box;
  UiTextBox *textBox;
  GfxRect *rect;
  GfxRect *highlight;

  self->text = NULL;
  self->extents[0] = 240.0f;
  self->extents[1] = 136.0f;
  self->extents[2] = 240.0f;
  self->extents[3] = 136.0f;
  UiMsgBoxSetText(self, NULL);
  self->unk14 = 1900.0f;
  self->unk10 = 1900.0f;
  self->unk1c = 0;
  self->id = 999;
  self->unk78 = 0;
  self->unk59 = 0;
  self->choices = NULL;
  self->choice = -1;
  self->state = 1;
  self->unk64 = 0;
  self->unk70 = 0;

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  box = MemAlloc(sizeof(UiTextBox), NULL, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  textBox = NULL;
  if (box != NULL) {
    UiTextBoxCtor(box);
    textBox = box;
  }
  self->textBox = textBox;
  self->owner = NULL;
  self->colorA[0] = g_colorWhite.x;
  self->colorA[1] = g_colorWhite.y;
  self->colorA[2] = g_colorWhite.z;
  self->colorA[3] = g_colorWhite.w;
  self->colorB[0] = g_colorBlack.x;
  self->colorB[1] = g_colorBlack.y;
  self->colorB[2] = g_colorBlack.z;
  self->colorB[3] = g_colorBlack.w;
  self->shadow = 1;
  self->unk50 = 0;
  self->unk54 = 0; /* stored as float 0.0f */

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  rect = MemAlloc(sizeof(GfxRect), NULL, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  highlight = NULL;
  if (rect != NULL) {
    GfxRectCtor(rect, 0);
    highlight = rect;
  }
  self->highlight = highlight;
  GfxRectSetSize(highlight, 0, 0);
  highlight = self->highlight;
  highlight->color[0] = 0.5f;
  highlight->color[1] = 0.5f;
  highlight->color[2] = 0.5f;
  highlight->color[3] = 1.0f;
  self->unk04 = 0;
  return self;
}
