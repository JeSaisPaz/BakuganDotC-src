// bdc 0x08a05ef0 matherr
#include "bdc.h"

/* Default SVID `matherr(struct exception *)`: returns 0, so the math wrappers (`acos`, `pow`,
   `sqrt`, `acosf`, `atan2f`, `powf`) set `errno` and use their default results. */
int matherr(void *exc)
{
    (void)exc;
    return 0;
}
