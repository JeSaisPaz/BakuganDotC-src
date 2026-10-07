// bdc 0x089ec174 UiMsgBoxPrintf
#include "bdc.h"

/* `vsprintf`s `fmt` into a 512-byte buffer, converts it to font codes with `UiTextEncodeSjis` and
   loads it into the box with `UiMsgBoxSetText`. */

bool UiMsgBoxPrintf(UiMsgBox *self, const char *fmt, ...)
{
  u8 enc[512];
  char buf[512];
  __builtin_va_list ap;

  __builtin_va_start(ap, fmt);
  vsprintf(buf, (char *)fmt, ap);
  __builtin_va_end(ap);
  UiTextEncodeSjis(enc, buf);
  return UiMsgBoxSetText(self, enc);
}
