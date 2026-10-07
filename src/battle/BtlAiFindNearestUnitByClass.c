// bdc 0x088932b8 BtlAiFindNearestUnitByClass
#include "bdc.h"

/* Calls unit virtual `slot` (a no-argument predicate) through the GCC 2.x vtable entry. */
static int UnitVirtual(BtlBakugan *unit, int slot)
{
    const VtblEntry *entry = &((const VtblEntry *)unit->base.base.vtable)[slot];

    return ((int (*)(void *))entry->fn)((u8 *)unit + entry->delta);
}

/* Returns the nearest unit (3D distance from the owner) other than the AI's owner (CoreObject id compare)
   that `BtlAi` may target, not dead and without the `respawnProtect` flag, whose class bit is set in `*mask`: 1 = answers
   virtual slot 10 (`+0x50`) and status 9 is inactive, 2 = slot 15 (`+0x78`), 4 = slot 11
   (`+0x58`), 0x10 = slot 16 (`+0x80`), tested in that order; NULL when none. Used by
   `BtlAiUpdateTarget`. */
void *BtlAiFindNearestUnitByClass(BtlAi *self, u32 *mask)
{
    CoreObjectList *list = (CoreObjectList *)BtlGetBakuganList();
    BtlBakugan *unit;
    BtlBakugan *best = NULL;
    float bestDist = 0.0f;
    float dist;
    float dx, dy, dz;
    u32 cls;

    if (list == NULL || list->head == NULL) {
        return NULL;
    }
    for (unit = (BtlBakugan *)list->head; unit != NULL;
         unit = (BtlBakugan *)unit->base.base.next) {
        if (self->owner->base.base.id == unit->base.base.id) {
            continue;
        }
        if (unit->combat.dead != 0 || unit->respawnProtect != 0) {
            continue;
        }
        if (BtlAiIsExcludedTarget(self, unit)) {
            continue;
        }
        if (UnitVirtual(unit, 10) != 0 && unit->combat.status[9].active == 0) {
            cls = 1;
        } else if (UnitVirtual(unit, 15) != 0) {
            cls = 2;
        } else if (UnitVirtual(unit, 11) != 0) {
            cls = 4;
        } else if (UnitVirtual(unit, 16) != 0) {
            cls = 0x10;
        } else {
            cls = 0;
        }
        if ((*mask & cls) == 0) {
            continue;
        }
        /* |owner->pos - unit->pos| over x, y, z */
        dx = self->owner->base.pos[0] - unit->base.pos[0];
        dy = self->owner->base.pos[1] - unit->base.pos[1];
        dz = self->owner->base.pos[2] - unit->base.pos[2];
        dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
        if (best == NULL || dist < bestDist) {
            best = unit;
            bestDist = dist;
        }
    }
    return best;
}
