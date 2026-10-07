// bdc 0x0887a184 BtlAttackUpdateBeam
#include "bdc.h"

/* Shared body of the beam attack types (`BtlAttackType01Update`, `BtlAttackType0BUpdate`,
   `BtlAttackType15Update`, `BtlAttackType3BUpdate`, `BtlAttackType29Update`,
   `BtlAttackType2AUpdate`, `BtlAttackType59Update`); `duration` is the age at which the beam
   starts fading.
   Phase 0: length `paramF0` = 0, width `paramF1` = 20, `auxVec` = `pos`; a `vel` pitch beyond ±45°
   (`atan2f`) is rotated back to ±45° about the horizontal axis; spawns the second effect `effectId`
   (`GfxEffectSpawnAttachedDir` at `auxVec` along `vel`) into `effect2` with the owner and its id,
   runs its vtable entry 2 and advances to phase 1.
   Phases 0..2: past `duration + 5` the second effect is released (`UiSpriteLayerRelease`, the main
   effect's `f1f4` cleared). Past `duration - 5` the width shrinks by 0.5 and the main effect's alpha
   by 0.08 per frame, and at width <= 0.1 the attack ends (`BtlAttackEnd`) and returns; before
   that the width eases toward `width` (×0.2 per frame) and an owner in states 3..6 or with flag
   0x20 sets `age = duration`.
   Every frame: the effects take the width; before `duration` the length eases toward 336 (capped at
   280); `mtx` = owner root matrix × `bone` local matrix and `pos` its translation (type 0x2a: +40
   up and 30 × `vel` forward); `segment` = `vel` × length × 10; the effects point along `vel`
   (`effect2` reversed). Before `duration` the beam is clipped against the units of
   `g_btlBakuganList` whose vtable entry 11 or 19 is true: `g_collisionSweptSphereDesc` (start
   `pos` 50 lower, `segment`, radius `width * 3.6`) is tested against each body collider
   (`CollisionTestShape`); a hit sets length = clamp(distance × 0.1 + 3, 10, 280). Then the beam
   sweeps for hits (`BtlAttackSweepHitSound`, radius `width * 3`, kind `hitKind`). A hit on a
   collider whose owner kind is 1..0x83 with an attach matrix spawns `unitHitFx` (when `param1 & m`
   is 0, m = 3 in a net game with `SaveGetProfileFlag0` set, else 0): at the closest point of the
   beam to the anchor (`CollisionSegmentClosestPoint`, `GfxEffectSpawnDirected`) for a unit
   whose entry 11/19 is true, else attached to the anchor; offset = (`pos` - that point) scaled to
   the unit's reach radius; units of kind < 0x21 with flag 0x10 set the length to
   clamp(hit distance × 0.1 + 5, 10, 280) and scale `segment` (not `vel`) by length × 10; `param1`
   counts these hits. Any other hit spawns `worldHitFx` (every other `param0`) 3 past the hit point
   along the contact normal, oriented to the normal (for a vertical normal a fixed basis with y and z
   swapped), and counts `param0`. With `width` < 18, a hit that is not on such a unit also does the
   `+ 5` length clip. Finally the main effect's `size[1]` takes the length, `auxVec` = `pos +
   segment`, and `BtlAttackCheckClash` runs (result ignored). */
void BtlAttackUpdateBeam(float width, BtlAttack *self, s32 effectId, s32 unitHitFx, s32 worldHitFx, s32 hitKind, s32 duration)
{
    float hit[4];        /* sp+0x10 */
    float axis[4];       /* sp+0x30 */
    float quat[4];       /* sp+0x40 */
    float prod[4];       /* quat * vel */
    float rotated[4];    /* sp+0x20 */
    float offset[4];     /* sp+0x90 */
    float t;
    ScePspFVector4 closest;  /* sp+0xb0 */
    float spot[4];       /* sp+0xc0 */
    float ra[3];
    float rb[3];
    float rc[3];
    float re[3];
    float *root;
    float *local;
    float *n;
    float *d;
    float *m;
    CollisionSweptSphereDesc *desc;
    CollisionCollider *collider;
    BtlBakugan *owner;
    BtlBakugan *unit;
    GfxEffect *fx;
    const VtblEntry *entry;
    float *anchor;
    float pitch;
    float limit;
    float len;
    float dist;
    float scale;
    float inv;
    float reach;
    float s;
    float c;
    float vx;
    float vy;
    float vz;
    float vw;
    float dx;
    float dy;
    float dz;
    float ia;
    float ib;
    float ic;
    s32 phase;
    s32 kind;
    u32 mask;
    int vertical;
    int i;
    int j;

    phase = self->phase;
    if (phase > 0) {
        if (phase >= 3) {
            goto track;
        }
        goto fade;
    }
    if (phase < 0) {
        goto track;
    }

    /* phase 0: spawn */
    self->paramF0 = 0.0f;
    self->paramF1 = 20.0f;
    __builtin_memcpy(self->auxVec, self->pos, sizeof self->auxVec);
    pitch = atan2f(self->vel[1],
                   __builtin_sqrtf(self->vel[0] * self->vel[0] + self->vel[2] * self->vel[2]));
    if (!(ABS(pitch) <= 0.785398185f)) {
        if (pitch <= 0.0f) {
            limit = -0.785398185f - pitch;
        } else {
            limit = 0.785398185f - pitch;
        }
        /* axis = (-vel.z, vel.y, vel.x, vel.w), then axis.y = 0 */
        axis[0] = -self->vel[2];
        axis[1] = self->vel[1];
        axis[2] = self->vel[0];
        axis[3] = self->vel[3];
        axis[1] = 0.0f;
        /* axis.xyz = clamp(normalise(axis), -1, 1) (0 for a zero vector), axis.w = 0 (S713) */
        dist = axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2];
        inv = VfRsq(dist);
        if (dist == 0.0f) {
            inv = 0.0f;
        }
        axis[0] = VfSat1(axis[0] * inv);
        axis[1] = VfSat1(axis[1] * inv);
        axis[2] = VfSat1(axis[2] * inv);
        axis[3] = 0.0f;
        /* quat = (axis * sin, cos) of `limit / pi` quarter turns (half the angle) */
        scale = 0.318309873f * limit;
        c = VfCosQuarter(scale);
        s = VfSinQuarter(scale);
        quat[0] = axis[0] * s;
        quat[1] = axis[1] * s;
        quat[2] = axis[2] * s;
        quat[3] = c;
        /* vel = quat * (vel.xyz, 0 (S730)) * conj(quat) */
        vx = self->vel[0];
        vy = self->vel[1];
        vz = self->vel[2];
        vw = 0.0f;
        prod[0] = quat[0] * vw + quat[1] * vz - quat[2] * vy + quat[3] * vx;
        prod[1] = -quat[0] * vz + quat[1] * vw + quat[2] * vx + quat[3] * vy;
        prod[2] = quat[0] * vy - quat[1] * vx + quat[2] * vw + quat[3] * vz;
        prod[3] = -quat[0] * vx - quat[1] * vy - quat[2] * vz + quat[3] * vw;
        vx = -quat[0];
        vy = -quat[1];
        vz = -quat[2];
        vw = quat[3];
        rotated[0] = prod[0] * vw + prod[1] * vz - prod[2] * vy + prod[3] * vx;
        rotated[1] = -prod[0] * vz + prod[1] * vw + prod[2] * vx + prod[3] * vy;
        rotated[2] = prod[0] * vy - prod[1] * vx + prod[2] * vw + prod[3] * vz;
        rotated[3] = -prod[0] * vx - prod[1] * vy - prod[2] * vz + prod[3] * vw;
        __builtin_memcpy(self->vel, rotated, sizeof self->vel);
    }
    fx = (GfxEffect *)GfxEffectSpawnAttachedDir(g_btlAttackEffectMgr, effectId, self->auxVec,
                                                self->vel);
    owner = self->owner;
    self->effect2 = fx;
    fx->ownerBakugan = owner;
    if (owner != NULL) {
        fx->ownerId = owner->base.base.id;
    }
    fx = (GfxEffect *)self->effect2;
    entry = &((const VtblEntry *)fx->base.vtable)[2];
    ((void (*)(void *))entry->fn)((u8 *)fx + entry->delta);
    self->phase = self->phase + 1;

fade:
    if (duration + 5 < self->age && self->effect2 != NULL) {
        fx = (GfxEffect *)self->effect2;
        UiSpriteLayerRelease(fx->mgr, fx);
        self->effect2 = NULL;
        ((GfxEffect *)self->effect)->f1f4 = 0.0f;
    }
    if (duration - 5 < self->age) {
        self->paramF1 = self->paramF1 - 0.5f;
        ((GfxEffect *)self->effect)->color[3] = ((GfxEffect *)self->effect)->color[3] - 0.0799999982f;
        if (self->paramF1 <= 0.100000001f) {
            BtlAttackEnd(self);
            return;
        }
    } else {
        self->paramF1 = self->paramF1 + (width - self->paramF1) * 0.200000003f;
        if ((self->owner->state >= 3 && self->owner->state < 7) ||
            (self->owner->flags & 0x20) != 0) {
            self->age = duration;
        }
    }

track:
    ((GfxEffect *)self->effect)->vec1d0[0] = self->paramF1;
    if (self->age < duration) {
        len = self->paramF0 + (336.0f - self->paramF0) * 0.200000003f;
        self->paramF0 = len;
        if (!(len <= 280.0f)) {
            self->paramF0 = 280.0f;
        }
    }
    if (self->effect2 != NULL) {
        ((GfxEffect *)self->effect2)->vec1d0[0] = self->paramF1;
    }
    /* mtx = owner root matrix x bone local matrix (column-major); pos = mtx row 3 */
    root = self->owner->base.data->rootMatrix;
    local = self->bone->localMatrix;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            self->mtx[j][i] = root[0 * 4 + i] * local[j * 4 + 0]
                              + root[1 * 4 + i] * local[j * 4 + 1]
                              + root[2 * 4 + i] * local[j * 4 + 2]
                              + root[3 * 4 + i] * local[j * 4 + 3];
        }
    }
    __builtin_memcpy(self->pos, self->mtx[3], sizeof self->pos);
    if (self->type == 0x2a) {
        self->pos[1] = self->pos[1] + 40.0f;
        /* pos.xyz += vel.xyz * 30 */
        self->pos[0] = self->pos[0] + self->vel[0] * 30.0f;
        self->pos[1] = self->pos[1] + self->vel[1] * 30.0f;
        self->pos[2] = self->pos[2] + self->vel[2] * 30.0f;
    }
    /* segment = (vel.xyz * length * 10, 0 (S713)) */
    scale = self->paramF0 * 10.0f;
    self->segment[0] = self->vel[0] * scale;
    self->segment[1] = self->vel[1] * scale;
    self->segment[2] = self->vel[2] * scale;
    self->segment[3] = 0.0f;
    if (self->effect2 != NULL) {
        fx = (GfxEffect *)self->effect2;
        fx->dir[0] = -self->vel[0];
        fx->dir[1] = -self->vel[1];
        fx->dir[2] = -self->vel[2];
        fx->dir[3] = self->vel[3];
    }
    __builtin_memcpy(((GfxEffect *)self->effect)->dir, self->vel, sizeof self->vel);

    kind = 0;
    if (self->age < duration) {
        void *list = BtlGetBakuganList();

        if (list != NULL) {
            unit = *(BtlBakugan **)list;
            desc = &g_collisionSweptSphereDesc;
            __builtin_memcpy(&desc->start, self->pos, sizeof desc->start);
            desc->start.y = desc->start.y - 50.0f;
            __builtin_memcpy(&desc->dir, self->segment, sizeof desc->dir);
            desc->radius = width * 6.0f * 0.600000024f;
            desc->start.w = desc->radius * desc->radius;
            desc->dir.w = __builtin_sqrtf(desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y +
                                          desc->dir.z * desc->dir.z);
            for (; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
                entry = &((const VtblEntry *)unit->base.base.vtable)[11];
                if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) == 0) {
                    entry = &((const VtblEntry *)unit->base.base.vtable)[19];
                    if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) == 0) {
                        continue;
                    }
                }
                __builtin_memcpy(hit, self->pos, sizeof hit);
                if (CollisionTestShape(desc->shapeBlock, unit->collider0->shapeBlock, hit,
                                       0xffffffff) != 0) {
                    dx = self->pos[0] - hit[0];
                    dy = self->pos[1] - hit[1];
                    dz = self->pos[2] - hit[2];
                    dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
                    len = dist * 0.100000001f + 3.0f;
                    if (len < 10.0f) {
                        len = 10.0f;
                    } else if (!(len <= 280.0f)) {
                        len = 280.0f;
                    }
                    self->paramF0 = len;
                    /* segment = (vel.xyz * length * 10, 0 (S713)) */
                    scale = len * 10.0f;
                    self->segment[0] = self->vel[0] * scale;
                    self->segment[1] = self->vel[1] * scale;
                    self->segment[2] = self->vel[2] * scale;
                    self->segment[3] = 0.0f;
                }
                hit[1] = hit[1] + 30.0f;
            }
        }
    }

    if (self->age < duration &&
        BtlAttackSweepHitSound(width * 6.0f * 0.5f, self, self->pos, self->segment, hitKind, 0, 1,
                               0x31bf337e) != 0) {
        if (g_btlAttackHitCollider != NULL) {
            collider = (CollisionCollider *)g_btlAttackHitCollider;
            kind = (s32)((BtlBakugan *)collider->owner)->base.base.unk08;
        }
        if (g_btlAttackHitCollider != NULL && kind != 0 && kind < 0x84) {
            /* hit on a unit */
            mask = 0;
            if (NetPlayHasManager() && SaveGetProfileFlag0() != 0) {
                mask = 3;
            }
            collider = (CollisionCollider *)g_btlAttackHitCollider;
            if (collider->attachMatrix != NULL) {
                unit = (BtlBakugan *)((CollisionCollider *)g_btlAttackHitCollider)->owner;
                if ((self->param1 & mask) == 0) {
                    __builtin_memcpy(offset, self->pos, sizeof offset);
                    anchor = ((CollisionCollider *)g_btlAttackHitCollider)->attachMatrix[3];
                    entry = &((const VtblEntry *)unit->base.base.vtable)[11];
                    if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0 ||
                        (entry = &((const VtblEntry *)unit->base.base.vtable)[19],
                         ((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0)) {
                        /* closest = anchor with y of the last clip probe, moved to the closest
                           point of the beam.
                           UB (original binary): `hit` is unset if the clip loop probed no unit */
                        __builtin_memcpy(&closest, anchor, sizeof closest);
                        closest.y = hit[1];
                        __builtin_memcpy(g_collisionSegmentDesc.start, self->pos,
                                         sizeof g_collisionSegmentDesc.start);
                        __builtin_memcpy(g_collisionSegmentDesc.dir, self->segment,
                                         sizeof g_collisionSegmentDesc.dir);
                        CollisionSegmentClosestPoint(&g_collisionSegmentDesc, &closest, &t,
                                                     &closest);
                        offset[0] = offset[0] - closest.x;
                        offset[1] = offset[1] - closest.y;
                        offset[2] = offset[2] - closest.z;
                        /* offset.xyz = normalise(offset) * reach (0 for a zero vector), w = 0 */
                        reach = unit->combat.stats->reachRadius;
                        dist = offset[0] * offset[0] + offset[1] * offset[1] + offset[2] * offset[2];
                        inv = VfRsq(dist);
                        if (dist == 0.0f) {
                            inv = 0.0f;
                        }
                        scale = inv * reach;
                        offset[0] = offset[0] * scale;
                        offset[1] = offset[1] * scale;
                        offset[2] = offset[2] * scale;
                        offset[3] = 0.0f;
                        fx = GfxEffectSpawnDirected(g_btlAttackEffectMgr, unitHitFx, &closest.x,
                                                    self->vel);
                    } else {
                        offset[0] = offset[0] - anchor[0];
                        offset[1] = offset[1] - anchor[1];
                        offset[2] = offset[2] - anchor[2];
                        /* offset.xyz = normalise(offset) * reach (0 for a zero vector), w = 0 */
                        reach = unit->combat.stats->reachRadius;
                        dist = offset[0] * offset[0] + offset[1] * offset[1] + offset[2] * offset[2];
                        inv = VfRsq(dist);
                        if (dist == 0.0f) {
                            inv = 0.0f;
                        }
                        scale = inv * reach;
                        offset[0] = offset[0] * scale;
                        offset[1] = offset[1] * scale;
                        offset[2] = offset[2] * scale;
                        offset[3] = 0.0f;
                        fx = (GfxEffect *)GfxEffectSpawnAttachedDir(g_btlAttackEffectMgr,
                                                                    unitHitFx, anchor, self->vel);
                    }
                    __builtin_memcpy(fx->offset, offset, sizeof fx->offset);
                }
                if (kind < 0x21 && (unit->flags & 0x10) != 0) {
                    dx = self->pos[0] - g_btlAttackHitPoint.x;
                    dy = self->pos[1] - g_btlAttackHitPoint.y;
                    dz = self->pos[2] - g_btlAttackHitPoint.z;
                    dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
                    len = dist * 0.100000001f + 5.0f;
                    if (len < 10.0f) {
                        len = 10.0f;
                    } else if (!(len <= 280.0f)) {
                        len = 280.0f;
                    }
                    self->paramF0 = len;
                    /* segment = (segment.xyz * length * 10, 0 (S713)) */
                    scale = len * 10.0f;
                    self->segment[0] = self->segment[0] * scale;
                    self->segment[1] = self->segment[1] * scale;
                    self->segment[2] = self->segment[2] * scale;
                    self->segment[3] = 0.0f;
                }
            }
            self->param1 = self->param1 + 1;
        } else {
            /* world (or non-unit) hit */
            SaveGetProfileFlag0();
            if ((self->param0 & 1) == 0) {
                /* spot = (hit point.xyz + normal.xyz * 3, hit point.w) */
                spot[0] = g_btlAttackHitPoint.x + g_collisionHitResult.normal.x * 3.0f;
                spot[1] = g_btlAttackHitPoint.y + g_collisionHitResult.normal.y * 3.0f;
                spot[2] = g_btlAttackHitPoint.z + g_collisionHitResult.normal.z * 3.0f;
                spot[3] = g_btlAttackHitPoint.w;
                fx = GfxEffectSpawnDirected(g_btlAttackEffectMgr, worldHitFx, spot,
                                            &g_collisionHitResult.normal.x);
                entry = &((const VtblEntry *)fx->base.vtable)[2];
                ((void (*)(void *))entry->fn)((u8 *)fx + entry->delta);
                if (g_collisionHitResult.normal.x * g_collisionHitResult.normal.x +
                        g_collisionHitResult.normal.z * g_collisionHitResult.normal.z <
                    9.99999975e-06f) {
                    vertical = !(g_collisionHitResult.normal.y * g_collisionHitResult.normal.y <=
                                 9.99999975e-05f);
                } else {
                    vertical = 0;
                }
                m = fx->matrix;
                if (vertical) {
                    /* identity columns stored as the listing does: rows 1 and 2 swapped */
                    for (i = 0; i < 16; i++) {
                        m[i] = 0.0f;
                    }
                    m[0] = 1.0f;
                    m[6] = 1.0f;
                    m[9] = 1.0f;
                    m[15] = 1.0f;
                } else {
                    if (g_btlBeamDownAxisInit == 0) {
                        g_btlBeamDownAxisInit = 1;
                        g_btlBeamDownAxis.x = 0.0f;
                        g_btlBeamDownAxis.z = 0.0f;
                        g_btlBeamDownAxis.y = -1.0f;
                        g_btlBeamDownAxis.w = 0.0f;
                    }
                    /* a = down x n, b = n x a, c = a x b, e = b x c; rows normalise(e),
                       normalise(b), normalise(c) with w = 0, row 3 = (0, 0, 0, 1) */
                    n = &g_collisionHitResult.normal.x;
                    d = &g_btlBeamDownAxis.x;
                    ra[0] = d[1] * n[2] - d[2] * n[1];
                    ra[1] = d[2] * n[0] - d[0] * n[2];
                    ra[2] = d[0] * n[1] - d[1] * n[0];
                    rb[0] = n[1] * ra[2] - n[2] * ra[1];
                    rb[1] = n[2] * ra[0] - n[0] * ra[2];
                    rb[2] = n[0] * ra[1] - n[1] * ra[0];
                    rc[0] = ra[1] * rb[2] - ra[2] * rb[1];
                    rc[1] = ra[2] * rb[0] - ra[0] * rb[2];
                    rc[2] = ra[0] * rb[1] - ra[1] * rb[0];
                    re[0] = rb[1] * rc[2] - rb[2] * rc[1];
                    re[1] = rb[2] * rc[0] - rb[0] * rc[2];
                    re[2] = rb[0] * rc[1] - rb[1] * rc[0];
                    ia = VfRsq(re[0] * re[0] + re[1] * re[1] + re[2] * re[2]);
                    ib = VfRsq(rb[0] * rb[0] + rb[1] * rb[1] + rb[2] * rb[2]);
                    ic = VfRsq(rc[0] * rc[0] + rc[1] * rc[1] + rc[2] * rc[2]);
                    m[0] = re[0] * ia;
                    m[1] = re[1] * ia;
                    m[2] = re[2] * ia;
                    m[3] = 0.0f;
                    m[4] = rb[0] * ib;
                    m[5] = rb[1] * ib;
                    m[6] = rb[2] * ib;
                    m[7] = 0.0f;
                    m[8] = rc[0] * ic;
                    m[9] = rc[1] * ic;
                    m[10] = rc[2] * ic;
                    m[11] = 0.0f;
                    m[12] = 0.0f;
                    m[13] = 0.0f;
                    m[14] = 0.0f;
                    m[15] = 1.0f;
                }
            }
            self->param0 = self->param0 + 1;
        }

        if (g_btlAttackHitCollider != NULL) {
            kind = (s32)((BtlBakugan *)((CollisionCollider *)g_btlAttackHitCollider)->owner)
                       ->base.base.unk08;
        }
        if (width < 18.0f &&
            (g_btlAttackHitCollider == NULL || kind == 0 || kind >= 0x84)) {
            dx = self->pos[0] - g_btlAttackHitPoint.x;
            dy = self->pos[1] - g_btlAttackHitPoint.y;
            dz = self->pos[2] - g_btlAttackHitPoint.z;
            dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
            len = dist * 0.100000001f + 5.0f;
            if (len < 10.0f) {
                len = 10.0f;
            } else if (!(len <= 280.0f)) {
                len = 280.0f;
            }
            self->paramF0 = len;
            /* segment = (segment.xyz * length * 10, 0 (S713)) */
            scale = len * 10.0f;
            self->segment[0] = self->segment[0] * scale;
            self->segment[1] = self->segment[1] * scale;
            self->segment[2] = self->segment[2] * scale;
            self->segment[3] = 0.0f;
        }
    }

    ((GfxEffect *)self->effect)->size[1] = self->paramF0;
    /* auxVec = (pos.xyz + segment.xyz, pos.w) */
    self->auxVec[0] = self->pos[0] + self->segment[0];
    self->auxVec[1] = self->pos[1] + self->segment[1];
    self->auxVec[2] = self->pos[2] + self->segment[2];
    self->auxVec[3] = self->pos[3];
    BtlAttackCheckClash(self);
}
