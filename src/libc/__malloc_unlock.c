// bdc 0x089b9a64 __malloc_unlock
#include "bdc.h"

/* Newlib's malloc unlock hook: leaves one nesting level of the allocator lock and, at the
   outermost level, restores the interrupt state saved by __malloc_lock. */
void __malloc_unlock(_reent *reent)
{
  (void)reent;
  g_mallocLockDepth = g_mallocLockDepth - 1;
  if (g_mallocLockDepth == 0) {
    sceKernelCpuResumeIntr(g_mallocLockIntrFlags);
  }
}
