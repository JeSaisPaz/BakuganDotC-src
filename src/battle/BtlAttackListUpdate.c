// bdc 0x08878d6c BtlAttackListUpdate
#include "bdc.h"

/* Per-frame update of every attack in `g_btlAttackList`: first resolves pending hits
   (`BtlAttackListResolvePendingHits`), then runs `BtlAttackUpdate` on each attack, reading the
   next link before the call (the update may delete the attack). */
void BtlAttackListUpdate(void)
{
    BtlAttack *attack;
    BtlAttack *next;

    BtlAttackListResolvePendingHits();
    attack = (BtlAttack *)g_btlAttackList;
    while (attack != NULL) {
        next = (BtlAttack *)attack->base.next;
        BtlAttackUpdate(attack);
        attack = next;
    }
}
