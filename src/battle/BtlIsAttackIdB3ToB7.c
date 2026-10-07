// bdc 0x0885dd9c BtlIsAttackIdB3ToB7
#include "bdc.h"

/* Returns 1 for attack ids 0xb3..0xb7, else 0. `BtlBakuganOnHit` uses it twice: such hits skip
   the dead branch, and they make the victim target its attacker (`BtlBakuganSetTarget`). */
int BtlIsAttackIdB3ToB7(int attackId)
{
    if (attackId > 0xb2 && attackId < 0xb8) {
        return 1;
    }
    return 0;
}
