// bdc 0x089b4e84 free
#include "bdc.h"

/* Standard free: returns ptr to the libc heap via _free_r on the global reent. */
void free(void *ptr)
{
  _free_r(g_impurePtr, ptr);
}
