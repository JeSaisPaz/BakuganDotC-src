// bdc 0x08a0fb94 GmoImageNullFree
#include "bdc.h"

/* No-op free function of the image library: the default of the third free slot `0x08af1254`, paired
   with `GmoImageNullAlloc`. */
void GmoImageNullFree(void *ptr)
{
    (void)ptr;
}
