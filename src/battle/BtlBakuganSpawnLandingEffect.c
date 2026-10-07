// bdc 0x08864c74 BtlBakuganSpawnLandingEffect
#include "bdc.h"

/* Spawns the landing effect at the ground point `groundPoint` pushed ahead by three frames of
   horizontal velocity (`velocity` with y cleared, times 3; w = 0 from the bank zero S713): off
   water (`BtlBakuganIsOnWaterFloor` false) effect 1 when the unit's `hoverHeight` is 0, else 2,
   replaced by 6/7 in arenas 0xc/0xe/0xf (`g_btlArenaIndex`); on a water floor that is stage
   water (`BtlBakuganIsInStageWater`) the point's y is raised by `BtlGetStageWaterHeight` and
   the splash 0x2c is spawned only when `splash` is set. All effects go to `g_worldEffectMgr`
   via `GfxEffectSpawn`. */
void BtlBakuganSpawnLandingEffect(BtlBakugan *self, char splash)
{
    float pos[4];
    int id;

    pos[0] = self->base.velocity[0] * 3.0f + self->groundPoint[0];
    pos[1] = 0.0f * 3.0f + self->groundPoint[1];
    pos[2] = self->base.velocity[2] * 3.0f + self->groundPoint[2];
    pos[3] = 0.0f; /* w lane of C710: bank S713 */

    if (BtlBakuganIsOnWaterFloor(self) == 0) {
        if (self->combat.stats->hoverHeight == 0.0f) {
            id = 1;
            if (g_btlArenaIndex == 0xc || g_btlArenaIndex == 0xe || g_btlArenaIndex == 0xf) {
                id = 6;
            }
        } else {
            id = 2;
            if (g_btlArenaIndex == 0xc || g_btlArenaIndex == 0xe || g_btlArenaIndex == 0xf) {
                id = 7;
            }
        }
        GfxEffectSpawn(g_worldEffectMgr, id, pos);
    } else if (BtlBakuganIsInStageWater(self) != 0) {
        pos[1] = pos[1] + BtlGetStageWaterHeight();
        if (splash != 0) {
            GfxEffectSpawn(g_worldEffectMgr, 0x2c, pos);
        }
    }
}
