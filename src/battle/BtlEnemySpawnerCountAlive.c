// bdc 0x088a5690 BtlEnemySpawnerCountAlive
#include "bdc.h"

/* Counts the live units of this spawner in the battle unit list (`BtlGetBakuganList`; 0 when
   the list is missing): a unit counts when its vtable entry 14 (`BtlBakuganIsTargetPoint` in
   the base class) returns 0, its entry 13 (the CPU-unit test) returns non-zero, its
   `BtlCombatState` is not `dead`, and its `BtlCpuUnit` `spawnerId` equals the spawner's
   `id`. Used by `BtlEnemySpawnerUpdate`. */
int BtlEnemySpawnerCountAlive(void *spawner)
{
    const BtlEnemySpawner *self = (const BtlEnemySpawner *)spawner;
    CoreObjectList *list = (CoreObjectList *)BtlGetBakuganList();
    CoreObject *obj;
    int count = 0;

    if (list == NULL) {
        return 0;
    }
    for (obj = list->head; obj != NULL; obj = obj->next) {
        const VtblEntry *isTargetPoint = &((const VtblEntry *)obj->vtable)[14];
        const VtblEntry *isCpuUnit;
        BtlCpuUnit *unit = (BtlCpuUnit *)obj;

        if (((int (*)(void *))isTargetPoint->fn)((u8 *)obj + isTargetPoint->delta) != 0) {
            continue;
        }
        isCpuUnit = &((const VtblEntry *)obj->vtable)[13];
        if (((int (*)(void *))isCpuUnit->fn)((u8 *)obj + isCpuUnit->delta) == 0) {
            continue;
        }
        if (unit->base.combat.dead != 0) {
            continue;
        }
        if (unit->spawnerId == self->id) {
            count++;
        }
    }
    return count;
}
