// bdc 0x0893b6b8 UiUnlockResultFormatNumber
#include "bdc.h"

/* Writes `value` as an 8-digit-max decimal string in the game's text encoding to `dst`: digits from
   `UiNumberToDigits` (0xff-terminated) are shifted by +0x11 (the encoding stores characters as ASCII
   − 0x1f) and the string is NUL-terminated. */

void UiUnlockResultFormatNumber(UiUnlockResult *self, char *dst, u32 value)

{
  char c;
  char *p;
  int i;

  UiNumberToDigits((u8 *)dst, value, 8, 0xff);
  i = 0;
  c = *dst;
  p = dst;
  while (c != -1) {
    *p = c + 0x11;
    i = i + 1;
    p = dst + i;
    c = *p;
  }
  *p = '\0';
  return;
}

