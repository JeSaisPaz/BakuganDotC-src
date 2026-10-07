// bdc 0x08944ee4 UiStaffCreditCreateLinePrinters
#include "bdc.h"

/* Creates the 20 credit-line text printers of `UiStaffCredit` (`overlays[0..19]`,
   0xf0-byte `UiTextPrinter`s allocated from low memory, font 2, width table
   `g_uiFont12Widths`, wrap width 480), then finds the credit text `"DNStaffCredit.bin"` in the
   loaded pack chain (`CorePackChainFind` on `g_ioLzsPackages`) into `lineTexts` and stores its
   entry count (`UiMesTableRelocate`, which also relocates it) in `lineCount`.

   ## Notes
   - A failed allocation still stores NULL and calls `UiTextPrinterSetFont` on it, as the
     original does. */

void UiStaffCreditCreateLinePrinters(UiScreen *screen)

{
  UiStaffCredit *credit = (UiStaffCredit *)screen;
  UiTextPrinter *mem;
  UiTextPrinter *printer;
  void *table;
  bool fromLow;
  s32 i;

  i = 0;
  do {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(0xf0, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    printer = NULL;
    if (mem != NULL) {
      UiTextPrinterCtor(mem, 0, NULL);
      printer = mem;
    }
    credit->overlays[i] = (GfxSpriteLayer *)printer; /* layer is the first member */
    UiTextPrinterSetFont(printer, 2);
    ((UiTextPrinter *)credit->overlays[i])->widthTable = g_uiFont12Widths;
    ((UiTextPrinter *)credit->overlays[i])->wrapWidth = 480.0f;
    i++;
  } while (i < 20);
  table = CorePackChainFind(g_ioLzsPackages, "DNStaffCredit.bin");
  credit->lineTexts = (char **)table;
  credit->lineCount = (s32)UiMesTableRelocate((u32 *)table);
}
