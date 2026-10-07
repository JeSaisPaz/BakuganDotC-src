// bdc 0x08a0f8b8 memalign
#include "bdc.h"

/* newlib `memalign(align, size)`: `_memalign_r` on `g_impurePtr`. */

void *memalign(size_t align, size_t size)
{
    return _memalign_r(g_impurePtr, align, size);
}
