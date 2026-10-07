// bdc 0x08a10830 GmoImageHeapNullFree
#include "bdc.h"

/* Default free function of the image library's 3-pool block heap (`GmoImageHeapSetPool` installs
   it when no free function is given): empty. */
void GmoImageHeapNullFree(void *ptr)
{
    (void)ptr;
}
