// bdc 0x08905480 BtlDemoIdIsVariant3
#include "bdc.h"

/* Returns 1 when `id` is in 0x17..0x7c and `(id - 0x17) % 4 == 3`, else 0. */
bool BtlDemoIdIsVariant3(int id)
{
    if (id < 0x17 || id >= 0x7d) {
        return false;
    }
    return (id - 0x17) % 4 == 3;
}
