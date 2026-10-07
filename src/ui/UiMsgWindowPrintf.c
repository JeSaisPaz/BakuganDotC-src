// bdc 0x08816ae0 UiMsgWindowPrintf
#include "bdc.h"

/* `vsprintf`s up to six arguments into a 256-byte buffer, converts it with `UiTextEncodeSjis` and
   sets it as the message window's text (`UiMsgWindowSetText`). */

s32 UiMsgWindowPrintf(UiMsgWindow *self, char *format, ...)
{
  u8 enc[256];
  char buf[256];
  __builtin_va_list ap;

  __builtin_va_start(ap, format);
  vsprintf(buf, (char *)format, ap);
  __builtin_va_end(ap);
  UiTextEncodeSjis(enc, buf);
  return UiMsgWindowSetText(self, enc);
}
