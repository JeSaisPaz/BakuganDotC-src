// bdc 0x0893aaa8 UiUnlockResultSetRewardName
#include "bdc.h"

/* Sets the reward name text of the unlock-result screen (task 375, `UiUnlockResultCtor`; reward
   kind `rewardKind`: 1 card, 2 hologram, 6/9 metal figure, 8 special unlock, others Maxus parts):
   picks the text table by reward kind (`"DWCardName"` 1, `"DWHologramName"` 2,
   `"DWMetalFigureName"` 6 and 9, `"DWSpecialUnlock"` 8; other kinds return without a name), loads it
   (`SaveFindLocalizedBin`, `UiMesTableRelocate`) and copies entry `rewardIndex` (kind 9: entry
   `UiUnlockResultMapFigureIndex`(table 0, `rewardIndex + 1`)) to `texts[0]`. Then clears
   `printers[0]`, prints the text at sprite 1's position (Y + 4) through the printer's print method,
   measures it (`UiTextMeasure`) into `textWidth`/`textHeight`/`textLines`, takes over the glyph
   list into `textGlyphs[0]` and the glyph count into `textReveal[0]`, and moves every glyph up by
   12. */

typedef void (*UiTextPrintFn)(float x, float y, float z, void *printer, char *text, s32 a, s32 b,
                              s32 c);

void UiUnlockResultSetRewardName(UiUnlockResult *self)
{
  char name[64];
  u32 *table;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  GfxSprite *anchor;
  GfxSprite *glyph;
  char *dst;
  u8 index;
  float count;
  int n;

  switch (self->rewardKind) {
  case 0:
    return;
  case 1:
    sprintf(name, "DWCardName");
    break;
  case 2:
    sprintf(name, "DWHologramName");
    break;
  case 3:
  case 4:
  case 5:
  case 7:
    return;
  case 6:
    sprintf(name, "DWMetalFigureName");
    break;
  case 8:
    sprintf(name, "DWSpecialUnlock");
    break;
  case 9:
    sprintf(name, "DWMetalFigureName");
    break;
  default:
    return;
  }
  dst = self->texts[0];
  table = SaveFindLocalizedBin(name);
  UiMesTableRelocate(table);
  if (self->rewardKind == 9) {
    index = UiUnlockResultMapFigureIndex(self, 0, (u8)(self->rewardIndex + 1));
    strcpy(dst, ((char **)table)[index]);
  } else {
    strcpy(dst, ((char **)table)[self->rewardIndex]);
  }
  printer = self->printers[0];
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;
  printer = self->printers[0];
  entry = &printer->layer.vtbl[2];
  anchor = ((GfxSprite **)self->base.data)[1];
  ((UiTextPrintFn)entry->fn)(anchor->posX, anchor->posY + 4.0f, 0.0f, (u8 *)printer + entry->delta,
                             dst, 1, 0, 0);
  UiTextMeasure(0.0f, self->printers[0], dst, &self->textWidth[0], &self->textHeight[0],
                &self->textLines[0]);
  printer = self->printers[0];
  self->textGlyphs[0] = printer->glyphs;
  glyph = self->textGlyphs[0];
  count = (float)printer->glyphCount;
  n = 0;
  self->textReveal[0] = count;
  if (0.0f < count) {
    do {
      n++;
      glyph->posY = glyph->posY - 12.0f;
      glyph = glyph->next;
    } while ((float)n < self->textReveal[0]);
  }
}
