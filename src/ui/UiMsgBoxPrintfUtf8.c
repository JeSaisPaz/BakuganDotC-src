// bdc 0x089ec1f8 UiMsgBoxPrintfUtf8
#include "bdc.h"

/* Same as `UiMsgBoxPrintf` but formats into a 768-byte buffer and converts UTF-8 with
   `UiTextEncodeUtf8`. */

bool UiMsgBoxPrintfUtf8(UiMsgBox *self, const char *fmt, ...)
{
  u8 enc[768];
  char buf[768];
  __builtin_va_list ap;

  __builtin_va_start(ap, fmt);
  vsprintf(buf, (char *)fmt, ap);
  __builtin_va_end(ap);
  UiTextEncodeUtf8(enc, buf);
  return UiMsgBoxSetText(self, enc);
}
