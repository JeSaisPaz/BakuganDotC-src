// bdc 0x0886713c BtlBakuganUpdateStatusVisuals
#include "bdc.h"

/* Per-frame visual upkeep of a battle Bakugan (`BtlBakuganUpdate`). Counts `frameCounter`. Outside
   a battle demo (`BtlIsDemoRunning`): while stateFlags 0x20000 (head-on wall hit) is set it counts
   `wallHitFrames` (cleared otherwise) and, on counts 0 and 3 with model alpha `ambient[3]` not
   <= 0.1 and a horizontal speed (velocity x, z) above 17, spawns directed effect 0x1e at
   `wallHitPoint` along `wallHitNormal`; then `BtlBakuganSpawnWaterEffects`. Status 0x14 is applied
   for 15 frames (`BtlCombatApplyStatus`) while `inWater12To1B` is set and the unit's attribute
   (vtable slot 20) is non-zero, else cleared (`BtlCombatClearStatus`). `statusVisualBits` is
   rebuilt from the 21 combat status slots, and for every status that just ended its effects are
   stopped or switched to state 2 (`GfxEffectStopAttached`, `GfxEffectSetStateAttached` on the
   world effect manager, attached to `pos` or the pelvis `anchorMatrix[3]`; statuses 0/2/4/0x11 use
   `BtlBakuganStopBuffEffects`, 1/3/5 also end status 7, 0x13 also stops sound `0x2001f7` of a
   local player). The guard effect ages (`guardAge` +1, `guardEffect` counted down once below 0x10,
   `blockTimer` counted down) while `guardEffect` is set, and `guardAnchor` follows the spine
   (root matrix x spine node matrix, applied to `g_btlGuardAnchorOffsetBrontes` for Brontes 0xc,
   else `g_btlGuardAnchorOffset`). Kind-specific attachments, then return:
   - Nemus 8/0x14 in state 8 with stateFlags 0x80000: from motion frame 7 the effects `legs[0..1]`
     are shown (alpha 1); until frame 27 (or while `attackIndex` is 0x3f) both sit on the left
     finger and point at the weapon-bone points `g_btlNemusWeaponFar`/`g_btlNemusWeaponNear`
     (kind 0x14: `g_btlNemusEnWeaponFar`/`g_btlNemusEnWeaponNear`): `dir` = that offset scaled to
     length 30, `size[1]` = its length x 0.1. Otherwise (and in any other state) both get alpha 0.
   - Hades 0xe: the wing model runs its motion while stateFlags 0x1800000 is set.
   - Wilda 7: preloaded effect model 0 updates while stateFlags 0x1800000 is set.
   - Vulcan 0xd / Helios 0xa: `legs[0..1]` (Helios also `legs[2..3]`) follow the calf/hand bones:
     `dir` = the -X (`g_vecNegX`) or down (`g_vecDown`, Helios hands) axis by the bone's world
     matrix, `anchorPos` = the bone-local offset (w = 1) by that matrix (`g_btlVulcanCalfOffset`,
     `g_btlHeliosHandOffset`, `g_btlHeliosCalfOffset`); their `key` is 2 while stateFlags
     0x1800000 is set, else 1.
   - Altair 0xf: `statePos` = midpoint of the two tail bones; every 4th frame with `charge` not
     < 100 spawns effect 0x108 attached to it (`GfxEffectSpawnAttached`).
   The function-static offsets are filled on first use behind their `...Inited` guards.
   VFPU (lifted): a zero-length Nemus aim falls back to the bank constant S713 (0), so `dir` is then
   (0, 0, 0, 0); `dir.w` is always S713 = 0. */

/* d = column `col` of a * b (vmmul with a in M100, b in M200; matrices column-major) */
static void UsvMtxColumn(ScePspFVector4 *d, const float *a, const float *col)
{
    d->x = col[0] * a[0] + col[1] * a[4] + col[2] * a[8] + col[3] * a[12];
    d->y = col[0] * a[1] + col[1] * a[5] + col[2] * a[9] + col[3] * a[13];
    d->z = col[0] * a[2] + col[1] * a[6] + col[2] * a[10] + col[3] * a[14];
    d->w = col[0] * a[3] + col[1] * a[7] + col[2] * a[11] + col[3] * a[15];
}

/* d = a * b */
static void UsvMtxMul(ScePspFMatrix4 *d, const float *a, const float *b)
{
    UsvMtxColumn(&d->x, a, b);
    UsvMtxColumn(&d->y, a, b + 4);
    UsvMtxColumn(&d->z, a, b + 8);
    UsvMtxColumn(&d->w, a, b + 12);
}

/* d = m . (x, y, z, w) (vtfm4.q with E100) */
static void UsvTransform(float *d, const ScePspFMatrix4 *m, float x, float y, float z, float w)
{
    d[0] = m->x.x * x + m->y.x * y + m->z.x * z + m->w.x * w;
    d[1] = m->x.y * x + m->y.y * y + m->z.y * z + m->w.y * w;
    d[2] = m->x.z * x + m->y.z * y + m->z.z * z + m->w.z * w;
    d[3] = m->x.w * x + m->y.w * y + m->z.w * z + m->w.w * w;
}

/* Effect `fx` follows bone matrix `root * node`: dir = matrix . axis (all four lanes), anchorPos =
   matrix . (offset xyz, 1) */
static void UsvBoneEffect(GfxEffect *fx, const float *root, const ScePspFMatrix4 *node,
                          const ScePspFVector4 *axis, const ScePspFVector4 *offset)
{
    ScePspFMatrix4 m;

    UsvMtxMul(&m, root, &node->x.x);
    UsvTransform(fx->dir, &m, axis->x, axis->y, axis->z, axis->w);
    UsvTransform(fx->anchorPos, &m, offset->x, offset->y, offset->z, 1.0f);
}

/* Aims `fx` from `from` at `to`: size[1] = |to - from| (xyz) * 0.1, dir = (to - from) resized to
   length 30, w = S713 (0); a zero vector gives the S713 fallback 0 for the inverse length. */
static void UsvAim(GfxEffect *fx, const ScePspFVector4 *to, const ScePspFVector4 *from)
{
    float dx;
    float dy;
    float dz;
    float lenSq;
    float len;
    float inv;
    float k;

    dx = to->x - from->x;
    dy = to->y - from->y;
    dz = to->z - from->z;
    lenSq = dx * dx + dy * dy + dz * dz;
    len = __builtin_sqrtf(lenSq);
    len = len * 0.100000001f;
    if (lenSq == 0.0f) {
        inv = 0.0f;
    } else {
        inv = VfRsq(lenSq);
    }
    k = inv * 30.0f;
    fx->size[1] = len;
    fx->dir[0] = dx * k;
    fx->dir[1] = dy * k;
    fx->dir[2] = dz * k;
    fx->dir[3] = 0.0f;
}

void BtlBakuganUpdateStatusVisuals(BtlBakugan *self)
{
    ScePspFMatrix4 boneMtx;
    ScePspFVector4 pt;
    ScePspFVector4 finger;
    ScePspFVector4 farPos;
    ScePspFVector4 nearPos;
    const VtblEntry *vtbl;
    const BtlSwordBlur *blur;
    const GmoNode *bone;
    const ScePspFVector4 *farOffset;
    const ScePspFVector4 *nearOffset;
    const float *root;
    ScePspFMatrix4 *node;
    float speedSq;
    bool attrOn;
    u32 prev;
    u32 bits;
    u32 ended;
    s32 frame;
    s32 i;

    self->frameCounter = self->frameCounter + 1;
    if (!BtlIsDemoRunning()) {
        if ((self->stateFlags & 0x20000) == 0) {
            self->wallHitFrames = 0;
        } else {
            if (!(self->base.ambient[3] <= 0.100000001f) && self->wallHitFrames % 3 == 0 &&
                self->wallHitFrames < 4) {
                /* vdot.t of (velocity.x, 0, velocity.z) with itself (S001 zeroed) */
                speedSq = self->base.velocity[0] * self->base.velocity[0] +
                          self->base.velocity[2] * self->base.velocity[2];
                if (!(speedSq <= 289.0f)) {
                    GfxEffectSpawnDirected(g_worldEffectMgr, 0x1e, self->wallHitPoint,
                                           self->wallHitNormal);
                }
            }
            self->wallHitFrames = self->wallHitFrames + 1;
        }
        BtlBakuganSpawnWaterEffects(self);
    }

    attrOn = false;
    if (self->inWater12To1B != 0) {
        vtbl = (const VtblEntry *)self->base.base.vtable;
        if (((s32 (*)(void *))vtbl[20].fn)((u8 *)self + vtbl[20].delta) != 0) {
            attrOn = true;
        }
    }
    if (attrOn) {
        BtlCombatApplyStatus(&self->combat, 0x14, 0xf);
    } else {
        BtlCombatClearStatus(&self->combat, 0x14);
    }

    prev = self->statusVisualBits;
    bits = prev;
    for (i = 0; i < 0x15; i++) {
        if (self->combat.status[i].active != 0) {
            bits = bits | (1u << i);
        } else {
            bits = bits & ~(1u << i);
        }
    }
    self->statusVisualBits = bits;
    ended = (prev ^ bits) & ~bits;
    for (i = 0; i < 0x15; i++) {
        if ((ended & (1u << i)) == 0) {
            continue;
        }
        switch (i) {
        case 0:
        case 2:
        case 4:
        case 0x11:
            BtlBakuganStopBuffEffects(self);
            break;
        case 1:
            BtlBakuganEndStatus(self, 7);
            GfxEffectStopAttached(g_worldEffectMgr, 0x53, self->base.pos);
            GfxEffectSetStateAttached(g_worldEffectMgr, 0x299, self->base.pos, 2);
            break;
        case 3:
            BtlBakuganEndStatus(self, 7);
            GfxEffectStopAttached(g_worldEffectMgr, 0x55, self->base.pos);
            GfxEffectSetStateAttached(g_worldEffectMgr, 0x299, self->base.pos, 2);
            break;
        case 5:
            BtlBakuganEndStatus(self, 7);
            GfxEffectStopAttached(g_worldEffectMgr, 0x54, self->base.pos);
            GfxEffectSetStateAttached(g_worldEffectMgr, 0x299, self->base.pos, 2);
            break;
        case 6:
            GfxEffectSetStateAttached(g_worldEffectMgr, 0x1c5, self->anchorMatrix[3], 2);
            break;
        case 8:
        case 10:
            GfxEffectSetStateAttached(g_worldEffectMgr, 0x1c5, self->anchorMatrix[3], 2);
            break;
        case 9:
            GfxEffectSetStateAttached(g_worldEffectMgr, 0x49, self->anchorMatrix[3], 2);
            break;
        case 0x12:
            GfxEffectSetStateAttached(g_worldEffectMgr, 0x236, self->anchorMatrix[3], 2);
            GfxEffectSetStateAttached(g_worldEffectMgr, 0x237, self->anchorMatrix[3], 2);
            break;
        case 0x13:
            GfxEffectSetStateAttached(g_worldEffectMgr, 0x29b, self->base.pos, 2);
            if (BtlBakuganIsLocalPlayer(self)) {
                BtlBakuganStopSound(self, 0x2001f7);
            }
            break;
        default: /* 7, 0xb..0x10, 0x14 */
            break;
        }
    }

    if (self->guardEffect != 0) {
        self->guardAge = self->guardAge + 1;
        if (self->guardEffect < 0x10) {
            self->guardEffect = self->guardEffect - 1;
        }
        if (self->blockTimer != 0) {
            self->blockTimer = self->blockTimer - 1;
        }
    }

    if (g_btlGuardAnchorOffsetInited == 0) {
        g_btlGuardAnchorOffsetInited = 1;
        g_btlGuardAnchorOffset.x = 30.0f;
        g_btlGuardAnchorOffset.y = 20.0f;
        g_btlGuardAnchorOffset.z = 0.0f;
        g_btlGuardAnchorOffset.w = 0.0f;
    }
    if (g_btlGuardAnchorOffsetBrontesInited == 0) {
        g_btlGuardAnchorOffsetBrontesInited = 1;
        g_btlGuardAnchorOffsetBrontes.x = -15.0f;
        g_btlGuardAnchorOffsetBrontes.y = 20.0f;
        g_btlGuardAnchorOffsetBrontes.z = 0.0f;
        g_btlGuardAnchorOffsetBrontes.w = 0.0f;
    }
    /* boneMtx = root matrix x spine node matrix */
    bone = self->spineNode;
    UsvMtxMul(&boneMtx, self->base.data->rootMatrix, bone->localMatrix);
    if (self->base.base.unk08 == 0xc) {
        MathMtx4TransformPoint(&boneMtx, &pt, &g_btlGuardAnchorOffsetBrontes);
    } else {
        MathMtx4TransformPoint(&boneMtx, &pt, &g_btlGuardAnchorOffset);
    }
    self->guardAnchor[0] = pt.x;
    self->guardAnchor[1] = pt.y;
    self->guardAnchor[2] = pt.z;
    self->guardAnchor[3] = pt.w;

    if (self->base.base.unk08 == 8 || self->base.base.unk08 == 0x14) {
        if (self->state != 8 || (self->stateFlags & 0x80000) == 0) {
            self->legs[0]->color[3] = 0.0f;
            self->legs[1]->color[3] = 0.0f;
            return;
        }
        frame = GfxModelGetMotionFrameInt(&self->base);
        farOffset = &g_btlNemusWeaponFar;
        nearOffset = &g_btlNemusWeaponNear;
        if (g_btlNemusWeaponFarInited == 0) {
            g_btlNemusWeaponFarInited = 1;
            g_btlNemusWeaponFar.x = 110.0f;
            g_btlNemusWeaponFar.y = 0.0f;
            g_btlNemusWeaponFar.z = 0.0f;
            g_btlNemusWeaponFar.w = 0.0f;
        }
        if (g_btlNemusWeaponNearInited == 0) {
            g_btlNemusWeaponNearInited = 1;
            g_btlNemusWeaponNear.x = 70.0f;
            g_btlNemusWeaponNear.y = 0.0f;
            g_btlNemusWeaponNear.z = 0.0f;
            g_btlNemusWeaponNear.w = 0.0f;
        }
        if (g_btlNemusEnWeaponFarInited == 0) {
            g_btlNemusEnWeaponFarInited = 1;
            g_btlNemusEnWeaponFar.x = 100.0f;
            g_btlNemusEnWeaponFar.y = 0.0f;
            g_btlNemusEnWeaponFar.z = 0.0f;
            g_btlNemusEnWeaponFar.w = 0.0f;
        }
        if (g_btlNemusEnWeaponNearInited == 0) {
            g_btlNemusEnWeaponNearInited = 1;
            g_btlNemusEnWeaponNear.x = 60.0f;
            g_btlNemusEnWeaponNear.y = 0.0f;
            g_btlNemusEnWeaponNear.z = 0.0f;
            g_btlNemusEnWeaponNear.w = 0.0f;
        }
        if (self->base.base.unk08 == 0x14) {
            farOffset = &g_btlNemusEnWeaponFar;
            nearOffset = &g_btlNemusEnWeaponNear;
        }
        GfxModelGetNodeWorldPos(&self->base, &pt, "Bip01_L_Finger1");
        /* finger = pt; boneMtx = root matrix x weapon bone matrix */
        finger = pt;
        blur = self->weapon;
        bone = blur->bone;
        UsvMtxMul(&boneMtx, self->base.data->rootMatrix, bone->localMatrix);
        MathMtx4TransformPoint(&boneMtx, &pt, farOffset);
        farPos = pt;
        MathMtx4TransformPoint(&boneMtx, &pt, nearOffset);
        nearPos = pt;
        if (frame >= 7) {
            self->legs[0]->color[3] = 1.0f;
            self->legs[1]->color[3] = 1.0f;
        }
        if (frame >= 0x1b && self->attackIndex != 0x3f) {
            self->legs[0]->color[3] = 0.0f;
            self->legs[1]->color[3] = 0.0f;
            return;
        }
        /* both effects on the finger, aimed at the far / near weapon point */
        self->legs[0]->pos[0] = finger.x;
        self->legs[0]->pos[1] = finger.y;
        self->legs[0]->pos[2] = finger.z;
        self->legs[0]->pos[3] = finger.w;
        self->legs[1]->pos[0] = finger.x;
        self->legs[1]->pos[1] = finger.y;
        self->legs[1]->pos[2] = finger.z;
        self->legs[1]->pos[3] = finger.w;
        UsvAim(self->legs[0], &farPos, &finger);
        UsvAim(self->legs[1], &nearPos, &finger);
        return;
    }
    if (self->base.base.unk08 == 1) {
        return;
    }
    if (self->base.base.unk08 == 0xe) {
        if ((self->stateFlags & 0x1800000) != 0) {
            GfxModelUpdateMotion(self->wingModel);
            GfxModelApplyMotion(self->wingModel);
        }
        return;
    }
    if (self->base.base.unk08 == 7) {
        if ((self->stateFlags & 0x1800000) != 0) {
            GfxEffectModelUpdateOnce(0);
        }
        return;
    }
    if (self->base.base.unk08 == 0xd) {
        if (g_btlVulcanCalfOffsetInited == 0) {
            g_btlVulcanCalfOffsetInited = 1;
            g_btlVulcanCalfOffset.x = 44.0f;
            g_btlVulcanCalfOffset.y = 0.0f;
            g_btlVulcanCalfOffset.z = 0.0f;
            g_btlVulcanCalfOffset.w = 0.0f;
        }
        root = self->base.data->rootMatrix;
        node = GfxModelFindNodeMatrix(&self->base, "Bip01_L_Calf");
        UsvBoneEffect(self->legs[0], root, node, &g_vecNegX, &g_btlVulcanCalfOffset);
        root = self->base.data->rootMatrix;
        node = GfxModelFindNodeMatrix(&self->base, "Bip01_R_Calf");
        UsvBoneEffect(self->legs[1], root, node, &g_vecNegX, &g_btlVulcanCalfOffset);
        if ((self->stateFlags & 0x1800000) != 0) {
            self->legs[0]->key = 2;
            self->legs[1]->key = 2;
        } else {
            self->legs[0]->key = 1;
            self->legs[1]->key = 1;
        }
        return;
    }
    if (self->base.base.unk08 == 0xa) {
        if (g_btlHeliosHandOffsetInited == 0) {
            g_btlHeliosHandOffsetInited = 1;
            g_btlHeliosHandOffset.x = 15.0f;
            g_btlHeliosHandOffset.y = 17.0f;
            g_btlHeliosHandOffset.z = 0.0f;
            g_btlHeliosHandOffset.w = 0.0f;
        }
        root = self->base.data->rootMatrix;
        node = GfxModelFindNodeMatrix(&self->base, "Bip01_L_Hand");
        UsvBoneEffect(self->legs[0], root, node, &g_vecDown, &g_btlHeliosHandOffset);
        root = self->base.data->rootMatrix;
        node = GfxModelFindNodeMatrix(&self->base, "Bip01_R_Hand");
        UsvBoneEffect(self->legs[1], root, node, &g_vecDown, &g_btlHeliosHandOffset);
        if (g_btlHeliosCalfOffsetInited == 0) {
            g_btlHeliosCalfOffsetInited = 1;
            g_btlHeliosCalfOffset.x = 70.0f;
            g_btlHeliosCalfOffset.y = -4.0f;
            g_btlHeliosCalfOffset.z = 0.0f;
            g_btlHeliosCalfOffset.w = 0.0f;
        }
        root = self->base.data->rootMatrix;
        node = GfxModelFindNodeMatrix(&self->base, "Bip01_L_Calf");
        UsvBoneEffect(self->legs[2], root, node, &g_vecNegX, &g_btlHeliosCalfOffset);
        root = self->base.data->rootMatrix;
        node = GfxModelFindNodeMatrix(&self->base, "Bip01_R_Calf");
        UsvBoneEffect(self->legs[3], root, node, &g_vecNegX, &g_btlHeliosCalfOffset);
        if ((self->stateFlags & 0x1800000) != 0) {
            self->legs[0]->key = 2;
            self->legs[1]->key = 2;
            self->legs[2]->key = 2;
            self->legs[3]->key = 2;
        } else {
            self->legs[0]->key = 1;
            self->legs[1]->key = 1;
            self->legs[2]->key = 1;
            self->legs[3]->key = 1;
        }
        return;
    }
    if (self->base.base.unk08 != 0xf) {
        return;
    }
    GfxModelGetNodeWorldPos(&self->base, &pt, "Bip01_L_Tail");
    self->statePos[0] = pt.x;
    self->statePos[1] = pt.y;
    self->statePos[2] = pt.z;
    self->statePos[3] = pt.w;
    GfxModelGetNodeWorldPos(&self->base, &pt, "Bip01_R_Tail");
    /* statePos += (pt - statePos) * 0.5 (all four lanes) */
    self->statePos[0] = self->statePos[0] + (pt.x - self->statePos[0]) * 0.5f;
    self->statePos[1] = self->statePos[1] + (pt.y - self->statePos[1]) * 0.5f;
    self->statePos[2] = self->statePos[2] + (pt.z - self->statePos[2]) * 0.5f;
    self->statePos[3] = self->statePos[3] + (pt.w - self->statePos[3]) * 0.5f;
    if ((self->frameCounter & 3) == 0 && !(self->charge < 100.0f)) {
        GfxEffectSpawnAttached(g_worldEffectMgr, 0x108, self->statePos);
    }
}
