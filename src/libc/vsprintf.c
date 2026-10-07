// bdc 0x089b4cc0 vsprintf
#include "bdc.h"

/* Standard `vsprintf(str, fmt, ap)`: builds a string-sink `FILE` on the stack (`_flags = 0x208` =
   `__SWR|__SSTR`, `_w = _bf._size = 0x7fffffff`, `_p = _bf._base = str`, `_data =
   ``g_impurePtr`), formats into it with `vfprintf` and NUL-terminates at the final `_p`.
   Returns `vfprintf`'s character count. */

int vsprintf(char *str, char *fmt, void *ap)
{
    FILE f;
    int result;

    f._flags = 0x208;
    f._p = (u8 *)str;
    f._bf_base = (u8 *)str;
    f._w = 0x7fffffff;
    f._bf_size = 0x7fffffff;
    f._data = g_impurePtr;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}
