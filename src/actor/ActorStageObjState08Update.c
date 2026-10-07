// bdc 0x088af318 ActorStageObjState08Update
#include "bdc.h"

/* Column-major 4x4 product d = a * b (the VFPU `vmmul.q` as displayed): column j of `d` is the sum
   over k of b[j][k] times column k of `a`. `d` must not alias `a` or `b`. */
static void ActorStageObjMtxMul(float *d, const float *a, const float *b)
{
    s32 j;
    s32 i;

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            d[j * 4 + i] = b[j * 4 + 0] * a[0 + i] + b[j * 4 + 1] * a[4 + i] + b[j * 4 + 2] * a[8 + i] +
                           b[j * 4 + 3] * a[12 + i];
        }
    }
}

/* Normalises the xyz of `v` (a zero vector gives scale 0), each lane clamped to [-1, 1], and sets w
   to 0 (the masked lane of the VFPU result register, bank S713). */
static void ActorStageObjNormalise(float *v)
{
    float d;
    float k;

    d = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    if (d == 0.0f) {
        k = 0.0f;
    } else {
        k = VfRsq(d);
    }
    v[0] = VfSat1(v[0] * k);
    v[1] = VfSat1(v[1] * k);
    v[2] = VfSat1(v[2] * k);
    v[3] = 0.0f;
}

/* State 8 handler of the shared stage-object state machine (state `+0x304`, `MemberFnPtr` table
   `0x08a842f8`, run by `ActorStageObjUpdate`): topple-over destruction, by `step`.
   Step 0 picks the topple axis `toppleAxis`: from the yaw of `hitBy` (`(cos, 0, sin, 0)`, also written
   over `hitBy`'s `+0x20` vector) or else from `pos - hitPos`; x is cleared for the kinds that topple
   as a whole (`ActorStageObjKindTopples`), y always; the axis is normalised (w 0) and turned a
   quarter around Y (`(z, y, -x)`). It clears the topple angle `val260`, `toppleTime`, `sinkY` and
   `toppleFlag`, copies the model matrix translation into `pos` and clears `matrixDirty`.
   Step 1 advances `toppleTime` by 1/60 and the angle by `(1 - cos(t^2 pi)) / 2 * pi/2` (next step
   once it reaches pi/2, clamped); the model matrix becomes the axis-angle rotation (with translation
   `(pos.x, sinkY, pos.z)`, `sinkY` growing every frame by `(1 - cos(t^2 pi)) / 2 * 25`, unclamped)
   times the yaw `base.rot[1]` rotation times a translation by `pos.y`; sets the collider's
   `attachDirty`.
   Step 2 plays sound `0x2000e6` at the model position (`SndEmitterCreateAtPos`) and, for
   billboards (`ActorStageObjIsBillboard`), spawns 8 dust effects 0xd (`GfxEffectSpawnDirected`
   on `g_btlUnitEffectMgr`) at random heights along the model's local Y axis (bounds `[5]`,
   `MathMtx4TransformPoint`) raised by 30, directed from the spawn point towards `pos` (also raised
   by 30 in y); starts a 60-frame timer.
   Step 3 counts the timer down. Step 4 blinks `fade` 0/1 on timer bit 1 while counting up to 32,
   then makes the materials translucent (`ActorStageObjMaterialSetTranslucent`) with `fade` 1.
   Step 5 fades out by 0.05 per frame. Step 6 sets `removeRequest`. */

void ActorStageObjState08Update(ActorStageObjBase *self)
{
    float stage[16];
    float quat[4];
    float qa[16];
    float qb[16];
    float rotMtx[16];
    float rotY[16];
    float mtx[16];
    float pos[4];
    float dir[4];
    ScePspFVector4 out;
    ScePspFVector4 pt;
    GfxModel *hitter;
    float *axis;
    float *m;
    float *bounds;
    float t;
    float c;
    float s;
    float x;
    float y;
    float z;
    float w;
    float angle;
    float half;
    float sink;
    float h;
    float rnd;
    float fade;
    s32 i;

    switch (self->step) {
    case 0:
        axis = self->toppleAxis;
        if (self->hitBy != NULL) {
            hitter = (GfxModel *)self->hitBy;
            /* vrot [C,0,S,0] of yaw * 2/pi (S703). */
            angle = hitter->rot[1];
            hitter->pos[0] = __builtin_cosf(angle);
            hitter->pos[1] = 0.0f;
            hitter->pos[2] = __builtin_sinf(angle);
            hitter->pos[3] = 0.0f;
            axis[0] = hitter->pos[0];
            axis[1] = hitter->pos[1];
            axis[2] = hitter->pos[2];
            axis[3] = hitter->pos[3];
        } else {
            /* xyz difference; w is pos.w. */
            axis[0] = self->base.pos[0] - self->hitPos[0];
            axis[1] = self->base.pos[1] - self->hitPos[1];
            axis[2] = self->base.pos[2] - self->hitPos[2];
            axis[3] = self->base.pos[3];
        }
        if (ActorStageObjKindTopples(self) != 0) {
            self->toppleAxis[0] = 0.0f;
        }
        self->toppleAxis[1] = 0.0f;
        ActorStageObjNormalise(axis);
        /* axis = (z, y, -x, w) */
        x = axis[0];
        y = axis[1];
        z = axis[2];
        axis[0] = z;
        axis[1] = y;
        axis[2] = -x;
        self->val260 = 0.0f;
        self->toppleTime = 0.0f;
        self->sinkY = 0.0f;
        self->toppleFlag = 0;
        self->base.pos[0] = self->base.data->rootMatrix[12];
        self->base.pos[1] = self->base.data->rootMatrix[13];
        self->base.pos[2] = self->base.data->rootMatrix[14];
        self->base.pos[3] = self->base.data->rootMatrix[15];
        self->matrixDirty = 0;
        self->step = self->step + 1;
        break;
    case 1:
        for (i = 0; i < 16; i++) {
            mtx[i] = (i % 5 == 0) ? 1.0f : 0.0f;
        }
        t = self->toppleTime + 0.016666668f;
        self->toppleTime = t;
        angle = self->val260;
        /* vcos of (t^2 pi) * 2/pi (S703) */
        c = __builtin_cosf(t * t * 3.1415927f);
        angle = angle + (1.0f - c) * 0.5f * 1.5707964f;
        self->val260 = angle;
        if (!(angle < 1.5707964f)) {
            self->val260 = 1.5707964f;
            self->step = self->step + 1;
        }
        angle = self->val260;
        /* quat = (axis * sin(angle / 2), cos(angle / 2)): angle / pi in quarter turns */
        half = 0.318309873f * angle;
        c = VfCosQuarter(half);
        s = VfSinQuarter(half);
        quat[0] = self->toppleAxis[0] * s;
        quat[1] = self->toppleAxis[1] * s;
        quat[2] = self->toppleAxis[2] * s;
        quat[3] = c;
        /* Quaternion -> rotation matrix as the product of its left and right multiplication
           matrices (columns built by vpfxs swizzles); the w row/column are reset to identity. */
        x = quat[0];
        y = quat[1];
        z = quat[2];
        w = quat[3];
        qa[0] = w;   qa[1] = z;   qa[2] = -y;  qa[3] = -x;
        qa[4] = -z;  qa[5] = w;   qa[6] = x;   qa[7] = -y;
        qa[8] = y;   qa[9] = -x;  qa[10] = w;  qa[11] = -z;
        qa[12] = x;  qa[13] = y;  qa[14] = z;  qa[15] = w;
        qb[0] = w;   qb[1] = z;   qb[2] = -y;  qb[3] = x;
        qb[4] = -z;  qb[5] = w;   qb[6] = x;   qb[7] = y;
        qb[8] = y;   qb[9] = -x;  qb[10] = w;  qb[11] = z;
        qb[12] = -x; qb[13] = -y; qb[14] = -z; qb[15] = w;
        ActorStageObjMtxMul(rotMtx, qa, qb);
        rotMtx[3] = 0.0f;
        rotMtx[7] = 0.0f;
        rotMtx[11] = 0.0f;
        rotMtx[12] = 0.0f;
        rotMtx[13] = 0.0f;
        rotMtx[14] = 0.0f;
        rotMtx[15] = 1.0f;
        rotMtx[12] = self->base.pos[0];
        rotMtx[14] = self->base.pos[2];
        sink = self->sinkY;
        t = self->toppleTime;
        c = __builtin_cosf(t * t * 3.1415927f);
        sink = sink + (1.0f - c) * 0.5f * 25.0f;
        self->sinkY = sink;
        rotMtx[13] = sink;
        mtx[13] = self->base.pos[1];
        /* mtx = rotY(base.rot[1]) * mtx (vrot of yaw * 2/pi, S703) */
        angle = self->base.rot[1];
        c = __builtin_cosf(angle);
        s = __builtin_sinf(angle);
        rotY[0] = c;    rotY[1] = 0.0f; rotY[2] = -s;   rotY[3] = 0.0f;
        rotY[4] = 0.0f; rotY[5] = 1.0f; rotY[6] = 0.0f; rotY[7] = 0.0f;
        rotY[8] = s;    rotY[9] = 0.0f; rotY[10] = c;   rotY[11] = 0.0f;
        rotY[12] = 0.0f; rotY[13] = 0.0f; rotY[14] = 0.0f; rotY[15] = 1.0f;
        ActorStageObjMtxMul(stage, rotY, mtx);
        for (i = 0; i < 16; i++) {
            mtx[i] = stage[i];
        }
        /* mtx = rotMtx * mtx (staged through stage) */
        ActorStageObjMtxMul(stage, rotMtx, mtx);
        for (i = 0; i < 16; i++) {
            mtx[i] = stage[i];
        }
        for (i = 0; i < 16; i++) {
            self->base.data->rootMatrix[i] = mtx[i];
        }
        if (self->collider != NULL) {
            ((CollisionCollider *)self->collider)->attachDirty = 1;
        }
        break;
    case 2:
        if (SndHasListener()) {
            SndEmitterCreateAtPos(SndGetListener(), 0x2000e6, &self->base.data->rootMatrix[12], 0, 1);
        }
        if (ActorStageObjIsBillboard(self) != 0) {
            for (i = 0; i < 8; i++) {
                m = self->base.data->rootMatrix;
                bounds = ActorStageObjGetBounds(self);
                h = bounds[5] * 2.0f;
                /* vrndf1 - 1 (S733): [0, 1) */
                rnd = PlatformRandFloat12() - 1.0f;
                h = h * rnd;
                bounds = ActorStageObjGetBounds(self);
                pt.x = 0.0f;
                pt.y = h - bounds[5];
                pt.z = 0.0f;
                pt.w = 0.0f;
                MathMtx4TransformPoint((ScePspFMatrix4 *)m, &out, &pt);
                pos[0] = out.x;
                pos[1] = out.y;
                pos[2] = out.z;
                pos[3] = out.w;
                /* xyz difference; w is the object's pos.w */
                dir[0] = self->base.pos[0] - pos[0];
                dir[1] = self->base.pos[1] - pos[1];
                dir[2] = self->base.pos[2] - pos[2];
                dir[3] = self->base.pos[3];
                pos[1] = pos[1] + 30.0f;
                dir[1] = dir[1] + 30.0f;
                ActorStageObjNormalise(dir);
                GfxEffectSpawnDirected(g_btlUnitEffectMgr, 0xd, pos, dir);
            }
        }
        self->timer = 60;
        self->step = self->step + 1;
        break;
    case 3:
        if (self->timer == 0) {
            self->step = self->step + 1;
        } else {
            self->timer = self->timer - 1;
        }
        break;
    case 4:
        if (self->timer == 32) {
            GfxModelForEachMaterial(&self->base, ActorStageObjMaterialSetTranslucent, NULL);
            self->fade = 1.0f;
            self->step = self->step + 1;
        } else {
            if ((self->timer & 2) != 0) {
                fade = 1.0f;
            } else {
                fade = 0.0f;
            }
            self->fade = fade;
            self->timer = self->timer + 1;
        }
        break;
    case 5:
        fade = self->fade - 0.05f;
        self->fade = fade;
        if (fade <= 0.0f) {
            self->fade = 0.0f;
            self->step = self->step + 1;
        }
        break;
    case 6:
        self->removeRequest = 1;
        self->step = self->step + 1;
        break;
    }
}
