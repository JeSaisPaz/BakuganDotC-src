// bdc 0x08818284 UiTextPrinterPrintf
#include "bdc.h"

/* printf-style front end of `UiTextPrinterPrintSjis`: formats `format` and the varargs into a
   512-byte stack buffer (`vsprintf`) and prints it with `printer` at (`x`, `y`); an empty format
   string prints nothing. */

void UiTextPrinterPrintf(float x, float y, UiTextPrinter *self, const char *format, ...)

{
  char buf[512];
  __builtin_va_list ap;

  if (strlen(format) != 0) {
    __builtin_va_start(ap, format);
    vsprintf(buf, (char *)format, ap);
    __builtin_va_end(ap);
    UiTextPrinterPrintSjis(x, y, self, buf);
  }
}
