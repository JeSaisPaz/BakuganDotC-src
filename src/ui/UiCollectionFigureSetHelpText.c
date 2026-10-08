// bdc 0x0898e45c UiCollectionFigureSetHelpText
#include "bdc.h"

/* Builds the description text of the opened entry of `UiCollectionFigure`:
   maps the entry id (`entryIds[page * 6 + cursor]`) through `g_uiCollectionFigureHelpIndex` to a
   message of `"DWCollectionHelp"` (`SaveFindLocalizedBin`/`UiMesTableRelocate`, entries from
   index 0x2c), copies it to `helpText`, lays it out in `helpPrinter` at the panel sprite's position
   (sprite 0x43), measures it (`UiTextMeasure` → `textW`/`textH`/`lineCount`) and shifts the
   glyphs up by half the largest glyph Y offset (`textLen` holds the glyph count). */

void UiCollectionFigureSetHelpText(UiCollectionFigure *self)
{
  u8 helpIndex[24];
  u8 msg;
  u32 *table;
  char *dst;
  UiTextPrinter *printer;
  GfxSprite *anchor;
  const VtblEntry *entry;
  GfxSprite *first;
  GfxSprite *glyph;
  float count;
  int maxOffset;
  int offset;
  int i;

  memcpy(helpIndex, g_uiCollectionFigureHelpIndex, 0x15);
  msg = helpIndex[self->entryIds[self->cursor + self->page * 6]];
  table = SaveFindLocalizedBin("DWCollectionHelp");
  UiMesTableRelocate(table);
  dst = self->helpText;
  strcpy(dst, (const char *)PspPtr(table[msg + 0x2c]));
  printer = self->helpPrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;

  printer = self->helpPrinter;
  anchor = ((GfxSprite **)self->base.data)[0x43];
  entry = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
      anchor->posX, anchor->posY, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);
  UiTextMeasure(0.0f, self->helpPrinter, dst, &self->textW, &self->textH, &self->lineCount);

  printer = self->helpPrinter;
  self->glyphs = printer->glyphs;
  count = (float)printer->glyphCount;
  first = self->glyphs;
  maxOffset = 0;
  i = 0;
  self->textLen = count;
  if (0.0f < count) {
    glyph = first;
    do {
      offset = (int)(glyph->posY - first->posY);
      if (maxOffset < offset) {
        maxOffset = offset;
      }
      i++;
      glyph = glyph->next;
    } while ((float)i < count);
  }
  i = 0;
  if (0.0f < count) {
    glyph = first;
    do {
      i++;
      glyph->posY = glyph->posY - (float)(maxOffset / 2);
      glyph = glyph->next;
    } while ((float)i < self->textLen);
  }
}
