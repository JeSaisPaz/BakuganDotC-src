// bdc 0x089b4fd4 strcat
#include "bdc.h"

/* Standard strcat: appends src (including its terminator) to the end of dst; returns dst. */
char *strcat(char *dst, const char *src)
{
  char *out = dst;
  char c;

  while (*out != '\0') {
    out++;
  }
  do {
    c = *src++;
    *out++ = c;
  } while (c != '\0');
  return dst;
}
