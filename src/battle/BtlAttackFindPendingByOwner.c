// bdc 0x08877f88 BtlAttackFindPendingByOwner
#include "bdc.h"

/* Returns the first attack in `g_btlAttackList` whose `pendingHit` flag is set and whose
   `owner` is `owner`, or NULL. */
void *BtlAttackFindPendingByOwner(void *owner)
{
    BtlAttack *atk;

    for (atk = (BtlAttack *)g_btlAttackList; atk != NULL; atk = (BtlAttack *)atk->base.next) {
        if (atk->pendingHit != 0 && (void *)atk->owner == owner) {
            return atk;
        }
    }
    return NULL;
}
