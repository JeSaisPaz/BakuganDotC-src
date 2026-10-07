// bdc 0x089b4c44 sprintf
#include "bdc.h"

/* Standard newlib sprintf: formats fmt and the variadic arguments into str with no length bound,
   through vfprintf on a stack string-sink FILE (__SWR | __SSTR, write space 0x7fffffff, owned by the
   global reent), then NUL-terminates at the final write position. Returns vfprintf's result. */
int sprintf(char *str, const char *format, ...)
{
  FILE sink;
  __builtin_va_list ap;
  int ret;

  sink._flags = 0x208;
  sink._p = (u8 *)str;
  sink._bf_base = (u8 *)str;
  sink._w = 0x7fffffff;
  sink._bf_size = 0x7fffffff;
  sink._data = g_impurePtr;
  __builtin_va_start(ap, format);
  ret = vfprintf(&sink, format, ap);
  __builtin_va_end(ap);
  *sink._p = '\0';
  return ret;
}
