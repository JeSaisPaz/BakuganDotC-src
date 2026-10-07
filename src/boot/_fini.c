// bdc 0x08a0d1bc _fini
#include "bdc.h"

/* crt0 `_fini` (the `.fini` section body), registered with atexit by BootUserMainThread
   before _init and main; empty (`jr ra`). */
void _fini(void)
{
}
