// bdc 0x089b47a8 _exit
#include "bdc.h"

/* PSP libc `_exit(status)`: unloads the game module with
   `sceKernelStopUnloadSelfModuleWithStatus(1, 0, NULL, NULL, NULL)`; if that returns, it prints
   `"libc:_exit: something wrong 0x%08X"` with the result through `sceKernelPrintf` and executes
   `break 0` (`__builtin_trap()`). The original also holds a main-thread check and a `_reclaim_reent` path guarded by a
   weak symbol whose address is 0 in this link, so that code is unreachable and `status` is unused. */

void _exit(int status)
{
    s32 result;

    (void)status;
    result = sceKernelStopUnloadSelfModuleWithStatus(1, 0, NULL, NULL, NULL);
    sceKernelPrintf("libc:%s: something wrong 0x%08X\n", "_exit", result);
    __builtin_trap(); /* break 0x0 */
}
