// bdc 0x0884d900 BtlMainIsBehindOnTimeUp
#include "bdc.h"

/* Time-up comparison. False when script global variable 8 (battle rule mode) is 1. When profile
   word 7 is 1 or 2 it is `BtlMainIsPlayerNotFirst`. Otherwise it compares the local player's HP
   fraction (HP / stat-table HP, `BtlGetPlayerBakugan`) with the opponent's (the first unit of
   the Bakugan list, or the second when the first is the local player): with `orEqual` true when
   opponent < player, else true when !(opponent <= player), i.e. the opponent is strictly ahead
   (also true for NaN). */
bool BtlMainIsBehindOnTimeUp(BtlMain *self, bool orEqual)
{
    BtlBakugan *player;
    BtlBakugan *other;
    float playerFrac;
    float otherFrac;
    u32 mode;

    if (g_scriptGlobalVars[8] == 1) {
        return false;
    }
    mode = SaveProfileGetWord(SaveGetProfile(), 7);
    if ((s32)mode > 0 && (s32)mode < 3) {
        return BtlMainIsPlayerNotFirst(self);
    }
    player = (BtlBakugan *)BtlGetPlayerBakugan();
    playerFrac = BtlCombatGetHp(&player->combat);
    player = (BtlBakugan *)BtlGetPlayerBakugan();
    playerFrac = playerFrac / player->combat.stats->levelHp[4];
    other = (BtlBakugan *)((CoreObjectList *)BtlGetBakuganList())->head;
    if (BtlBakuganIsLocalPlayer(other)) {
        other = (BtlBakugan *)other->base.base.next;
    }
    otherFrac = BtlCombatGetHp(&other->combat) / other->combat.stats->levelHp[4];
    if (orEqual) {
        return otherFrac < playerFrac;
    }
    return !(otherFrac <= playerFrac);
}
