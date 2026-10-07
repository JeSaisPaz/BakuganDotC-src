// bdc 0x08880608 BtlAttackType07Update
#include "bdc.h"

/* Per-frame handler of attack types 7 and 0x71 (entry 7 of the handler table `0x08a685f0` run by
   `BtlAttackUpdate`): a beam from the owner's spinning bone to a target point, lasting 18
   frames. Frame 0 calls vtable entry 2 of the attached effect; from age 18 on it sets the effect's
   `key` to 2, drops it and ends the attack (`BtlAttackEnd`). Every other frame spins the bone
   (`localMatrix` x a rotation of 55 degrees about Z), sets `mtx` = owner `rootMatrix` x bone
   `localMatrix` and `pos` = `mtx` x `g_btlAttackType07Offset` (40, 0, 0; w = 1). The target
   point is the target unit's (`BtlFindBakuganById`) `pos` raised by 0.8 x its height, swept
   towards from `pos` (`BtlAttackSweepHit`, radius 10, kind 0x94, flags 3, mask `0x31bf337e`);
   without a target it sweeps along `vel` and takes `g_btlAttackHitPoint` on a hit, else the
   point 500 units out along the owner's yaw (`rot.y`). It spawns effect 0xeb at `pos` and, every
   third frame, 0xec at the target point. The effect's `dir` becomes the normalised beam direction
   (each lane clamped to [-1, 1], w = 0; a zero-length beam gives (0, 0, 0)), `vec1d0.x` = 0.1 x
   the beam length, and the six effect-chain points of its mesh are `pos` + dir x length x
   (0, 0.1, 0.2, 0.9, 1, 2) (w = `pos.w`). */
void BtlAttackType07Update(BtlAttack *self)
{
    float rot[16];
    float prod[16];
    float point[4];
    float toPoint[4];
    float offset[4];
    float scales[6];
    float *a;
    float *b;
    float angleCos;
    float angleSin;
    float yaw;
    float lenSq;
    float length;
    float inv;
    GfxEffect *effect;
    const VtblEntry *entry;
    BtlBakugan *target;
    ScePspFVector4 *points;
    int i;
    int j;

    if (self->age == 0) {
        effect = (GfxEffect *)self->effect;
        entry = &((const VtblEntry *)effect->base.vtable)[2];
        ((void (*)(void *))entry->fn)((u8 *)effect + entry->delta);
    } else if (!(self->age < 0x12)) {
        ((GfxEffect *)self->effect)->key = 2;
        self->effect = NULL;
        BtlAttackEnd(self);
        return;
    }
    if (g_btlAttackType07OffsetReady == 0) {
        g_btlAttackType07OffsetReady = 1;
        g_btlAttackType07Offset.y = 0.0f;
        g_btlAttackType07Offset.x = 40.0f;
        g_btlAttackType07Offset.z = 0.0f;
        g_btlAttackType07Offset.w = 0.0f;
    }

    /* rot = rotation by 55 degrees (0x3f75be0b rad, vrot of angle x S703) about Z, column-major */
    angleCos = __builtin_cosf(0.959931076f);
    angleSin = __builtin_sinf(0.959931076f);
    rot[0] = angleCos;
    rot[1] = angleSin;
    rot[2] = 0.0f;
    rot[3] = 0.0f;
    rot[4] = -angleSin;
    rot[5] = angleCos;
    rot[6] = 0.0f;
    rot[7] = 0.0f;
    rot[8] = 0.0f;
    rot[9] = 0.0f;
    rot[10] = 1.0f;
    rot[11] = 0.0f;
    rot[12] = 0.0f;
    rot[13] = 0.0f;
    rot[14] = 0.0f;
    rot[15] = 1.0f;

    /* bone localMatrix = localMatrix x rot (vmmul.q: column j = sum_k rot.col_j[k] x local.col_k) */
    a = self->bone->localMatrix;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            prod[j * 4 + i] = a[i] * rot[j * 4 + 0] + a[4 + i] * rot[j * 4 + 1] +
                              a[8 + i] * rot[j * 4 + 2] + a[12 + i] * rot[j * 4 + 3];
        }
    }
    for (i = 0; i < 16; i++) {
        a[i] = prod[i];
    }

    /* mtx = owner rootMatrix x bone localMatrix */
    a = self->owner->base.data->rootMatrix;
    b = self->bone->localMatrix;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            self->mtx[j][i] = a[i] * b[j * 4 + 0] + a[4 + i] * b[j * 4 + 1] +
                              a[8 + i] * b[j * 4 + 2] + a[12 + i] * b[j * 4 + 3];
        }
    }

    /* pos = mtx x (offset.xyz, 1) (vtfm4.q with E100; vfim.s S203 = 1.0f) */
    offset[0] = g_btlAttackType07Offset.x;
    offset[1] = g_btlAttackType07Offset.y;
    offset[2] = g_btlAttackType07Offset.z;
    offset[3] = 1.0f;
    for (i = 0; i < 4; i++) {
        self->pos[i] = self->mtx[0][i] * offset[0] + self->mtx[1][i] * offset[1] +
                       self->mtx[2][i] * offset[2] + self->mtx[3][i] * offset[3];
    }

    target = (BtlBakugan *)BtlFindBakuganById(self->targetId);
    if (target != NULL) {
        point[0] = target->base.pos[0];
        point[1] = target->base.pos[1];
        point[2] = target->base.pos[2];
        point[3] = target->base.pos[3];
        point[1] = point[1] + target->height * 0.800000012f;
        /* toPoint = point - pos (xyz; w = point.w) */
        toPoint[0] = point[0] - self->pos[0];
        toPoint[1] = point[1] - self->pos[1];
        toPoint[2] = point[2] - self->pos[2];
        toPoint[3] = point[3];
        BtlAttackSweepHit(10.0f, self, self->pos, toPoint, 0x94, 3, 0, 0x31bf337e);
    } else if (BtlAttackSweepHit(10.0f, self, self->pos, self->vel, 0x94, 3, 0, 0x31bf337e) != 0) {
        point[0] = g_btlAttackHitPoint.x;
        point[1] = g_btlAttackHitPoint.y;
        point[2] = g_btlAttackHitPoint.z;
        point[3] = g_btlAttackHitPoint.w;
    } else {
        /* point = pos + (cos, 0, sin)(yaw) x 500 (xyz; w = 0 from the vrot) */
        yaw = self->owner->base.rot[1];
        point[0] = __builtin_cosf(yaw) * 500.0f;
        point[1] = 0.0f;
        point[2] = __builtin_sinf(yaw) * 500.0f;
        point[3] = 0.0f;
        point[0] = point[0] + self->pos[0];
        point[1] = point[1] + self->pos[1];
        point[2] = point[2] + self->pos[2];
    }
    GfxEffectSpawn(g_btlAttackEffectMgr, 0xeb, self->pos);
    if (self->age % 3 == 0) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0xec, point);
    }

    /* point -= pos (xyz); length = |point|; point = clamped normalise(point), w = S713 (0) */
    point[0] = point[0] - self->pos[0];
    point[1] = point[1] - self->pos[1];
    point[2] = point[2] - self->pos[2];
    lenSq = point[0] * point[0] + point[1] * point[1] + point[2] * point[2];
    length = __builtin_sqrtf(lenSq);
    if (lenSq == 0.0f) {
        inv = 0.0f;
    } else {
        inv = VfRsq(lenSq);
    }
    point[0] = VfSat1(point[0] * inv);
    point[1] = VfSat1(point[1] * inv);
    point[2] = VfSat1(point[2] * inv);
    point[3] = 0.0f;
    effect = (GfxEffect *)self->effect;
    effect->dir[0] = point[0];
    effect->dir[1] = point[1];
    effect->dir[2] = point[2];
    effect->dir[3] = point[3];

    scales[1] = length * 0.100000001f;
    ((GfxEffect *)self->effect)->vec1d0[0] = scales[1];
    points = ((GfxEffect *)self->effect)->meshObj->effectChain.points;
    scales[0] = 0.0f;
    scales[2] = length * 0.200000003f;
    scales[3] = length * 0.899999976f;
    scales[4] = length;
    scales[5] = length * 2.0f;

    /* points[i] = pos + point x scales[i] (xyz; w = pos.w) */
    for (i = 0; i < 6; i++) {
        points[i].x = self->pos[0] + point[0] * scales[i];
        points[i].y = self->pos[1] + point[1] * scales[i];
        points[i].z = self->pos[2] + point[2] * scales[i];
        points[i].w = self->pos[3];
    }
}
