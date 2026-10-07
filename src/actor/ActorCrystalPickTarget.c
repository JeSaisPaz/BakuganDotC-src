// bdc 0x08858f8c ActorCrystalPickTarget
#include "bdc.h"

/* Target for the crystal's shots. In ranked score battles (script variable 8 == 2 and profile
   word 7 == 1) it walks `g_btlBakuganList` and takes the nearest unit that is not dead
   (`combat.dead`), not respawn-protected and whose vtable entry 10 reports it active (start
   distance +inf, strict `<`, so the first of equal distances wins); it returns NULL when the list
   is missing or empty. Otherwise it takes the slot-0 player Bakugan
   (`BtlFindPlayerBakuganBySlot`). The chosen unit is returned unless it is dead or
   respawn-protected, then NULL. As in the binary, no candidate found (or no slot-0 unit) is
   dereferenced without a NULL check. */

void *ActorCrystalPickTarget(ActorCrystal *self)
{
    BtlBakugan *target = NULL;
    BtlBakugan **list;
    BtlBakugan *unit;
    const VtblEntry *vtbl;
    float best;
    float dist;
    float dx;
    float dy;
    float dz;
    u8 protect;

    if (g_scriptGlobalVars[8] == 2 &&
        SaveProfileGetWord(SaveGetProfile(), 7) == 1) {
        best = __builtin_inff();
        list = (BtlBakugan **)BtlGetBakuganList();
        if (list == NULL || *list == NULL) {
            return NULL;
        }
        for (unit = *list; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
            if (unit->combat.dead != 0) {
                continue;
            }
            if (unit->respawnProtect != 0) {
                continue;
            }
            vtbl = &((const VtblEntry *)unit->base.base.vtable)[10];
            if (((s32 (*)(void *))vtbl->fn)((u8 *)unit + vtbl->delta) == 0) {
                continue;
            }
            /* dist = |self->pos - unit->pos| (xyz) */
            dx = self->base.base.pos[0] - unit->base.pos[0];
            dy = self->base.base.pos[1] - unit->base.pos[1];
            dz = self->base.base.pos[2] - unit->base.pos[2];
            dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
            if (dist < best) {
                best = dist;
                target = unit;
            }
        }
        protect = target->respawnProtect;
    } else {
        target = (BtlBakugan *)BtlFindPlayerBakuganBySlot(0);
        protect = target->respawnProtect;
    }
    if ((target->combat.dead | (protect != 0)) != 0) {
        return NULL;
    }
    return target;
}
