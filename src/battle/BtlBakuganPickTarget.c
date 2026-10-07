// bdc 0x08862d44 BtlBakuganPickTarget
#include "bdc.h"

/* Returns the unique id of the next target candidate for `self`: collects the candidates of
   `g_btlBakuganList` (same filter as `BtlBakuganCountTargetCandidates`: not itself, not
   dead, status 9 inactive, with mode 1 virtual slot 17 false, for a player unit slots 15 and 16
   false) as `(id, -squared distance)` pairs (positions `pos`, x, y, z only), sorts them
   by descending key, i.e. nearest first (`GfxCombSortByDepth`, only for two or more), and
   returns the entry at the round-robin cursor `targetAux` (reset to 0 when it is at or past the
   end), incrementing the cursor. Returns 0 when there is no candidate. The pair buffer holds 21
   entries, unchecked. */
s32 BtlBakuganPickTarget(BtlBakugan *self, s32 mode)
{
    struct {
        u32 id;
        float depth;
    } pairs[21];
    BtlBakugan *unit;
    const VtblEntry *vtbl;
    s32 count = 0;
    s32 picked = 0;
    float dx;
    float dy;
    float dz;
    float dist;

    for (unit = *(BtlBakugan **)g_btlBakuganList; unit != NULL;
         unit = (BtlBakugan *)unit->base.base.next) {
        if (unit == self || unit->combat.dead != 0 || unit->combat.status[9].active != 0) {
            continue;
        }
        if (mode == 1) {
            vtbl = (const VtblEntry *)unit->base.base.vtable;
            if (((int (*)(void *))vtbl[17].fn)((u8 *)unit + vtbl[17].delta) != 0) {
                continue;
            }
        }
        if (self->isPlayer != 0) {
            vtbl = (const VtblEntry *)unit->base.base.vtable;
            if (((int (*)(void *))vtbl[15].fn)((u8 *)unit + vtbl[15].delta) != 0) {
                continue;
            }
            vtbl = (const VtblEntry *)unit->base.base.vtable;
            if (((int (*)(void *))vtbl[16].fn)((u8 *)unit + vtbl[16].delta) != 0) {
                continue;
            }
        }
        /* |self->pos - unit->pos|^2 over x, y, z */
        dx = self->base.pos[0] - unit->base.pos[0];
        dy = self->base.pos[1] - unit->base.pos[1];
        dz = self->base.pos[2] - unit->base.pos[2];
        dist = dx * dx + dy * dy + dz * dz;
        pairs[count].depth = -dist;
        pairs[count].id = unit->base.base.id;
        count++;
    }
    if (count > 0) {
        if (!(self->targetAux < count)) {
            self->targetAux = 0;
        }
        if (count >= 2) {
            GfxCombSortByDepth(pairs, count);
        }
        picked = pairs[self->targetAux].id;
        self->targetAux = self->targetAux + 1;
    }
    return picked;
}
