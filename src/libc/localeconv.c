// bdc 0x089b9a08 localeconv
#include "bdc.h"

/* Standard `localeconv()`: returns `_localeconv_r`(`g_impurePtr`), the address of the
   C-locale `struct lconv` (`g_lconv`). */
void *localeconv(void)
{
    return _localeconv_r(g_impurePtr);
}
