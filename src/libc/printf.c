// bdc 0x089b4be4 printf
#include "bdc.h"

/* Standard `printf(fmt, ...)`: sets `stdout->_data` to the global `_reent` (`g_impurePtr`)
   and formats to `stdout` (`_impure_ptr->_stdout`) through `vfprintf` with the variadic
   arguments. Output reaches the PSP debug console (stdout handle) via `_write`; returns
   `vfprintf`'s result. */
int printf(char *fmt, ...)
{
    _reent *reent = g_impurePtr;
    __builtin_va_list ap;
    int ret;

    reent->_stdout->_data = reent;
    __builtin_va_start(ap, fmt);
    ret = vfprintf(reent->_stdout, fmt, ap);
    __builtin_va_end(ap);
    return ret;
}
