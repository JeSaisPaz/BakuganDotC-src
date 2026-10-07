// bdc 0x08877a90 BtlAttackTypeGetCategory
#include "bdc.h"

/* Returns the category (bits 10..15 of the sign-extended low halfword of the info word in
   `g_btlAttackTypeInfo`) of attack type `type`. `BtlBakuganOnHit` treats category 0xc
   specially when the unit is guarding. */
int BtlAttackTypeGetCategory(int type)
{
    return ((int)(s16)g_btlAttackTypeInfo[type] & 0xfc00U) >> 10;
}
