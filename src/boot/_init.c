// bdc 0x08a0d1b4 _init
#include "bdc.h"

/* crt0 `_init` (the `.init` section body), called by `BootUserMainThread` right after
   `atexit(_fini)` (`atexit` with `0x08a0d1bc`, the adjacent `.fini` stub) and before `main`,
   exactly as in the PSP SDK crt0 `_main`. It is empty (`jr ra`) because the C++ static constructors
   run from `CxxInit` instead. */
void _init(void)
{
}
