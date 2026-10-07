// bdc 0x0884e0b8 BtlMainGetTeamOutcome
#include "bdc.h"

/* Calls unit virtual `slot` (a no-argument predicate) through the GCC 2.x vtable entry. */
static int UnitVirtual(BtlBakugan *unit, int slot)
{
    const VtblEntry *entry = &((const VtblEntry *)unit->base.base.vtable)[slot];

    return ((int (*)(void *))entry->fn)((u8 *)unit + entry->delta);
}

/* Outcome of the battle for team `team` (unit `playerSlot`): 1 win, 2 loss, 4 draw; 0 in rule
   mode 1 (script global 8), 1 when there is no unit list. Multi-round results come first
   (`BtlMainIsMatchDrawn` → 4, then `BtlMainIsMatchWon` / `BtlMainIsMatchLost`, judged from
   the player unit's slot). Otherwise, with profile word 2 == 0, the HP fractions
   (`BtlCombatGetHp` / `levelHp[4]`) of a unit of `team` and a unit of another team are compared:
   equal → 4, other lower → 1, else 2. With word 2 != 0, every unit passing virtual 10 or 12 is
   checked: a living one of `team` gives 1, a dead one of `team` gives 2 (the last such unit
   wins), and if no checked unit is alive the result is 4. */
int BtlMainGetTeamOutcome(BtlMain *self, int team)
{
    void *list = BtlGetBakuganList();
    BtlBakugan *first;
    BtlBakugan *unit;
    float mine;
    float other;
    int outcome;
    bool noneAlive;

    if (g_scriptGlobalVars[8] == 1) {
        return 0;
    }
    if (list == NULL) {
        return 1;
    }
    first = *(BtlBakugan **)list;
    if (SaveProfileGetWord(SaveGetProfile(), 2) == 0) {
        if (BtlMainIsMatchDrawn(self)) {
            return 4;
        }
        if (BtlMainIsMatchWon(self)) {
            outcome = 1;
            if (((BtlBakugan *)BtlGetPlayerBakugan())->playerSlot != team) {
                outcome = 2;
            }
            return outcome;
        }
        if (BtlMainIsMatchLost(self)) {
            outcome = 2;
            if (((BtlBakugan *)BtlGetPlayerBakugan())->playerSlot != team) {
                outcome = 1;
            }
            return outcome;
        }
        /* The binary only ever steps to the second unit; with team < 2 it spins if that unit
           does not match either. */
        unit = first;
        if (unit->playerSlot != team) {
            do {
                unit = (BtlBakugan *)first->base.base.next;
            } while (team < 2 && unit->playerSlot != team);
        }
        mine = BtlCombatGetHp(&unit->combat) / unit->combat.stats->levelHp[4];
        unit = first;
        if (unit->playerSlot == team) {
            do {
                unit = (BtlBakugan *)first->base.base.next;
            } while (team < 2 && unit->playerSlot == team);
        }
        other = BtlCombatGetHp(&unit->combat) / unit->combat.stats->levelHp[4];
        if (other == mine) {
            return 4;
        }
        outcome = 2;
        if (other < mine) {
            outcome = 1;
        }
        return outcome;
    }

    outcome = 2;
    if (BtlMainIsMatchDrawn(self)) {
        return 4;
    }
    if (BtlMainIsMatchWon(self)) {
        outcome = 1;
        if (((BtlBakugan *)BtlGetPlayerBakugan())->playerSlot != team) {
            outcome = 2;
        }
        return outcome;
    }
    if (BtlMainIsMatchLost(self)) {
        outcome = 2;
        if (((BtlBakugan *)BtlGetPlayerBakugan())->playerSlot != team) {
            outcome = 1;
        }
        return outcome;
    }
    noneAlive = true;
    for (unit = first; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
        if (UnitVirtual(unit, 10) != 0 || UnitVirtual(unit, 12) != 0) {
            if (unit->combat.dead != 0) {
                if (unit->playerSlot == team) {
                    outcome = 2;
                }
            } else {
                if (unit->playerSlot == team) {
                    outcome = 1;
                }
                noneAlive = false;
            }
        }
    }
    if (noneAlive) {
        outcome = 4;
    }
    return outcome;
}
