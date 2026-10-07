// bdc 0x08883804 BtlAttackUpdateProjectile
#include "bdc.h"

/* Shared per-frame body of the simple projectile attack types (called by the thin
   `BtlAttackType<NN>Update` wrappers with constant arguments). Past `lifetime` it ends
   (`BtlAttackEnd`) and stops the trail effect `trailEffect` at `pos` on `effectMgr` (-1 = none).
   On later frames it first resolves clashes (`BtlAttackResolveClash` with `hitEffect`; returns
   when that handled the attack) and sweeps for hits (`BtlAttackSweepHit` radius, pos -> vel,
   `hitKind`, kind 3, mask 0x31bf337e): with `flagB` a hit becomes a pending hit at
   `g_btlAttackHitPoint` (`BtlAttackSetPendingHit`); without it a hit spawns `hitEffect` at
   `pos`, ends and stops the trail. Movement: with `flagA` it homes
   (`BtlAttackSteerToTarget(speed, maxSpeed, self, 1, NULL)`) and adds `vel` to `pos`, except on
   frame 0. Without `flagA`, frame 0 aims a side-launched shot (`paramF2 != 0`) at the target unit
   (`targetId`, `BtlFindBakuganById`): dir = normalize((target + (0, 80, 0)) - (owner + (0,
   paramF2, 0))) (w = 0, a zero vector stays zero), copied to `vel` and the attached effect's
   `dir`; then it scales `vel.xyz` by `speed` (w = 0); frame 1 rescales `vel.xyz` by `speed` when
   its length is below 10; every frame from 1 on adds `vel` to `pos`. `sound` is unused. */
void BtlAttackUpdateProjectile(float speed, float maxSpeed, BtlAttack *self, s32 hitKind,
                               s32 hitEffect, void *effectMgr, s32 trailEffect, s32 lifetime,
                               char flagA, s32 sound, char flagB)
{
    float targetY;
    float lenSq;
    float len;
    float k;
    BtlBakugan *unit;
    BtlBakugan *owner;
    GfxEffect *effect;

    (void)sound;
    if (self->age > lifetime) {
        BtlAttackEnd(self);
        if (trailEffect != -1) {
            GfxEffectStopAttached(effectMgr, trailEffect, self->pos);
        }
        return;
    }
    if (self->age != 0) {
        if (BtlAttackResolveClash(self, hitEffect) != 0) {
            return;
        }
        if (flagB != 0) {
            if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitKind, 3, 0,
                                  0x31bf337e) != 0) {
                BtlAttackSetPendingHit(self, hitEffect, &g_btlAttackHitPoint.x);
                return;
            }
        } else {
            if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitKind, 3, 0,
                                  0x31bf337e) != 0) {
                GfxEffectSpawn(effectMgr, hitEffect, self->pos);
                BtlAttackEnd(self);
                if (trailEffect != -1) {
                    GfxEffectStopAttached(effectMgr, trailEffect, self->pos);
                }
                return;
            }
        }
    }
    if (flagA != 0) {
        if (self->age != 0) {
            BtlAttackSteerToTarget(speed, maxSpeed, self, 1, NULL);
            /* pos.xyz += vel.xyz (w kept) */
            self->pos[0] = self->pos[0] + self->vel[0];
            self->pos[1] = self->pos[1] + self->vel[1];
            self->pos[2] = self->pos[2] + self->vel[2];
        }
        return;
    }
    if (self->age == 0) {
        if (self->paramF2 != 0.0f) {
            unit = (BtlBakugan *)BtlFindBakuganById(self->targetId);
            if (unit != NULL) {
                owner = self->owner;
                targetY = unit->base.pos[1] + 80.0f;
                /* dir = (target + (0, 80, 0)) - (owner + (0, paramF2, 0)); w = target w */
                self->dir[0] = unit->base.pos[0] - owner->base.pos[0];
                self->dir[1] = targetY - (owner->base.pos[1] + self->paramF2);
                self->dir[2] = unit->base.pos[2] - owner->base.pos[2];
                /* dir = normalize(dir), clamped to [-1, 1], w = 0 */
                lenSq = self->dir[0] * self->dir[0] + self->dir[1] * self->dir[1] +
                        self->dir[2] * self->dir[2];
                k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
                self->dir[0] = VfSat1(self->dir[0] * k);
                self->dir[1] = VfSat1(self->dir[1] * k);
                self->dir[2] = VfSat1(self->dir[2] * k);
                self->dir[3] = 0.0f;
                self->vel[0] = self->dir[0];
                self->vel[1] = self->dir[1];
                self->vel[2] = self->dir[2];
                self->vel[3] = self->dir[3];
                if (self->effect != NULL) {
                    effect = (GfxEffect *)self->effect;
                    effect->dir[0] = self->vel[0];
                    effect->dir[1] = self->vel[1];
                    effect->dir[2] = self->vel[2];
                    effect->dir[3] = self->vel[3];
                }
            }
        }
        /* vel.xyz *= speed; vel.w = 0 */
        self->vel[0] = self->vel[0] * speed;
        self->vel[1] = self->vel[1] * speed;
        self->vel[2] = self->vel[2] * speed;
        self->vel[3] = 0.0f;
        return;
    }
    if (self->age == 1) {
        len = __builtin_sqrtf(self->vel[0] * self->vel[0] + self->vel[1] * self->vel[1] +
                              self->vel[2] * self->vel[2]);
        if (len < 10.0f) {
            /* vel.xyz *= speed; vel.w = 0 */
            self->vel[0] = self->vel[0] * speed;
            self->vel[1] = self->vel[1] * speed;
            self->vel[2] = self->vel[2] * speed;
            self->vel[3] = 0.0f;
        }
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}
