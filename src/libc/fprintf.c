// bdc 0x08a0f85c fprintf
#include "bdc.h"

/* Standard `fprintf(fp, fmt, ...)`: spills the variadic register arguments `a2..a7` into a stack
   array and passes its address as the `va_list` to `vfprintf`. Its only caller is the C++ runtime
   message printer `CxxPrintAbortMessage`. */

int fprintf(void *fp, const char *fmt, ...)
{
    __builtin_va_list ap;
    int result;

    __builtin_va_start(ap, fmt);
    result = vfprintf(fp, fmt, ap);
    __builtin_va_end(ap);
    return result;
}
