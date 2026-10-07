// bdc 0x0887715c BtlAttackLaunch
#include "bdc.h"

/* Launches an attack from its owner (called through `BtlAttackLaunchAuto`/`BtlAttackLaunchStage`
   right after `BtlAttackCtor`); `event` is the owner's attack `BtlMotionEvent` or NULL.
   1. `dir` (when given) is copied to `self->dir`.
   2. Matrix: identity without an event. With a bone (`event->param.ref.bone >= 0`) it is the
      model root matrix times that node's local matrix (`GfxModelGetNode`, node kept in
      `self->bone`), moved by `(launchDist, 0, 0)` through the matrix rotation when
      `launchDist != 0`; `dir` is set from the owner's heading. With a negative bone it is the
      identity placed at the owner's position plus `launchDist` along the heading, raised by
      `reach`.
   3. `self->pos` = `pos` or the matrix translation.
   4. Without `dir` the attack aims at the target unit (`BtlFindBakuganById(targetId)`): with
      the type's linked-target nibble set straight at its position, otherwise at its position
      +80 in y from (owner x, attack y, owner z), with the pitch clamped to +-0.61086524 rad
      (35 degrees) by a quaternion rotation about the horizontal axis (-z, 0, x).
   5. `paramF2` = `event->value`; `segment` = 0. When the type's id (low 10 bits of
      `g_btlAttackTypeInfo`) is 0 it only runs `BtlAttackUpdate`. Otherwise `radius` = the
      type's size byte, the effect id is bumped by one for 0xa5/0xd1/0xe8/0xed when
      `SaveGetProfileFlag0` is set, and the effect is spawned on `effectMgr` at `pos` along
      `dir` (`GfxEffectSpawnAttachedDir`) owned by the owner. Ids 0x80/0x81 scale effect size
      and radius by the owner's height (x0.7 for kinds 4/0x11/0x14), take the owner's
      attribute (virtual `vtbl[20]`) as texture slot and its `g_btlAttributeEffectHue`, and
      end the owner's other sustained attacks (`BtlAttackEndOwnedSustained` with `type`
      cleared meanwhile). Then `vel` = `dir`, `BtlAttackUpdate` runs, and when an effect
      exists and the type's low status nibble is set while the owner's status 6 is active,
      type 0x3a doubles radius (effect size 15), 0x3b quadruples it, others rise 60 in y with
      radius x2.5 (size 1.5); the effect's virtual `vtbl[2]` (update) is then called.
   6. Plays the launch sound `params.launchSound` (`BtlAttackPlaySound`) at the owner's
      position while the owner is still listed (`BtlBakuganListFind`), else at `self->pos`.
   VFPU bank constants as literals: C720/S713/S730 = 0, S703 = 2/pi (radians to quarter turns). */

void BtlAttackLaunch(BtlAttack *self, float *pos, float *dir, s16 *event, void *effectMgr)
{
    const BtlMotionEvent *ev = (const BtlMotionEvent *)event;
    float offset[4];
    float moved[3];
    float step[3];
    float aimTarget[4];
    float aimFrom[4];
    float axis[4];
    float quat[4];
    float conj[4];
    float tmp[4];
    float res[4];
    float *root;
    float *local;
    GmoNode *node;
    BtlBakugan *target;
    BtlBakugan *owner;
    GfxEffect *effect;
    const VtblEntry *vtbl;
    u32 info;
    s32 id;
    s32 savedType;
    s32 attr;
    s32 i;
    s32 j;
    float heading;
    float lenSq;
    float k;
    float pitch;
    float delta;
    float turns;
    float c;
    float s;
    float scale;

    if (dir != NULL) {
        self->dir[0] = dir[0];
        self->dir[1] = dir[1];
        self->dir[2] = dir[2];
        self->dir[3] = dir[3];
    }
    if (ev == NULL) {
        for (j = 0; j < 4; j++) {
            for (i = 0; i < 4; i++) {
                self->mtx[j][i] = (i == j) ? 1.0f : 0.0f;
            }
        }
    } else if (ev->param.ref.bone >= 0) {
        node = GfxModelGetNode(&self->owner->base, ev->param.ref.bone);
        self->bone = node;
        /* mtx = model root matrix x node local matrix (vmmul.q, column-major) */
        root = self->owner->base.data->rootMatrix;
        local = node->localMatrix;
        for (j = 0; j < 4; j++) {
            for (i = 0; i < 4; i++) {
                self->mtx[j][i] = root[0 * 4 + i] * local[j * 4 + 0]
                                  + root[1 * 4 + i] * local[j * 4 + 1]
                                  + root[2 * 4 + i] * local[j * 4 + 2]
                                  + root[3 * 4 + i] * local[j * 4 + 3];
            }
        }
        if (ev->launchDist != 0.0f) {
            /* offset = (launchDist, 0, 0, 0); mtx[3].xyz += mtx rotation x offset (vtfm3.t) */
            offset[0] = ev->launchDist;
            offset[1] = 0.0f;
            offset[2] = 0.0f;
            offset[3] = 0.0f;
            for (i = 0; i < 3; i++) {
                moved[i] = self->mtx[0][i] * offset[0] + self->mtx[1][i] * offset[1]
                           + self->mtx[2][i] * offset[2];
            }
            self->mtx[3][0] = self->mtx[3][0] + moved[0];
            self->mtx[3][1] = self->mtx[3][1] + moved[1];
            self->mtx[3][2] = self->mtx[3][2] + moved[2];
        }
        /* dir = (cos(heading), 0, sin(heading), 0) (vrot.q of heading * 2/pi) */
        heading = self->owner->base.rot[1];
        self->dir[0] = __builtin_cosf(heading);
        self->dir[1] = 0.0f;
        self->dir[2] = __builtin_sinf(heading);
        self->dir[3] = 0.0f;
    } else {
        for (j = 0; j < 4; j++) {
            for (i = 0; i < 4; i++) {
                self->mtx[j][i] = (i == j) ? 1.0f : 0.0f;
            }
        }
        owner = self->owner;
        self->mtx[3][0] = owner->base.pos[0];
        self->mtx[3][1] = owner->base.pos[1];
        self->mtx[3][2] = owner->base.pos[2];
        self->mtx[3][3] = owner->base.pos[3];
        /* dir = (cos(heading), 0, sin(heading), 0) */
        heading = self->owner->base.rot[1];
        self->dir[0] = __builtin_cosf(heading);
        self->dir[1] = 0.0f;
        self->dir[2] = __builtin_sinf(heading);
        self->dir[3] = 0.0f;
        /* step = dir * launchDist; mtx[3].xyz += step */
        step[0] = self->dir[0] * ev->launchDist;
        step[1] = self->dir[1] * ev->launchDist;
        step[2] = self->dir[2] * ev->launchDist;
        self->mtx[3][0] = self->mtx[3][0] + step[0];
        self->mtx[3][1] = self->mtx[3][1] + step[1];
        self->mtx[3][2] = self->mtx[3][2] + step[2];
        self->mtx[3][1] = self->mtx[3][1] + ev->reach;
    }

    if (pos != NULL) {
        self->pos[0] = pos[0];
        self->pos[1] = pos[1];
        self->pos[2] = pos[2];
        self->pos[3] = pos[3];
    } else {
        self->pos[0] = self->mtx[3][0];
        self->pos[1] = self->mtx[3][1];
        self->pos[2] = self->mtx[3][2];
        self->pos[3] = self->mtx[3][3];
    }

    if (dir == NULL) {
        target = (BtlBakugan *)BtlFindBakuganById(self->targetId);
        if (((g_btlAttackTypeInfo[self->type] >> 20) & 0xf) != 0) {
            /* dir = target pos (w kept) minus owner pos (xyz), then normalized and clamped */
            self->dir[0] = target->base.pos[0];
            self->dir[1] = target->base.pos[1];
            self->dir[2] = target->base.pos[2];
            self->dir[3] = target->base.pos[3];
            owner = self->owner;
            self->dir[0] = self->dir[0] - owner->base.pos[0];
            self->dir[1] = self->dir[1] - owner->base.pos[1];
            self->dir[2] = self->dir[2] - owner->base.pos[2];
            lenSq = self->dir[0] * self->dir[0] + self->dir[1] * self->dir[1]
                    + self->dir[2] * self->dir[2];
            k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
            self->dir[0] = VfSat1(self->dir[0] * k);
            self->dir[1] = VfSat1(self->dir[1] * k);
            self->dir[2] = VfSat1(self->dir[2] * k);
        } else {
            owner = self->owner;
            aimFrom[0] = owner->base.pos[0];
            aimFrom[1] = self->pos[1];
            aimFrom[2] = owner->base.pos[2];
            aimFrom[3] = 0.0f;
            aimTarget[0] = target->base.pos[0];
            aimTarget[1] = target->base.pos[1];
            aimTarget[2] = target->base.pos[2];
            aimTarget[3] = target->base.pos[3];
            aimTarget[1] = aimTarget[1] + 80.0f;
            /* dir = normalize(aimTarget - aimFrom) (xyz; w = aimTarget w) */
            self->dir[0] = aimTarget[0] - aimFrom[0];
            self->dir[1] = aimTarget[1] - aimFrom[1];
            self->dir[2] = aimTarget[2] - aimFrom[2];
            self->dir[3] = aimTarget[3];
            lenSq = self->dir[0] * self->dir[0] + self->dir[1] * self->dir[1]
                    + self->dir[2] * self->dir[2];
            k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
            self->dir[0] = VfSat1(self->dir[0] * k);
            self->dir[1] = VfSat1(self->dir[1] * k);
            self->dir[2] = VfSat1(self->dir[2] * k);
            pitch = atan2f(self->dir[1],
                           __builtin_sqrtf(self->dir[0] * self->dir[0]
                                           + self->dir[2] * self->dir[2]));
            if (!(__builtin_fabsf(pitch) <= 0.610865235f)) {
                if (pitch <= 0.0f) {
                    delta = -0.610865235f - pitch;
                } else {
                    delta = 0.610865235f - pitch;
                }
                /* axis = normalize(-dir.z, 0, dir.x), w = dir.w */
                axis[0] = -self->dir[2];
                axis[1] = 0.0f;
                axis[2] = self->dir[0];
                axis[3] = self->dir[3];
                lenSq = axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2];
                k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
                axis[0] = VfSat1(axis[0] * k);
                axis[1] = VfSat1(axis[1] * k);
                axis[2] = VfSat1(axis[2] * k);
                /* quat = (axis * sin, cos) of delta * 1/pi quarter turns (half the angle) */
                turns = 0.318309873f * delta;
                c = VfCosQuarter(turns);
                s = VfSinQuarter(turns);
                quat[0] = axis[0] * s;
                quat[1] = axis[1] * s;
                quat[2] = axis[2] * s;
                quat[3] = c;
                conj[0] = -quat[0];
                conj[1] = -quat[1];
                conj[2] = -quat[2];
                conj[3] = quat[3];
                /* dir = quat * (dir.xyz, 0) * conj(quat) (vqmul.q twice) */
                res[0] = self->dir[0];
                res[1] = self->dir[1];
                res[2] = self->dir[2];
                res[3] = 0.0f;
                tmp[0] = quat[0] * res[3] + quat[1] * res[2] - quat[2] * res[1] + quat[3] * res[0];
                tmp[1] = -quat[0] * res[2] + quat[1] * res[3] + quat[2] * res[0] + quat[3] * res[1];
                tmp[2] = quat[0] * res[1] - quat[1] * res[0] + quat[2] * res[3] + quat[3] * res[2];
                tmp[3] = -quat[0] * res[0] - quat[1] * res[1] - quat[2] * res[2] + quat[3] * res[3];
                self->dir[0] = tmp[0] * conj[3] + tmp[1] * conj[2] - tmp[2] * conj[1]
                               + tmp[3] * conj[0];
                self->dir[1] = -tmp[0] * conj[2] + tmp[1] * conj[3] + tmp[2] * conj[0]
                               + tmp[3] * conj[1];
                self->dir[2] = tmp[0] * conj[1] - tmp[1] * conj[0] + tmp[2] * conj[3]
                               + tmp[3] * conj[2];
                self->dir[3] = -tmp[0] * conj[0] - tmp[1] * conj[1] - tmp[2] * conj[2]
                               + tmp[3] * conj[3];
            }
        }
    }

    if (ev != NULL) {
        self->paramF2 = ev->value;
    }
    id = g_btlAttackTypeInfo[self->type] & 0x3ff;
    self->segment[0] = 0.0f;
    self->segment[1] = 0.0f;
    self->segment[2] = 0.0f;
    self->segment[3] = 0.0f;
    if (id == 0) {
        BtlAttackUpdate(self);
    } else {
        self->radius = (float)(u8)(g_btlAttackTypeInfo[self->type] >> 24);
        if (SaveGetProfileFlag0() != 0
            && (id == 0xed || id == 0xe8 || id == 0xd1 || id == 0xa5)) {
            id = id + 1;
        }
        effect = (GfxEffect *)GfxEffectSpawnAttachedDir(effectMgr, id, self->pos, self->dir);
        owner = self->owner;
        self->effect = effect;
        effect->ownerBakugan = owner;
        if (owner != NULL) {
            effect->ownerId = owner->base.base.id;
        }
        if (id == 0x80 || id == 0x81) {
            owner = self->owner;
            scale = owner->height * 0.00666667009f;
            if (owner->base.base.unk08 == 0x14 || owner->base.base.unk08 == 0x11
                || owner->base.base.unk08 == 4) {
                scale = scale * 0.699999988f;
            }
            ((GfxEffect *)self->effect)->vec1d0[0] = scale * 0.800000012f;
            self->radius = scale * 100.0f;
            owner = self->owner;
            effect = (GfxEffect *)self->effect;
            vtbl = (const VtblEntry *)owner->base.base.vtable;
            effect->textureSlot = ((s32 (*)(void *))vtbl[20].fn)((u8 *)owner + vtbl[20].delta);
            owner = self->owner;
            effect = (GfxEffect *)self->effect;
            vtbl = (const VtblEntry *)owner->base.base.vtable;
            attr = ((s32 (*)(void *))vtbl[20].fn)((u8 *)owner + vtbl[20].delta);
            effect->vec1d0[1] = g_btlAttributeEffectHue[attr];
            savedType = self->type;
            self->type = 0;
            BtlAttackEndOwnedSustained(self->owner);
            self->type = savedType;
        }
        self->vel[0] = self->dir[0];
        self->vel[1] = self->dir[1];
        self->vel[2] = self->dir[2];
        self->vel[3] = self->dir[3];
        BtlAttackUpdate(self);
        if (self->effect != NULL) {
            info = g_btlAttackTypeInfo[self->type];
            if (((info >> 16) & 0xf) != 0 && self->owner->combat.status[6].active != 0) {
                if (self->type == 0x3a) {
                    self->radius = self->radius * 2.0f;
                    ((GfxEffect *)self->effect)->vec1d0[0] = 15.0f;
                } else if (self->type == 0x3b) {
                    self->radius = self->radius * 4.0f;
                } else {
                    self->pos[1] = self->pos[1] + 60.0f;
                    self->radius = self->radius * 2.5f;
                    ((GfxEffect *)self->effect)->vec1d0[0] = 1.5f;
                }
            }
            effect = (GfxEffect *)self->effect;
            vtbl = (const VtblEntry *)effect->base.vtable;
            ((void (*)(void *))vtbl[2].fn)((u8 *)effect + vtbl[2].delta);
        }
    }

    if (BtlBakuganListFind(self->owner) != NULL) {
        BtlAttackPlaySound(self, self->params.launchSound, self->owner->base.pos, 0, 0);
    } else {
        BtlAttackPlaySound(self, self->params.launchSound, self->pos, 0, 0);
    }
}
