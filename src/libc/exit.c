// bdc 0x089b4db4 exit
#include "bdc.h"

/* Standard `exit(status)`: runs every registered `atexit` handler (walking the `_atexit` block
   list of `_reent` `_atexit` from the newest block back, and each block's `_fns[]` from `_ind-1`
   down to 0, i.e. reverse registration order), then calls `__cleanup(reent)` (stdio flush) if set,
   and finally `_exit`. */

void exit(int status)
{
    _atexit *block;
    s32 i;
    void (*cleanup)(_reent *);

    for (block = g_impurePtr->_atexit; block != NULL; block = block->_next) {
        for (i = block->_ind - 1; i >= 0; i--) {
            ((void (*)(void))block->_fns[i])();
        }
    }
    cleanup = (void (*)(_reent *))g_impurePtr->__cleanup;
    if (cleanup != NULL) {
        cleanup(g_impurePtr);
    }
    _exit(status);
}
