// bdc 0x089b9a28 __malloc_lock
#include "bdc.h"

/* Newlib's `__malloc_lock(reent)` hook, implemented for the PSP with interrupt suspension: calls
   `sceKernelCpuSuspendIntr`, stores its return value in `g_mallocLockIntrFlags` only for the
   outermost lock (when `g_mallocLockDepth` is 0) and increments the depth. Nested locks therefore
   keep the original interrupt state. Taken by `_malloc_r`, `_malloc_trim_r` and `_free_r`;
   the `reent` argument is unused. */

void __malloc_lock(_reent *reent)
{
    s32 intrFlags;

    (void)reent;
    intrFlags = sceKernelCpuSuspendIntr();
    if (g_mallocLockDepth == 0) {
        g_mallocLockIntrFlags = intrFlags;
    }
    g_mallocLockDepth++;
}
