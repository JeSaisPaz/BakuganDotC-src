// bdc 0x08877fcc BtlAttackListKillAll
#include "bdc.h"

/* Expires every attack object chained from `g_btlAttackList`: sets each one's `age` to
   0x7fffffff and runs `BtlAttackUpdate` on it, so ended attacks are destroyed at once. The next
   link is read before the update, which may delete the object. Called by `BtlFinishTaskDtor`. */
void BtlAttackListKillAll(void)
{
  BtlAttack *attack;
  BtlAttack *next;

  attack = (BtlAttack *)g_btlAttackList;
  while (attack != NULL) {
    next = (BtlAttack *)attack->base.next;
    attack->age = 0x7fffffff;
    BtlAttackUpdate(attack);
    attack = next;
  }
}
