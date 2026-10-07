// bdc 0x08877994 BtlAttackGetCategory
#include "bdc.h"

/* Returns the category of the attack's type: bits 10..15 of the sign-extended low halfword of
   the type's info word in `g_btlAttackTypeInfo`. Used by `BtlAttackUpdateGuided`. */
int BtlAttackGetCategory(BtlAttack *self)
{
    return ((int)(s16)g_btlAttackTypeInfo[self->type] & 0xfc00U) >> 10;
}
