// bdc 0x0893c638 UiUnlockResultSetHelpText
#include "bdc.h"

/* Loads the description text for the reward of the unlock-result screen (task 375,
   `UiUnlockResultCtor`): picks a text table and entry by the reward kind (`"DWCardHelp"`,
   `"DWHologramHelp"`, `"DWCommon"`, `"DWMaxusPartsHelp"`, `"DWMetalFigureHelp"`,
   `"DWSpecialUnlock"`), copies the entry into `texts[1]` (kind 4 appends it after the number
   already written there), prints it with printer 1 at sprite 3 of the screen data, measures it,
   raises the glyphs by half the text height plus 6 px and shows text slot 1. Kind 0 or > 9
   returns without doing anything. */

void UiUnlockResultSetHelpText(UiUnlockResult *self)
{
  u32 *table;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  GfxSprite *anchor;
  GfxSprite *first;
  GfxSprite *glyph;
  char *dst;
  char *src;
  float count;
  int index;
  int maxOffset;
  int offset;
  int i;
  char name[64];
  char line[128];

  index = self->rewardIndex;
  switch (self->rewardKind) {
  case 0:
    return;
  case 1:
    sprintf(name, "DWCardHelp");
    break;
  case 2:
    sprintf(name, "DWHologramHelp");
    break;
  case 3:
    sprintf(name, "DWCommon");
    index = 0;
    break;
  case 4:
    UiUnlockResultFormatNumber(self, self->texts[1], index);
    sprintf(name, "DWCommon");
    index = 1;
    break;
  case 5:
    sprintf(name, "DWMaxusPartsHelp");
    index = g_rewardMaxusSet;
    break;
  case 6:
    sprintf(name, "DWMetalFigureHelp");
    break;
  case 7:
    sprintf(name, "DWSpecialUnlock");
    index = 2;
    break;
  case 8:
    sprintf(name, "DWSpecialUnlock");
    index = 5;
    break;
  case 9:
    sprintf(name, "DWCommon");
    index = 2;
    break;
  default:
    return;
  }
  dst = self->texts[1];
  table = SaveFindLocalizedBin(name);
  UiMesTableRelocate(table);
  src = ((char **)table)[index];
  if (self->rewardKind == 4) {
    strcpy(line, src);
    strcat(dst, line);
  } else {
    strcpy(dst, src);
  }
  printer = self->printers[1];
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;

  printer = self->printers[1];
  anchor = ((GfxSprite **)self->base.data)[3];
  entry = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
      anchor->posX, anchor->posY, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);
  UiTextMeasure(0.0f, self->printers[1], dst, &self->textWidth[1], &self->textHeight[1],
                &self->textLines[1]);

  printer = self->printers[1];
  self->textGlyphs[1] = printer->glyphs;
  first = self->textGlyphs[1];
  count = (float)printer->glyphCount;
  maxOffset = 0;
  i = 0;
  self->textReveal[1] = count;
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
    } while ((float)i < self->textReveal[1]);
  }
  self->textAlpha[1] = 0.0f;
  self->textFadeBase[1] = 0.0f;
  self->textVisible[1] = 1;
}
