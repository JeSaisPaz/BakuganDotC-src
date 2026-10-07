// bdc 0x089da9fc GmoFreeAligned
#include "bdc.h"

/* Free callback matching `GmoAllocAligned`: forwards to `MemFreeAligned`. */
void GmoFreeAligned(void *ptr)
{
    MemFreeAligned(ptr);
}
