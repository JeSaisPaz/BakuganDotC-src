// bdc 0x08a12ccc GmoHeapNullFree
#include "bdc.h"

/* Default free function of the model library's 3-pool block heap (`GmoHeapSetVramPool`,
   `GmoHeapSetMainPool` install it when no free function is given): empty. */
void GmoHeapNullFree(void *ptr)
{
    (void)ptr;
}
