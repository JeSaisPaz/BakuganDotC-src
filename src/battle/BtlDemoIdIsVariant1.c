// bdc 0x089054c8 BtlDemoIdIsVariant1
#include "bdc.h"

/* Returns 1 when `id` is in 0x17..0x7c and `(id - 0x17) % 4 == 1`, else 0. */
bool BtlDemoIdIsVariant1(int id)
{
    if (id < 0x17 || id >= 0x7d) {
        return false;
    }
    return (id - 0x17) % 4 == 1;
}
