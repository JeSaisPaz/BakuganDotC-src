// bdc 0x089b99fc _localeconv_r
#include "bdc.h"

/* Newlib's `_localeconv_r(reent)`: returns the address of the static C-locale `struct lconv`
   (`g_lconv`). The `reent` argument is ignored. */
void *_localeconv_r(_reent *reent)
{
    (void)reent;
    return (void *)&g_lconv;
}
