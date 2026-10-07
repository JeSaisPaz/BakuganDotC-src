// bdc 0x088d0380 UiFieldHudLoadHints
#include "bdc.h"

/* Creates the hint text printer `hintPrinter` of the field HUD (task 3001, `UiFieldHudCtor`)
   (`UiTextPrinterCtor` on a 0xf0-byte low-heap block, font 3, scale 0.7, wrap width 200, view
   y = `promptY`) and loads `"mes_Adventure_hint_<language>.bin"` from the resource packs into
   `hints` (`UiMesTableRelocate`). A failed allocation leaves `hintPrinter` NULL (and still
   calls `UiTextPrinterSetFont` on it). */

void UiFieldHudLoadHints(UiFieldHud *self)
{
  bool fromLow;
  UiTextPrinter *mem;
  UiTextPrinter *printer;
  const char *language;
  u32 *table;
  char name[0x40];

  printer = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(UiTextPrinter), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    UiTextPrinterCtor(mem, 0, NULL);
    printer = mem;
  }
  self->hintPrinter = printer;
  UiTextPrinterSetFont(printer, 3);
  self->hintPrinter->scale = 0.7f;
  self->hintPrinter->widthScale = 0.7f;
  self->hintPrinter->wrapWidth = 200.0f;
  self->hintPrinter->layer.view.w.y = self->promptY;
  name[0] = '\0';
  memset(&name[1], 0, 0x3f);
  SaveGetProfile();
  language = SaveGetLanguageName();
  sprintf(name, "mes_Adventure_hint_%s.bin", language);
  table = CorePackChainFind(g_ioLzsPackages, name);
  self->hints = (char **)table;
  UiMesTableRelocate(table);
}
