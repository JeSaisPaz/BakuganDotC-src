// bdc 0x08980358 UiCollectionSphereSetHelpText
#include "bdc.h"

/* Builds the description text of the selected entry of `UiCollectionSphere`:
   in categories 0/1 maps `entryIds[page * 6 + cursor]` through `g_uiCollectionSphereHelpIndex`;
   in category 2 the page kind (`UiCollectionSphereGetPageKind`) picks `entryIds[(page / 3) * 6 +
   cursor]` through `g_uiCollectionSphereHelpIndexCat2` (model pages, kind 0/0xff) or
   `kind1Ids`/`kind2Ids[page / 3]` through `g_uiCollectionSphereHelpIndex`; any other category or
   kind uses message 0. Copies that `"DWCollectionHelp"` message (`SaveFindLocalizedBin`/
   `UiMesTableRelocate`) to `helpText`, lays it out in `helpPrinter` at sprite 69's position,
   measures it (`UiTextMeasure` → `helpWidth`/`helpTextH`/`helpLines`) and shifts the glyphs up by
   half the largest glyph Y offset (`helpHeight` holds the glyph count). */

void UiCollectionSphereSetHelpText(UiCollectionSphere *self)
{
  u8 helpIndex[36];
  u8 helpIndexCat2[16];
  u8 msg;
  u8 kind;
  s8 category;
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

  msg = 0;
  memcpy(helpIndex, g_uiCollectionSphereHelpIndex, 0x21);
  memcpy(helpIndexCat2, g_uiCollectionSphereHelpIndexCat2, 0xd);
  category = self->category;
  if (category < 2) {
    if (category >= 0) {
      msg = helpIndex[self->entryIds[self->cursor + self->page * 6]];
    }
  } else if (category < 3) {
    kind = UiCollectionSphereGetPageKind(self, (u8)self->page);
    if (kind == 0xff || kind == 0) {
      msg = helpIndexCat2[self->entryIds[self->cursor + (self->page / 3) * 6]];
    } else if (kind == 1) {
      msg = helpIndex[self->kind1Ids[self->page / 3]];
    } else if (kind == 2) {
      msg = helpIndex[self->kind2Ids[self->page / 3]];
    }
  }
  table = SaveFindLocalizedBin("DWCollectionHelp");
  UiMesTableRelocate(table);
  dst = self->helpText;
  strcpy(dst, (const char *)PspPtr(table[msg]));
  printer = self->helpPrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;

  printer = self->helpPrinter;
  anchor = ((GfxSprite **)self->base.data)[69];
  entry = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
      anchor->posX, anchor->posY, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);
  UiTextMeasure(0.0f, self->helpPrinter, dst, &self->helpWidth, &self->helpTextH, &self->helpLines);

  printer = self->helpPrinter;
  self->helpGlyphs = printer->glyphs;
  count = (float)printer->glyphCount;
  first = self->helpGlyphs;
  maxOffset = 0;
  i = 0;
  self->helpHeight = count;
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
    } while ((float)i < self->helpHeight);
  }
}
