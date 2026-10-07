// bdc 0x08a0f780 __errno
#include "bdc.h"

/* newlib `__errno()`: returns `&errno`, the `_errno` field (first word) of the current
   reentrancy structure `g_impurePtr`. Used by the math wrappers to set `EDOM`/`ERANGE`. */
int *__errno(void)
{
    return &g_impurePtr->_errno;
}
