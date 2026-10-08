// bdc 0x0892989c UiHologramViewSetHelpText
#include "bdc.h"

/* Sets the help text of the hologram view screen (task 392, `UiHologramViewCtor`). With `hide`
   set it only resets the fade step and sets the text alpha to 1. Otherwise it plays the page voice
   (`UiHologramViewPlayVoice`), copies entry `page` of the `"DWHologramHelp"` text table to
   `text`, prints it with the printer 16 px right of sprite `sprite`, measures it
   (`UiTextMeasure` into `textW`/`textH`/`textLines`), keeps the glyph list and count, raises
   the glyphs by half the text height plus 6 px and resets the fade step. */

void UiHologramViewSetHelpText(UiHologramView *self, s8 hide, u16 sprite)
{
  u32 *table;
  char *dst;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  GfxSprite *anchor;
  GfxSprite *first;
  GfxSprite *glyph;
  float count;
  int maxOffset;
  int offset;
  int i;

  if (hide == 0) {
    UiHologramViewPlayVoice(self, self->page & 0xff);
    table = SaveFindLocalizedBin("DWHologramHelp");
    UiMesTableRelocate(table);
    dst = self->text;
    strcpy(dst, (const char *)PspPtr(table[self->page]));
    printer = self->printer;
    GfxSpriteLayerClear(&printer->layer);
    printer->glyphs = NULL;

    printer = self->printer;
    anchor = ((GfxSprite **)self->base.data)[sprite];
    entry = &printer->layer.vtbl[2];
    ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
        anchor->posX + 16.0f, anchor->posY, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);
    UiTextMeasure(0.0f, self->printer, dst, &self->textW, &self->textH, &self->textLines);

    printer = self->printer;
    self->glyphs = printer->glyphs;
    count = (float)printer->glyphCount;
    first = self->glyphs;
    maxOffset = 0;
    i = 0;
    self->glyphCount = count;
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
        glyph->posY = glyph->posY - (float)(maxOffset / 2 + 6);
        glyph = glyph->next;
      } while ((float)i < self->glyphCount);
    }
    self->fadeT = 0.0f;
  } else {
    self->fadeT = 0.0f;
    self->textAlpha = 1.0f;
  }
}
