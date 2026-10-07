// bdc 0x089b53b4 _itoa
#include "bdc.h"

/* Non-standard itoa(value, buf, base): writes the digits of value in base (2..36) into buf least
   significant first, appends '-' if value was negative, terminates and reverses buf with _strrev;
   returns buf. Only base 10 negates a negative value: other bases convert its unsigned bit pattern
   but still append the '-'. */
char *_itoa(s32 value, char *buf, u32 base)
{
  int negative = value < 0;
  u32 v = (u32)value;
  int len = 0;

  if (negative && base == 10) {
    v = (u32)-value;
  }
  do {
    buf[len++] = g_itoaDigits[v % base];
    v /= base;
  } while (v != 0);
  if (negative) {
    buf[len++] = '-';
  }
  buf[len] = '\0';
  return _strrev(buf);
}
