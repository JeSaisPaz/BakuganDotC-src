// bdc 0x0880f308 ScriptOpJumpIfAllOpponentsDeadB
#include "bdc.h"

/* Variant of `ScriptOpJumpIfAllOpponentsDead` with a different unit filter: a unit counts when it
   is not a target point (virtual `+0x74`) or its virtual `+0x9c` returns 1, is not the player's
   unit, and its virtual `+0x8c` is false; when all counted units have the `BtlCombatState` `dead`
   byte (`+0x4c1`) set (or none are counted) the track pc is set to u16 `target` and 3 is returned,
   else 0. */

int ScriptOpJumpIfAllOpponentsDeadB(Script *script)
{
    u32 target;
    CoreObjectList *list;
    void *player;
    CoreObject *obj;
    int deadCount = 0;
    int count;

    target = ScriptReadU16(script);
    list = (CoreObjectList *)BtlGetBakuganList();
    obj = NULL;
    player = BtlGetPlayerBakugan();
    if (list != NULL) {
        obj = list->head;
    }
    count = 0;
    for (; obj != NULL; obj = obj->next) {
        const VtblEntry *isTargetPoint = &((const VtblEntry *)obj->vtable)[14];
        const VtblEntry *vf19;
        const VtblEntry *vf17;

        if (((int (*)(void *))isTargetPoint->fn)((u8 *)obj + isTargetPoint->delta) != 0) {
            vf19 = &((const VtblEntry *)obj->vtable)[19];
            if (((int (*)(void *))vf19->fn)((u8 *)obj + vf19->delta) != 1) {
                continue;
            }
        }
        if ((void *)obj == player) {
            continue;
        }
        vf17 = &((const VtblEntry *)obj->vtable)[17];
        if (((int (*)(void *))vf17->fn)((u8 *)obj + vf17->delta) != 0) {
            continue;
        }
        if (((BtlBakugan *)obj)->combat.dead != 0) {
            deadCount++;
        }
        count++;
    }
    if (count != deadCount) {
        return 0;
    }
    script->curTrack->pc = (u16)target;
    return 3;
}
