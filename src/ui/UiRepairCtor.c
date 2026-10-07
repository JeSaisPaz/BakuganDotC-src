// bdc 0x0890f450 UiRepairCtor
#include "bdc.h"

/* Constructor of the card repair screen, task id 430 (0x1ae) (base `UiScreenCtor`, vtable
   `g_uiRepairVtable`). Allocates the 21-entry layout sprite table (0x54 bytes) as `data`, saves
   the pad's `stickEmulatesDpad` and forces it on, clears `unk70` and the repair kind, sets the
   active fader's sort key to 10, creates the text printer (`UiTextPrinterCtor` with
   `g_uiRepairFontNames`; 16x16 cells, spacing -4, width table `g_uiRepairFontWidths`, scale
   0.75, wrap width 248) and empties the two message lines. Returns `screen`.

   ## Notes
   A failed printer allocation stores NULL and still writes the printer fields through it. */

UiScreen *UiRepairCtor(UiScreen *screen)

{
  UiRepair *repair = (UiRepair *)screen;
  bool fromLow;
  void *sprites;
  GfxFader *fader;
  UiTextPrinter *mem;
  UiTextPrinter *printer;
  PadState *pad;

  UiScreenCtor(&screen->base);
  screen->base.vtable = &g_uiRepairVtable;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprites = MemAlloc(21 * sizeof(GfxSprite *), (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  pad = screen->pad;
  screen->data = sprites;
  repair->savedStickEmulatesDpad = pad->stickEmulatesDpad;
  pad->stickEmulatesDpad = 1;
  repair->unk70 = 0;
  repair->value = 0;
  fader = GfxGetActiveFader();
  fader->sortKey = 10.0f;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(UiTextPrinter), (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  printer = (UiTextPrinter *)0x0;
  if (mem != (UiTextPrinter *)0x0) {
    UiTextPrinterCtor(mem, 0, g_uiRepairFontNames);
    printer = mem;
  }
  repair->textBox = printer;
  printer->cellW = 16.0f;
  repair->textBox->cellH = 16.0f;
  repair->textBox->advanceX = 16.0f;
  repair->textBox->lineHeight = 16.0f;
  repair->textBox->spacing = -4.0f;
  repair->textBox->widthTable = g_uiRepairFontWidths;
  repair->textBox->scale = 0.75f;
  repair->textBox->wrapWidth = 248.0f;
  strcpy(repair->message, "");
  strcpy(repair->detail, "");
  return screen;
}
