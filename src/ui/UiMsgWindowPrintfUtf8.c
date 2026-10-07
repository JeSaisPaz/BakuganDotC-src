// bdc 0x08816b64 UiMsgWindowPrintfUtf8
#include "bdc.h"

/* Like `UiMsgWindowPrintf` but with 768-byte buffers and `UiTextEncodeUtf8`: used for the
   localised (accented) message strings of the language tables, e.g. the save tasks' 'not enough
   space (%d KB)' message. */

s32 UiMsgWindowPrintfUtf8(UiMsgWindow *self, char *format, ...)
{
  u8 enc[768];
  char buf[768];
  __builtin_va_list ap;

  __builtin_va_start(ap, format);
  vsprintf(buf, (char *)format, ap);
  __builtin_va_end(ap);
  UiTextEncodeUtf8(enc, buf);
  return UiMsgWindowSetText(self, enc);
}
