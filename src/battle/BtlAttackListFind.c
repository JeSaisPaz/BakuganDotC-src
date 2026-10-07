// bdc 0x08878ca4 BtlAttackListFind
#include "bdc.h"

/* Returns `self` if it is still in `g_btlAttackList`, otherwise NULL (validity check for stored
   attack pointers). */
void *BtlAttackListFind(BtlAttack *self)
{
    CoreObject *node;

    for (node = g_btlAttackList; node != NULL; node = node->next) {
        if (node == &self->base) {
            return node;
        }
    }
    return NULL;
}
