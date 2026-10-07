// bdc 0x089b7474 _fmt_group_thousands
#include "bdc.h"

/* Implements the ' (thousands-grouping) printf flag: copies the number text [start, end] to out
   with a ',' inserted every three digits left of the decimal point. The text is rebuilt right to
   left into a stack buffer (end - start + 10 bytes) and then memcpy'd to out; the new length is
   stored in *len and out is returned. Text containing 'e'/'E' is left alone (returns start, *len
   untouched); returns NULL if out, start or end is NULL. */
char *_fmt_group_thousands(char *out, char *start, char *end, u32 *len)
{
  int count = (int)(end - start) + 10;
  char buf[count];
  char *dst = &buf[count - 1];
  int digits = 0;
  int state; /* 0: no '.', 1: '.' seen, 2: exponent seen */
  char *dot;
  char *p;
  char c;
  size_t n;

  if (out == NULL || start == NULL || end == NULL) {
    return NULL;
  }

  state = 0;
  dot = end;
  for (p = start; p <= end && state != 2; p++) {
    c = *p;
    if (c == 'e' || c == 'E') {
      state = 2;
    } else if (c == '.') {
      dot = p; /* every '.' moves the split point, even after the first */
      if (state == 0) {
        state = 1;
      }
    }
  }
  if (state == 2) {
    return start;
  }

  c = *end;
  while (start < end) {
    *dst-- = c;
    if (end <= dot) {
      if (digits % 3 != 0 || digits == 0) {
        digits++;
      } else {
        *dst-- = ',';
        count++;
        digits++;
      }
    }
    end--;
    c = *end;
  }
  n = count - 10;
  *dst = c;
  memcpy(out, dst, n);
  *len = n;
  return out;
}
