// bdc 0x089b52e4 _fmt_round_digits
#include "bdc.h"

/* Decimal rounding step of the %e/%f/%g formatter: digits[n-1] is the first discarded digit.
   If it is above '4' it becomes '0' and a carry is added to the preceding digits ('9' -> '0',
   propagating left). Returns 0 when the carry runs off the front (the digit reached at index 0
   or below is still '9'), else 1. Note: for n == 1 the loop reads digits[-1], as the original. */
int _fmt_round_digits(char *digits, int n)
{
  int i = n - 1;
  char c;

  if (n > 0 && digits[i] > '4') {
    do {
      digits[i] = '0';
      i--;
      c = digits[i];
      if (i < 1) {
        break;
      }
    } while (c == '9');
    if (c == '9') {
      return 0;
    }
    digits[i] = c + 1;
  }
  return 1;
}
