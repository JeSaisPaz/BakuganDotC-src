// bdc 0x08a29fc8 BtlTargetPointIsBakugan
#include "bdc.h"

/* Target-point override of the battle-unit "is a Bakugan" class test (virtual slot 10, `+0x54`):
   a target point is never a Bakugan, so it returns 0. */
int BtlTargetPointIsBakugan(void *unit)
{
    (void)unit;
    return 0;
}
