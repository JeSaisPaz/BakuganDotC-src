// bdc 0x0887f478 BtlAttackType38Update
#include "bdc.h"

/* Per-frame handler of attack type 0x38 (entry 56 of the handler table `0x08a685f0` run by
   `BtlAttackUpdate`): the owner's rocket. Once its age passes 180 it shows the owner's `_rocket`
   sub-model again, queues the rocket model for deletion and ends. Frame 0: spawns the launch effect
   0xe4 at `pos` into `effect2`, calls vtable entry 2 of it, records the owner Bakugan and its id on
   it, builds the rocket model `afx_139m_L.gmo` (0x140-byte `GfxModelCtor` object allocated from
   low memory) with the owner's stencil ref, ambient (0.2, 0.2, 0.2, 1) and lighting on, attaches
   it to the effect (`model`), hides the owner's `_rocket`, sets `dir` to `g_vecX` transformed by
   `mtx` (its first row), resets turn rate and speed (`mtx[0][0]`) to 0, sets the wobble phase
   `paramF0` = (1 - paramF2) * 0.5 * pi and plays sound 0x1500029. Later frames: after age 20 the
   turn rate grows by 0.01 while below 0.2; the speed grows by 0.5 while below 35; a wobble offset
   (cos, 0, sin)(paramF0) * ((0.2 - turnRate) * 1500 + 100) is built, the phase advances by
   0.04 and the rocket homes (`BtlAttackSteerToTarget`) at height
   (0.2 - turnRate) * 500 + 80 - age * 0.5. After `BtlAttackCheckClash`, a sweep hit (kind 0x5b)
   spawns impact 0xe6 at `g_btlAttackHitPoint`, shows `_rocket`, deletes the model, plays sound
   0x200098 and ends. Otherwise: the attached effect takes `dir`, `pos += vel` (xyz), `effect2`
   takes `pos`, its matrix is the basis along `dir` (with `g_vecUp`; a zero-length axis scales
   to 0) times a rotation about Z by paramF2 radians. */
void BtlAttackType38Update(BtlAttack *self)
{
    float offset[4];
    float rot[16];
    float out[16];
    float *m;
    float spread;
    float angle;
    float len;
    float k;
    float ax[3];
    float ay[3];
    float az[4];
    const ScePspFVector4 *up;
    GfxEffect *effect;
    GfxEffect *rocket;
    GfxModel *obj;
    GfxModel *model;
    BtlBakugan *owner;
    const VtblEntry *entry;
    bool fromLow;
    int i;
    int j;

    if (self->age > 180) {
        GfxModelSetMaterialVisibleByName(&self->owner->base, "_rocket", true);
        CoreObjectDeferDelete(&((GfxEffect *)self->effect2)->model->base, 0);
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->turnRate = 0.0199999996f;
        rocket = (GfxEffect *)GfxEffectSpawn(g_btlAttackEffectMgr, 0xe4, self->pos);
        self->effect2 = rocket;
        entry = &((const VtblEntry *)rocket->base.vtable)[2];
        ((void (*)(void *))entry->fn)((u8 *)rocket + entry->delta);
        rocket = (GfxEffect *)self->effect2;
        owner = self->owner;
        rocket->ownerBakugan = owner;
        if (owner != NULL) {
            rocket->ownerId = owner->base.base.id;
        }
        model = NULL;
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        obj = (GfxModel *)MemAlloc(sizeof(GfxModel), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (obj != NULL) {
            GfxModelCtor(obj, "afx_139m_L.gmo", 0);
            model = obj;
        }
        GfxModelSetStencilRef(model, (u8)self->owner->stencilRef);
        model->ambient[0] = 0.200000003f;
        model->ambient[1] = 0.200000003f;
        model->ambient[2] = 0.200000003f;
        model->ambient[3] = 1.0f;
        model->lighting = 1;
        ((GfxEffect *)self->effect2)->model = model;
        GfxModelSetMaterialVisibleByName(&self->owner->base, "_rocket", false);
        /* dir = sum over k of g_vecX[k] * mtx[k] (vtfm4.q, E form) */
        for (i = 0; i < 4; i++) {
            self->dir[i] = g_vecX.x * self->mtx[0][i] + g_vecX.y * self->mtx[1][i]
                         + g_vecX.z * self->mtx[2][i] + g_vecX.w * self->mtx[3][i];
        }
        self->turnRate = 0.0f;
        self->mtx[0][0] = 0.0f; /* homing speed */
        self->paramF0 = (1.0f - self->paramF2) * 0.5f * 3.14159274f;
        BtlAttackPlaySound(self, 0x1500029, NULL, 0, 0);
        return;
    }

    if (self->age > 20 && self->turnRate < 0.200000003f) {
        self->turnRate = self->turnRate + 0.00999999978f;
    }
    if (self->mtx[0][0] < 35.0f) {
        self->mtx[0][0] = self->mtx[0][0] + 0.5f;
    }
    spread = (0.200000003f - self->turnRate) * 1500.0f + 100.0f;
    angle = self->paramF0;
    /* offset = (cos, 0, sin)(paramF0) * spread, w = 0 */
    offset[0] = __builtin_cosf(angle) * spread;
    offset[1] = 0.0f * spread;
    offset[2] = __builtin_sinf(angle) * spread;
    offset[3] = 0.0f;
    self->paramF0 = self->paramF0 + 0.0399999991f;
    BtlAttackSteerToTarget(self->mtx[0][0],
                           ((0.200000003f - self->turnRate) * 500.0f + 80.0f) - (float)self->age * 0.5f,
                           self, 0, offset);
    BtlAttackCheckClash(self);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x5b, 3, 0, 0x31bf337e) != 0) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0xe6, &g_btlAttackHitPoint.x);
        GfxModelSetMaterialVisibleByName(&self->owner->base, "_rocket", true);
        CoreObjectDeferDelete(&((GfxEffect *)self->effect2)->model->base, 0);
        BtlAttackPlaySound(self, 0x200098, NULL, 0, 0);
        BtlAttackEnd(self);
        return;
    }

    effect = (GfxEffect *)self->effect;
    effect->dir[0] = self->dir[0];
    effect->dir[1] = self->dir[1];
    effect->dir[2] = self->dir[2];
    effect->dir[3] = self->dir[3];
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
    rocket = (GfxEffect *)self->effect2;
    rocket->pos[0] = self->pos[0];
    rocket->pos[1] = self->pos[1];
    rocket->pos[2] = self->pos[2];
    rocket->pos[3] = self->pos[3];

    /* rocket matrix = basis along dir with g_vecUp: z = dir, x = up x z, y = z x x (both
       normalised and clamped to [-1, 1]); w lanes 0, last row/column identity */
    rocket = (GfxEffect *)self->effect2;
    up = &g_vecUp;
    len = self->dir[0] * self->dir[0] + self->dir[1] * self->dir[1] + self->dir[2] * self->dir[2];
    k = (len == 0.0f) ? 0.0f : VfRsq(len);
    az[0] = VfSat1(self->dir[0] * k);
    az[1] = VfSat1(self->dir[1] * k);
    az[2] = VfSat1(self->dir[2] * k);
    ax[0] = up->y * az[2] - up->z * az[1];
    ax[1] = up->z * az[0] - up->x * az[2];
    ax[2] = up->x * az[1] - up->y * az[0];
    len = ax[0] * ax[0] + ax[1] * ax[1] + ax[2] * ax[2];
    k = (len == 0.0f) ? 0.0f : VfRsq(len);
    ax[0] = VfSat1(ax[0] * k);
    ax[1] = VfSat1(ax[1] * k);
    ax[2] = VfSat1(ax[2] * k);
    ay[0] = az[1] * ax[2] - az[2] * ax[1];
    ay[1] = az[2] * ax[0] - az[0] * ax[2];
    ay[2] = az[0] * ax[1] - az[1] * ax[0];
    m = rocket->matrix;
    m[0] = ax[0];
    m[1] = ax[1];
    m[2] = ax[2];
    m[3] = 0.0f;
    m[4] = ay[0];
    m[5] = ay[1];
    m[6] = ay[2];
    m[7] = 0.0f;
    m[8] = az[0];
    m[9] = az[1];
    m[10] = az[2];
    m[11] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;

    /* rot = rotation about Z by paramF2 radians; rocket matrix = matrix * rot (column-major:
       column j = sum over k of rot[j][k] * column k of matrix) */
    rocket = (GfxEffect *)self->effect2;
    angle = self->paramF2;
    rot[0] = __builtin_cosf(angle);
    rot[1] = __builtin_sinf(angle);
    rot[2] = 0.0f;
    rot[3] = 0.0f;
    rot[4] = -__builtin_sinf(angle);
    rot[5] = __builtin_cosf(angle);
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
    m = rocket->matrix;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            out[j * 4 + i] = rot[j * 4 + 0] * m[0 * 4 + i] + rot[j * 4 + 1] * m[1 * 4 + i]
                           + rot[j * 4 + 2] * m[2 * 4 + i] + rot[j * 4 + 3] * m[3 * 4 + i];
        }
    }
    for (i = 0; i < 16; i++) {
        m[i] = out[i];
    }
}
