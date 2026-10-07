// bdc 0x08830e04 BtlHudUpdateThreatMarkers
#include "bdc.h"

/* HUD widget updater (`BtlHudPhaseMain`) for the "you are targeted" markers: for each of the
   first 3 opponent units (`BtlHudGetEnemyUnit`) whose object id differs from the player
   Bakugan's (`BtlHudGetPlayerBakugan`), that is not `dead` and whose target
   (`BtlBakuganGetTarget`) is the player, shows an off-screen arrow towards the unit's position
   through `BtlHudUpdateEnemyArrow``(hud, mode, pos)` (mode 1, the warning style, when the unit
   is charging (`BtlHudIsUnitCharging`), else 2) and remembers the arrow direction it returned;
   then hides the arrows no unit used (`BtlHudShowEnemyArrow``(hud, i, 0)`) and animates all 4
   (`BtlHudAnimateEnemyArrow`). Nothing happens without a player Bakugan. The charging branch
   copies the unit position a second time, as compiled. */

void BtlHudUpdateThreatMarkers(BtlHud *self)
{
    BtlBakugan *player;
    BtlBakugan *unit;
    float pos[4];
    u8 used[4];
    float arg[4];
    s32 mode;
    s32 dir;
    s32 n;
    s32 i;

    player = (BtlBakugan *)BtlHudGetPlayerBakugan(self);
    pos[2] = 0.0f;
    pos[1] = 0.0f;
    pos[0] = 0.0f;
    pos[3] = 0.0f;
    used[0] = 0;
    used[1] = 0;
    used[2] = 0;
    used[3] = 0;
    if (player == NULL) {
        return;
    }
    for (n = 0; n < 3; n++) {
        unit = (BtlBakugan *)BtlHudGetEnemyUnit(self, n);
        if (unit == NULL || player->base.base.id == unit->base.base.id || unit->combat.dead != 0) {
            continue;
        }
        if (player != BtlBakuganGetTarget(unit)) {
            continue;
        }
        mode = 2;
        memcpy(pos, unit->base.pos, sizeof(pos));
        if (BtlHudIsUnitCharging(self, unit)) {
            mode = 1;
            memcpy(pos, unit->base.pos, sizeof(pos));
        }
        memcpy(arg, pos, sizeof(arg));
        dir = BtlHudUpdateEnemyArrow(self, mode, arg);
        if (dir != -1) {
            used[dir] = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (used[i] == 0) {
            BtlHudShowEnemyArrow(self, i, 0);
        }
    }
    for (i = 0; i < 4; i++) {
        BtlHudAnimateEnemyArrow(self, i);
    }
}
