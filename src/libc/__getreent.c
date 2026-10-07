// bdc 0x089b4a60 __getreent
#include "bdc.h"

/* Newlib's `__getreent()`: returns the current reentrancy struct. Here it unconditionally returns
   the global `g_impurePtr` (`_impure_ptr`); there is no per-thread lookup. */
_reent *__getreent(void)
{
    return g_impurePtr;
}
