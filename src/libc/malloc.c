// bdc 0x089b4e60 malloc
#include "bdc.h"

/* Standard malloc: allocates nbytes from the libc heap via _malloc_r on the global reent. */
void *malloc(u32 nbytes)
{
  return _malloc_r(g_impurePtr, nbytes);
}
