// bdc 0x08996b80 UiWorldMapCreateTextSlots
#include "bdc.h"

/* Creates the three text slots `textSlot[]` of `UiWorldMap` (stride 0x224 from
   +0x1220): each gets a 0xf0-byte `UiTextPrinterCtor` printer allocated from the low heap on the
   `g_uiWorldMapFontNames` (`"wd_font16"`) font table (NULL if the allocation fails), then font 1,
   scale 0.8, wrap width 1000 and width scale 0.53, an empty string `text`, alpha 0, last applied
   alpha 1 and glyph count 0. */

void UiWorldMapCreateTextSlots(UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  bool fromLow;
  UiTextPrinter *mem;
  UiTextPrinter *printer;
  int i;

  for (i = 0; i < 3; i++) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(0xf0, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    printer = NULL;
    if (mem != NULL) {
      UiTextPrinterCtor(mem, 0, &g_uiWorldMapFontNames);
      printer = mem;
    }
    map->textSlot[i].printer = printer;
    UiTextPrinterSetFont(printer, 1);
    map->textSlot[i].printer->scale = 0.8f;
    map->textSlot[i].printer->wrapWidth = 1000.0f;
    map->textSlot[i].printer->widthScale = 0.53333336f;
    strcpy(map->textSlot[i].text, "");
    map->textSlot[i].alpha = 0.0f;
    map->textSlot[i].drawnAlpha = 1.0f;
    map->textSlot[i].glyphCount = 0.0f;
  }
  return;
}
