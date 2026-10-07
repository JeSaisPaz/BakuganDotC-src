// bdc 0x0888bfc4 BtlAttackParamsGet
#include "bdc.h"

/* Returns the `BtlAttackParams` entry of attack type `type` in `g_btlAttackParamTable`, or NULL
   when `type` is outside 0..0x8d. */
BtlAttackParams *BtlAttackParamsGet(int type)
{
    if (type >= 0 && type < 0x8e) {
        return &g_btlAttackParamTable[type];
    }
    return NULL;
}
