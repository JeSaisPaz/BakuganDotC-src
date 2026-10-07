// bdc 0x08878760 BtlAttackCheckClash
#include "bdc.h"

/* Attack-versus-attack clash test. Builds this attack's collision shape: with a zero `segment`
   the sphere `g_btlClashSelfSphere` (centre `pos`, radius × 1.3), otherwise the capsule
   `g_btlClashSelfCapsule` (start `pos`, axis `segment`, radius × 1.5). Then walks
   `g_btlAttackList` and, for every other attack of a different owner that has not ended
   (`endFrame` 0) and is not `cancelled`, looks up the outcome in `g_btlAttackClashMatrix`
   `[categoryThis][categoryOther]` (category from `g_btlAttackTypeInfo`). A non-zero outcome
   builds the other attack's sphere `g_btlClashOtherSphere` or capsule `g_btlClashOtherCapsule`
   (radius × 1.5) and tests it against this attack's shape (`CollisionSphereOverlapSphere`,
   `CollisionSphereOverlapCapsule`, `CollisionCapsuleOverlapSphere`,
   `CollisionCapsuleOverlapCapsule`). On a hit `g_btlClashCount` is incremented and outcome 1
   cancels the other attack, -1 this one, 2 both. Independently of the outcome, an attack of type
   0x5b hitting a non-barrier attack (category 0xc) either ends it silently with `reflector` =
   this owner (other `params.reflectMode` 1) or makes it reflect towards this attack's `pos`
   (`reflect`, `auxVec`, `noSpark`); type 0xc only sets `reflect` and `auxVec`. Returns 1 when
   this attack was cancelled, else 0. */

int BtlAttackCheckClash(BtlAttack *self)
{
    BtlAttack *other;
    BtlAttack *next;
    int selfCategory;
    int otherCategory;
    s8 outcome;
    bool selfIsSphere;
    bool hit;
    u8 selfCancelled;
    float radius;

    if (g_btlClashSelfSphereInit == 0) {
        g_btlClashSelfSphereInit = 1;
        g_btlClashSelfSphere.vtbl = g_collisionSphereVtbl;
        g_btlClashSelfSphere.type = 3;
    }
    if (g_btlClashSelfCapsuleInit == 0) {
        g_btlClashSelfCapsuleInit = 1;
        g_btlClashSelfCapsule.vtbl = g_collisionCapsuleVtbl;
        ((SegmentShape *)g_btlClashSelfCapsule.segmentHead)->info = (void *)g_collisionSegmentVtbl;
        ((SegmentShape *)g_btlClashSelfCapsule.segmentHead)->type = 2;
        g_btlClashSelfCapsule.type = 4;
    }
    if (g_btlClashOtherSphereInit == 0) {
        g_btlClashOtherSphereInit = 1;
        g_btlClashOtherSphere.vtbl = g_collisionSphereVtbl;
        g_btlClashOtherSphere.type = 3;
    }
    if (g_btlClashOtherCapsuleInit == 0) {
        g_btlClashOtherCapsuleInit = 1;
        g_btlClashOtherCapsule.vtbl = g_collisionCapsuleVtbl;
        ((SegmentShape *)g_btlClashOtherCapsule.segmentHead)->info = (void *)g_collisionSegmentVtbl;
        ((SegmentShape *)g_btlClashOtherCapsule.segmentHead)->type = 2;
        g_btlClashOtherCapsule.type = 4;
    }
    selfCategory = ((int)(s16)g_btlAttackTypeInfo[self->type] & 0xfc00U) >> 10;
    g_btlClashCount = 0;
    /* the asm ORs the raw bits of x, y, z and masks the sign: true exactly for ±0 in all three */
    selfIsSphere = self->segment[0] == 0.0f && self->segment[1] == 0.0f && self->segment[2] == 0.0f;
    if (selfIsSphere) {
        /* the asm copies 4 lanes; the w lane lands in radiusSq and is overwritten below */
        g_btlClashSelfSphere.center[0] = self->pos[0];
        g_btlClashSelfSphere.center[1] = self->pos[1];
        g_btlClashSelfSphere.center[2] = self->pos[2];
        radius = self->radius * 1.3f;
        g_btlClashSelfSphere.radius = radius;
        g_btlClashSelfSphere.radiusSq = radius * radius;
    } else {
        /* the asm copies 4 lanes; the w lanes land in radiusSq and axisLen and are overwritten below */
        g_btlClashSelfCapsule.start[0] = self->pos[0];
        g_btlClashSelfCapsule.start[1] = self->pos[1];
        g_btlClashSelfCapsule.start[2] = self->pos[2];
        g_btlClashSelfCapsule.axis[0] = self->segment[0];
        g_btlClashSelfCapsule.axis[1] = self->segment[1];
        g_btlClashSelfCapsule.axis[2] = self->segment[2];
        radius = self->radius * 1.5f;
        g_btlClashSelfCapsule.radius = radius;
        g_btlClashSelfCapsule.radiusSq = radius * radius;
        g_btlClashSelfCapsule.axisLen = __builtin_sqrtf(
            g_btlClashSelfCapsule.axis[0] * g_btlClashSelfCapsule.axis[0] +
            g_btlClashSelfCapsule.axis[1] * g_btlClashSelfCapsule.axis[1] +
            g_btlClashSelfCapsule.axis[2] * g_btlClashSelfCapsule.axis[2]);
    }
    selfCancelled = 0;
    for (other = (BtlAttack *)g_btlAttackList; other != NULL; other = next) {
        next = (BtlAttack *)other->base.next;
        if (other == self || other->owner == self->owner || other->endFrame != 0 ||
            other->cancelled != 0) {
            continue;
        }
        otherCategory = ((int)(s16)g_btlAttackTypeInfo[other->type] & 0xfc00U) >> 10;
        outcome = g_btlAttackClashMatrix[selfCategory][otherCategory];
        if (outcome == 0) {
            continue;
        }
        if (other->segment[0] == 0.0f && other->segment[1] == 0.0f && other->segment[2] == 0.0f) {
            /* the asm copies 4 lanes; the w lane lands in radiusSq and is overwritten below */
            g_btlClashOtherSphere.center[0] = other->pos[0];
            g_btlClashOtherSphere.center[1] = other->pos[1];
            g_btlClashOtherSphere.center[2] = other->pos[2];
            radius = other->radius * 1.5f;
            g_btlClashOtherSphere.radius = radius;
            g_btlClashOtherSphere.radiusSq = radius * radius;
            if (selfIsSphere) {
                hit = CollisionSphereOverlapSphere(&g_btlClashOtherSphere, &g_btlClashSelfSphere);
            } else {
                hit = CollisionSphereOverlapCapsule(&g_btlClashOtherSphere, &g_btlClashSelfCapsule);
            }
        } else {
            /* the asm copies 4 lanes; the w lanes land in radiusSq and axisLen and are overwritten below */
            g_btlClashOtherCapsule.start[0] = other->pos[0];
            g_btlClashOtherCapsule.start[1] = other->pos[1];
            g_btlClashOtherCapsule.start[2] = other->pos[2];
            g_btlClashOtherCapsule.axis[0] = other->segment[0];
            g_btlClashOtherCapsule.axis[1] = other->segment[1];
            g_btlClashOtherCapsule.axis[2] = other->segment[2];
            radius = other->radius * 1.5f;
            g_btlClashOtherCapsule.radius = radius;
            g_btlClashOtherCapsule.radiusSq = radius * radius;
            g_btlClashOtherCapsule.axisLen = __builtin_sqrtf(
                g_btlClashOtherCapsule.axis[0] * g_btlClashOtherCapsule.axis[0] +
                g_btlClashOtherCapsule.axis[1] * g_btlClashOtherCapsule.axis[1] +
                g_btlClashOtherCapsule.axis[2] * g_btlClashOtherCapsule.axis[2]);
            if (selfIsSphere) {
                hit = CollisionCapsuleOverlapSphere(&g_btlClashOtherCapsule, &g_btlClashSelfSphere);
            } else {
                hit = CollisionCapsuleOverlapCapsule(&g_btlClashOtherCapsule, &g_btlClashSelfCapsule);
            }
        }
        if (!hit) {
            continue;
        }
        g_btlClashCount++;
        if (outcome == 1) {
            other->cancelled = 1;
        } else if (outcome == -1) {
            self->cancelled = 1;
            selfCancelled = 1;
        } else if (outcome == 2) {
            other->cancelled = 1;
            self->cancelled = 1;
            selfCancelled = 1;
        }
        if (self->type == 0x5b) {
            if ((((int)(s16)g_btlAttackTypeInfo[other->type] & 0xfc00U) >> 10) == 0xc) {
                continue;
            }
            if (other->params.reflectMode == 1) {
                other->silentEnd = 1;
                other->reflector = self->owner;
            } else {
                other->reflect = 1;
                other->auxVec[0] = self->pos[0];
                other->auxVec[1] = self->pos[1];
                other->auxVec[2] = self->pos[2];
                other->auxVec[3] = self->pos[3];
                other->noSpark = 1;
            }
        } else if (self->type == 0xc) {
            if ((((int)(s16)g_btlAttackTypeInfo[other->type] & 0xfc00U) >> 10) == 0xc) {
                continue;
            }
            other->reflect = 1;
            other->auxVec[0] = self->pos[0];
            other->auxVec[1] = self->pos[1];
            other->auxVec[2] = self->pos[2];
            other->auxVec[3] = self->pos[3];
        }
    }
    return selfCancelled;
}
