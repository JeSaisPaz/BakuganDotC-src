// bdc 0x0894e68c UiNetMenuSetHelpText
#include "bdc.h"

/* Prints help message `entry` of `"DOMesHelp"` (mapped through `UiButtonCellOffset(0, entry)`) into the
   help printer of `UiNetMenu`: copies it to `helpText`, clears the printer and
   re-prints it 6 px above sprite 14 of the sprite table, measures it (`UiTextMeasure` into
   `helpWidth`/`helpHeight`/`helpLines`), records the glyphs (`helpGlyphs`, count `helpGlyphCount`)
   and resets the help animation (`helpAnimFrom` = 24, `helpAnimT` = 0, `helpAnimFrames` = 30). */

void UiNetMenuSetHelpText(UiScreen *screen, u8 entry)
{
  UiNetMenu *menu = (UiNetMenu *)screen;
  u32 *table;
  char *dst;
  int index;
  UiTextPrinter *printer;
  GfxSprite *anchor;
  const VtblEntry *slot;

  menu->helpAnimFrom = 24.0f;
  table = SaveFindLocalizedBin("DOMesHelp");
  UiMesTableRelocate(table);
  dst = (char *)menu->helpText;
  index = UiButtonCellOffset(0, entry);
  strcpy(dst, ((char **)table)[index]);
  printer = (UiTextPrinter *)menu->helpPrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;

  printer = (UiTextPrinter *)menu->helpPrinter;
  anchor = ((GfxSprite **)screen->data)[14];
  slot = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, s32, s32, s32))slot->fn)(
      anchor->posX, anchor->posY - 6.0f, 0.0f, (u8 *)printer + slot->delta, dst, 1, 0, 0);
  UiTextMeasure(0.0f, menu->helpPrinter, dst, &menu->helpWidth, &menu->helpHeight, &menu->helpLines);

  printer = (UiTextPrinter *)menu->helpPrinter;
  menu->helpGlyphs = printer->glyphs;
  menu->helpAnimT = 0.0f;
  menu->helpAnimFrames = 30;
  menu->helpGlyphCount = (float)printer->glyphCount;
}
