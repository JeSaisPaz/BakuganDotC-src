// bdc 0x08877f48 BtlAttackListResolvePendingHits
#include "bdc.h"

/* Runs `BtlAttackResolvePendingHit` on every attack chained from `g_btlAttackList`. The next
   link is read before the call, which may end the attack. Called by `BtlAttackListUpdate`. */
void BtlAttackListResolvePendingHits(void)
{
    BtlAttack *attack;
    BtlAttack *next;

    attack = (BtlAttack *)g_btlAttackList;
    while (attack != NULL) {
        next = (BtlAttack *)attack->base.next;
        BtlAttackResolvePendingHit(attack);
        attack = next;
    }
}
