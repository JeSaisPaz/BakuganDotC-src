// bdc 0x0886696c BtlBakuganSpawnWaterEffects
#include "bdc.h"

/* Water-contact effects of a Bakugan, one effect set per kind of water stage; the surface height
   is `groundPoint[1]` plus `BtlGetStageWaterHeight`, and each set runs while the unit is in that
   kind of water and below the surface (else its exit branch runs). For every set the ripple period
   N = clamp((180 - 6.5 * horizontal speed) * 0.1, 3, 15) and the effect position is the ground
   point at surface height plus twice the horizontal velocity.
   Real water (`BtlBakuganIsInStageWater`, flag `inWater`): entry splash 0x29 on the first frame,
   else ripples 0x2a every N frames (`frameCounter`); on the first frame a big splash 0x2c with kind
   sound 10 (`BtlBakuganPlayKindSound`) when falling faster than 8 after more than 8 airborne
   frames; the wake 0x2b (`GfxEffectSpawnDirected`, direction 0.1 * velocity) on the first frame
   or every N frames when N < 10, and when N < 10 also 0x2e along the normalised direction (each
   component saturated to [-1, 1]); sets the flag. Leaving it with the flag set spawns the exit
   splash 0x2d at the surface when rising faster than 10; clears the flag.
   Stages 0xc..0xf (`BtlBakuganIsInWaterStage0CTo0F`, flag `slowWalk`): only the big splash 0x35
   (no sound), the wake 0x34 and, on exit, 0x36 and 6.
   Stages 0x12..0x1b (`BtlBakuganIsInWaterStage12To1B`, flag `inWater12To1B`): entry 0x32,
   ripples 0x33, big splash 0x30 with sound 10, wake 0x2f, exit 0x31, plus every 4 frames (input
   enabled, virtual at vtable entry 20 nonzero, kind not 0xf) the footstep 0x107 at the left (2) or
   right (3) foot (`BtlBakuganGetFootPosition`) at ground height.
   A zero-length 0x2e direction gets the bank constant S713 (0) as its scale; every scaled direction
   gets w = 0 (lane S713 of C710 is stored with it). */

void BtlBakuganSpawnWaterEffects(BtlBakugan *self)
{
    float dir[4];
    float pos[4];
    float exitPos[4];
    float dir2[4];
    float pos2[4];
    float exitPos2[4];
    float dir3[4];
    float pos3[4];
    float exitPos3[4];
    float height;
    float surface;
    float speed;
    float period;
    float lenSq;
    float scale;
    int frames;
    int i;
    GfxEffectMgr *mgr;
    const VtblEntry *entry;
    float *foot;

    height = BtlGetStageWaterHeight();
    surface = self->groundPoint[1] + height;

    /* real water */
    if (BtlBakuganIsInStageWater(self) != 0 && !(surface <= self->base.pos[1])) {
        /* horizontal speed: length of velocity with y zeroed */
        speed = __builtin_sqrtf(self->base.velocity[0] * self->base.velocity[0] + 0.0f * 0.0f +
                                self->base.velocity[2] * self->base.velocity[2]);
        period = (180.0f - speed * 6.5f) * 0.1f;
        if (period < 3.0f) {
            frames = 3;
        } else if (period <= 15.0f) {
            frames = (int)period;
        } else {
            frames = 15;
        }
        for (i = 0; i < 4; i++) {
            dir[i] = self->base.velocity[i];
        }
        dir[1] = 0.0f;
        for (i = 0; i < 4; i++) {
            pos[i] = self->groundPoint[i];
        }
        pos[1] = surface;
        /* dir *= 2 (w = S713 = 0); pos.xyz += dir */
        for (i = 0; i < 3; i++) {
            dir[i] = dir[i] * 2.0f;
        }
        dir[3] = 0.0f;
        for (i = 0; i < 3; i++) {
            pos[i] = pos[i] + dir[i];
        }
        if (self->inWater == 0) {
            GfxEffectSpawn(g_worldEffectMgr, 0x29, pos);
        } else if (self->frameCounter % frames == 0) {
            GfxEffectSpawn(g_worldEffectMgr, 0x2a, pos);
        }
        if (self->inWater == 0 && self->base.velocity[1] < -8.0f && self->airborneFrames > 8) {
            GfxEffectSpawn(g_worldEffectMgr, 0x2c, pos);
            BtlBakuganPlayKindSound(self, 10, 0, 0);
        }
        if ((frames < 10 && self->frameCounter % frames == 0) || self->inWater == 0) {
            /* dir *= 0.1 (w = S713 = 0) */
            for (i = 0; i < 3; i++) {
                dir[i] = dir[i] * 0.1f;
            }
            dir[3] = 0.0f;
            GfxEffectSpawnDirected(g_worldEffectMgr, 0x2b, pos, dir);
        }
        if (frames < 10) {
            mgr = g_worldEffectMgr;
            /* dir = normalise(dir), scale S713 (0) if zero length, saturated to [-1, 1] */
            lenSq = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
            scale = VfRsq(lenSq);
            if (lenSq == 0.0f) {
                scale = 0.0f;
            }
            for (i = 0; i < 3; i++) {
                dir[i] = VfSat1(dir[i] * scale);
            }
            dir[3] = 0.0f;
            GfxEffectSpawnDirected(mgr, 0x2e, pos, dir);
        }
        self->inWater = 1;
    } else {
        if (self->inWater != 0 && !(self->base.velocity[1] <= 10.0f)) {
            for (i = 0; i < 4; i++) {
                exitPos[i] = self->groundPoint[i];
            }
            exitPos[1] = surface;
            GfxEffectSpawn(g_worldEffectMgr, 0x2d, exitPos);
        }
        self->inWater = 0;
    }

    /* stages 0xc..0xf */
    if (BtlBakuganIsInWaterStage0CTo0F(self) != 0 && !(surface <= self->base.pos[1])) {
        speed = __builtin_sqrtf(self->base.velocity[0] * self->base.velocity[0] + 0.0f * 0.0f +
                                self->base.velocity[2] * self->base.velocity[2]);
        period = (180.0f - speed * 6.5f) * 0.1f;
        if (period < 3.0f) {
            frames = 3;
        } else if (period <= 15.0f) {
            frames = (int)period;
        } else {
            frames = 15;
        }
        for (i = 0; i < 4; i++) {
            dir2[i] = self->base.velocity[i];
        }
        dir2[1] = 0.0f;
        for (i = 0; i < 4; i++) {
            pos2[i] = self->groundPoint[i];
        }
        pos2[1] = surface;
        for (i = 0; i < 3; i++) {
            dir2[i] = dir2[i] * 2.0f;
        }
        dir2[3] = 0.0f;
        for (i = 0; i < 3; i++) {
            pos2[i] = pos2[i] + dir2[i];
        }
        if (self->slowWalk == 0 && self->base.velocity[1] < -8.0f && self->airborneFrames > 8) {
            GfxEffectSpawn(g_worldEffectMgr, 0x35, pos2);
        }
        if ((frames < 10 && self->frameCounter % frames == 0) || self->slowWalk == 0) {
            for (i = 0; i < 3; i++) {
                dir2[i] = dir2[i] * 0.1f;
            }
            dir2[3] = 0.0f;
            GfxEffectSpawnDirected(g_worldEffectMgr, 0x34, pos2, dir2);
        }
        self->slowWalk = 1;
    } else {
        if (self->slowWalk != 0 && !(self->base.velocity[1] <= 10.0f)) {
            for (i = 0; i < 4; i++) {
                exitPos2[i] = self->groundPoint[i];
            }
            exitPos2[1] = surface;
            GfxEffectSpawn(g_worldEffectMgr, 0x36, exitPos2);
            GfxEffectSpawn(g_worldEffectMgr, 6, exitPos2);
        }
        self->slowWalk = 0;
    }

    /* stages 0x12..0x1b */
    if (BtlBakuganIsInWaterStage12To1B(self) != 0 && !(surface <= self->base.pos[1])) {
        speed = __builtin_sqrtf(self->base.velocity[0] * self->base.velocity[0] + 0.0f * 0.0f +
                                self->base.velocity[2] * self->base.velocity[2]);
        period = (180.0f - speed * 6.5f) * 0.1f;
        if (period < 3.0f) {
            frames = 3;
        } else if (period <= 15.0f) {
            frames = (int)period;
        } else {
            frames = 15;
        }
        for (i = 0; i < 4; i++) {
            dir3[i] = self->base.velocity[i];
        }
        dir3[1] = 0.0f;
        for (i = 0; i < 4; i++) {
            pos3[i] = self->groundPoint[i];
        }
        pos3[1] = surface;
        for (i = 0; i < 3; i++) {
            dir3[i] = dir3[i] * 2.0f;
        }
        dir3[3] = 0.0f;
        for (i = 0; i < 3; i++) {
            pos3[i] = pos3[i] + dir3[i];
        }
        if (self->inWater12To1B == 0) {
            GfxEffectSpawn(g_worldEffectMgr, 0x32, pos3);
        } else if (self->frameCounter % frames == 0) {
            GfxEffectSpawn(g_worldEffectMgr, 0x33, pos3);
        }
        if (self->inWater12To1B == 0 && self->base.velocity[1] < -8.0f &&
            self->airborneFrames > 8) {
            GfxEffectSpawn(g_worldEffectMgr, 0x30, pos3);
            BtlBakuganPlayKindSound(self, 10, 0, 0);
        }
        if ((frames < 10 && self->frameCounter % frames == 0) || self->inWater12To1B == 0) {
            for (i = 0; i < 3; i++) {
                dir3[i] = dir3[i] * 0.1f;
            }
            dir3[3] = 0.0f;
            GfxEffectSpawnDirected(g_worldEffectMgr, 0x2f, pos3, dir3);
        }
        if (self->input->disabled == 0) {
            entry = &((const VtblEntry *)self->base.base.vtable)[20];
            if (((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0 &&
                self->base.base.unk08 != 0xf && (self->frameCounter & 3) == 0) {
                foot = BtlBakuganGetFootPosition(self, (self->frameCounter & 4) != 0 ? 2 : 3, 0);
                for (i = 0; i < 4; i++) {
                    pos3[i] = foot[i];
                }
                pos3[1] = self->groundPoint[1];
                GfxEffectSpawn(g_worldEffectMgr, 0x107, pos3);
            }
        }
        self->inWater12To1B = 1;
    } else {
        if (self->inWater12To1B != 0 && !(self->base.velocity[1] <= 10.0f)) {
            for (i = 0; i < 4; i++) {
                exitPos3[i] = self->groundPoint[i];
            }
            exitPos3[1] = surface;
            GfxEffectSpawn(g_worldEffectMgr, 0x31, exitPos3);
        }
        self->inWater12To1B = 0;
    }
}
