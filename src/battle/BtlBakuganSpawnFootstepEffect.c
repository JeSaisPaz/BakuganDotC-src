// bdc 0x08864dd4 BtlBakuganSpawnFootstepEffect
#include "bdc.h"

/* Spawns a footstep effect on `g_worldEffectMgr` at foot `foot` (`BtlBakuganGetFootPosition`;
   the same call for kind 10 and the others), with its height replaced by the ground height
   `groundY`, moved `back` units against the active camera's view direction (`dir` of
   `g_gfxActiveCamera`) and `ahead` frames along the velocity (each step skipped when its factor
   is 0). Dry floor: dust 3 (8 in arenas 0xc/0xe/0xf, `g_btlArenaIndex`). Water floor
   (`BtlBakuganIsOnWaterFloor`): only on stages 0xc..0xf (`BtlBakuganIsInWaterStage0CTo0F`),
   effect 8, or 0x105 raised to the water surface (`groundY` + `BtlGetStageWaterHeight`) when
   the surface is not at or below the unit's `pos.y`. */
void BtlBakuganSpawnFootstepEffect(float back, float ahead, BtlBakugan *self, int foot)
{
    float point[4] __attribute__((aligned(16)));
    float step[3];
    float *footPos;
    float *src;
    float level;
    s32 effect;

    if (self->base.base.unk08 == 10) {
        footPos = BtlBakuganGetFootPosition(self, foot, 0);
    } else {
        footPos = BtlBakuganGetFootPosition(self, foot, 0);
    }
    point[0] = footPos[0];
    point[1] = footPos[1];
    point[2] = footPos[2];
    point[3] = footPos[3];
    point[1] = self->groundY;
    if (back != 0.0f) {
        /* point.xyz -= camera dir * back */
        src = g_gfxActiveCamera->dir;
        step[0] = src[0] * back;
        step[1] = src[1] * back;
        step[2] = src[2] * back;
        point[0] = point[0] - step[0];
        point[1] = point[1] - step[1];
        point[2] = point[2] - step[2];
    }
    effect = 3;
    if (ahead != 0.0f) {
        /* point.xyz += velocity * ahead */
        src = self->base.velocity;
        step[0] = src[0] * ahead;
        step[1] = src[1] * ahead;
        step[2] = src[2] * ahead;
        point[0] = point[0] + step[0];
        point[1] = point[1] + step[1];
        point[2] = point[2] + step[2];
    }
    if (BtlBakuganIsOnWaterFloor(self) == 0) {
        if (g_btlArenaIndex == 0xc || g_btlArenaIndex == 0xe || g_btlArenaIndex == 0xf) {
            effect = 8;
        }
        GfxEffectSpawn(g_worldEffectMgr, effect, point);
        return;
    }
    effect = 8;
    if (BtlBakuganIsInWaterStage0CTo0F(self) == 0) {
        return;
    }
    level = self->groundY + BtlGetStageWaterHeight();
    if (!(level <= self->base.pos[1])) {
        effect = 0x105;
        point[1] = level;
    }
    GfxEffectSpawn(g_worldEffectMgr, effect, point);
}
