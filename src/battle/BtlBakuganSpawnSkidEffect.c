// bdc 0x08864fa0 BtlBakuganSpawnSkidEffect
#include "bdc.h"

/* On dry floor (`BtlBakuganIsOnWaterFloor` false), spawns directed skid-dust effects 4 (5 in
   arenas 0xc/0xe/0xf, `g_btlArenaIndex`) on `g_worldEffectMgr` along the normalised velocity
   (`GfxEffectSpawnDirected`): for kind 10 at the `anchorPos` of each of the four `legs`, else at
   both feet (`BtlBakuganGetFootPosition` 2 then 3, written into the returned static vector),
   always at height `groundY`. A zero velocity gives a zero direction. Called by
   `BtlBakuganState14Update`. */

void BtlBakuganSpawnSkidEffect(BtlBakugan *self)
{
    float dir[4] __attribute__((aligned(16)));
    float pos[4] __attribute__((aligned(16)));
    float *vel;
    float *foot;
    float *next;
    float lenSq;
    float k;
    s32 effectId;
    s32 i;

    if (BtlBakuganIsOnWaterFloor(self) != 0) {
        return;
    }
    /* dir.xyz = velocity.xyz / |velocity| (0 when zero), each clamped to [-1, 1]; dir.w = 0 */
    vel = self->base.velocity;
    lenSq = vel[0] * vel[0];
    lenSq = lenSq + vel[1] * vel[1];
    lenSq = lenSq + vel[2] * vel[2];
    k = VfRsq(lenSq);
    if (lenSq == 0.0f) {
        k = 0.0f;
    }
    dir[0] = VfSat1(vel[0] * k);
    dir[1] = VfSat1(vel[1] * k);
    dir[2] = VfSat1(vel[2] * k);
    dir[3] = 0.0f;
    effectId = 4;
    if (g_btlArenaIndex == 0xc || g_btlArenaIndex == 0xe || g_btlArenaIndex == 0xf) {
        effectId = 5;
    }
    if (self->base.base.unk08 == 10) {
        for (i = 0; i < 4; i++) {
            pos[0] = self->legs[i]->anchorPos[0];
            pos[1] = self->legs[i]->anchorPos[1];
            pos[2] = self->legs[i]->anchorPos[2];
            pos[3] = self->legs[i]->anchorPos[3];
            pos[1] = self->groundY;
            GfxEffectSpawnDirected(g_worldEffectMgr, effectId, pos, dir);
        }
        return;
    }
    foot = BtlBakuganGetFootPosition(self, 2, 0);
    foot[1] = self->groundY;
    GfxEffectSpawnDirected(g_worldEffectMgr, effectId, foot, dir);
    next = BtlBakuganGetFootPosition(self, 3, 0);
    /* both point at the static g_btlFootPos: copy kept as in the asm */
    pos[0] = next[0];
    pos[1] = next[1];
    pos[2] = next[2];
    pos[3] = next[3];
    foot[0] = pos[0];
    foot[1] = pos[1];
    foot[2] = pos[2];
    foot[3] = pos[3];
    foot[1] = self->groundY;
    GfxEffectSpawnDirected(g_worldEffectMgr, effectId, foot, dir);
}
