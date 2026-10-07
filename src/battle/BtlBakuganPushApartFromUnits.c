// bdc 0x08861e58 BtlBakuganPushApartFromUnits
#include "bdc.h"

/* Pushes the unit away from the other units it overlaps and moves it with collision. Does
   nothing without a push collider (`collider1`) or when that collider is flagged 0x40 (busy). The
   push starts from zero and walks `g_btlBakuganList`; every other unit
   with a push collider not flagged 4 contributes. Its radius is its stat reach radius and its
   height this unit's `height`, unless it is big: virtual slot 11 non-zero gives radius 240 x
   scale.x clamped to 0.6..1 and height 730 x scale.x; else virtual slot 19 non-zero gives 180 and
   700. Units more than 1.2 x height apart vertically are skipped; the vertical overlap fraction
   (at most 1) weights the push. With horizontal distance d below the combined reach (own reach +
   radius) and above sqrt(0.001), the push is along the horizontal offset with length
   0.7 x (reach - d) x overlap (0.7 x reach x overlap when reach - d is not <= reach, i.e. NaN).
   For (almost) coincident units it pushes along the reversed heading, unless a ray of twice the
   reach from the push collider's sphere centre (`CollisionRaycastRayB` with `collisionMask`)
   hits something, then along the heading, by 5 x overlap. A big unit stops the walk after its
   push. Without a big last unit, the total horizontal push is clamped to the own reach radius
   and state flag 0x10 is set. The push is applied with `BtlBakuganMoveCollide``(self, push,
   flags & 0xff)` while the collider is flagged busy (cleared again unless it was set before).
   `delta` is not used. */

void BtlBakuganPushApartFromUnits(BtlBakugan *self, float *delta, u32 flags)
{
    float push[4] BDC_ALIGN16;
    float rel[4] BDC_ALIGN16;
    float origin[4] BDC_ALIGN16;
    float move[4] BDC_ALIGN16;
    CollisionCollider *collider;
    const VtblEntry *entry;
    BtlBakugan *unit;
    float selfReach;
    float radius;
    float height;
    float reach;
    float scale;
    float overlap;
    float dy;
    float lenSq;
    float dist;
    float amount;
    float yaw;
    float k;
    bool found;
    bool big;
    u32 wasBusy;

    (void)delta;
    collider = self->collider1;
    if (collider == NULL || (collider->flags & 0x40) != 0) {
        return;
    }
    found = false;
    big = false;
    /* push = C720, the constant bank's zero vector */
    push[0] = 0.0f;
    push[1] = 0.0f;
    push[2] = 0.0f;
    push[3] = 0.0f;
    selfReach = self->combat.stats->reachRadius;

    for (unit = *(BtlBakugan **)g_btlBakuganList; unit != NULL;
         unit = (BtlBakugan *)unit->base.base.next) {
        if (unit == self || unit->collider1 == NULL || (unit->collider1->flags & 4) != 0) {
            continue;
        }
        radius = unit->combat.stats->reachRadius;
        height = self->height;
        big = false;
        entry = &((const VtblEntry *)unit->base.base.vtable)[11];
        if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
            scale = unit->base.scale[0];
            if (scale < 0.600000024f) {
                scale = 0.600000024f;
            } else if (!(scale <= 1.0f)) {
                scale = 1.0f;
            }
            radius = scale * 240.0f;
            height = unit->base.scale[0] * 730.0f;
            big = true;
        } else {
            entry = &((const VtblEntry *)unit->base.base.vtable)[19];
            if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
                radius = 180.0f;
                height = 700.0f;
                big = true;
            }
        }
        reach = selfReach + radius;
        /* rel = self pos - unit pos (xyz; w keeps self pos w) */
        rel[0] = self->base.pos[0] - unit->base.pos[0];
        rel[1] = self->base.pos[1] - unit->base.pos[1];
        rel[2] = self->base.pos[2] - unit->base.pos[2];
        rel[3] = self->base.pos[3];
        dy = ABS(rel[1]) - height * 1.20000005f;
        if (!(dy < 0.0f)) {
            continue;
        }
        overlap = -dy / height;
        if (!(overlap <= 1.0f)) {
            overlap = 1.0f;
        }
        rel[1] = 0.0f;
        lenSq = rel[0] * rel[0] + rel[1] * rel[1] + rel[2] * rel[2];
        if (lenSq < reach * reach && !(lenSq <= 0.00100000005f)) {
            dist = reach - __builtin_sqrtf(lenSq);
            if (dist <= reach) {
                amount = dist * 0.699999988f * overlap;
            } else {
                amount = reach * 0.699999988f * overlap;
            }
            /* rel.xyz = normalise(rel.xyz) x amount (zero length: 0); w = S713 (0) */
            lenSq = rel[0] * rel[0] + rel[1] * rel[1] + rel[2] * rel[2];
            k = (lenSq == 0.0f ? 0.0f : VfRsq(lenSq)) * amount;
            rel[0] = rel[0] * k;
            rel[1] = rel[1] * k;
            rel[2] = rel[2] * k;
            rel[3] = 0.0f;
            push[0] = push[0] + rel[0];
            push[1] = push[1] + rel[1];
            push[2] = push[2] + rel[2];
        } else if (lenSq <= 0.00100000005f) {
            /* origin = push collider's sphere centre (collider1 re-read after the calls) */
            origin[0] = ((CollisionSphereQuery *)self->collider1->shapeDesc)->center.x;
            origin[1] = ((CollisionSphereQuery *)self->collider1->shapeDesc)->center.y;
            origin[2] = ((CollisionSphereQuery *)self->collider1->shapeDesc)->center.z;
            origin[3] = ((CollisionSphereQuery *)self->collider1->shapeDesc)->center.w;
            yaw = self->base.rot[1] + 3.14159274f;
            if (!(yaw <= 3.14159274f)) {
                yaw = yaw - 6.28318548f;
            } else if (yaw <= -3.14159274f) {
                yaw = yaw + 6.28318548f;
            }
            /* rel = (cos yaw, 0, sin yaw, 0) x 2 reach */
            k = reach * 2.0f;
            rel[0] = __builtin_cosf(yaw) * k;
            rel[1] = 0.0f;
            rel[2] = __builtin_sinf(yaw) * k;
            rel[3] = 0.0f;
            if (CollisionRaycastRayB(self->collisionMask, origin, rel) != NULL) {
                yaw = self->base.rot[1];
            }
            /* rel = (cos yaw, 0, sin yaw, 0) x 5 overlap */
            k = overlap * 5.0f;
            rel[0] = __builtin_cosf(yaw) * k;
            rel[1] = 0.0f;
            rel[2] = __builtin_sinf(yaw) * k;
            rel[3] = 0.0f;
            push[0] = push[0] + rel[0];
            push[1] = push[1] + rel[1];
            push[2] = push[2] + rel[2];
        } else {
            continue;
        }
        found = true;
        if (big) {
            break;
        }
    }

    if (!found) {
        return;
    }
    lenSq = push[0] * push[0] + push[1] * push[1] + push[2] * push[2];
    if (!big) {
        selfReach = self->combat.stats->reachRadius;
        if (!(lenSq <= selfReach * selfReach)) {
            /* push.xyz = normalise(push.xyz) x reach (zero length: 0); w = S713 (0) */
            k = (lenSq == 0.0f ? 0.0f : VfRsq(lenSq)) * selfReach;
            push[0] = push[0] * k;
            push[1] = push[1] * k;
            push[2] = push[2] * k;
            push[3] = 0.0f;
        }
        self->stateFlags |= 0x10;
    }
    collider = self->collider1;
    wasBusy = collider->flags & 0x40;
    collider->flags |= 0x40;
    move[0] = push[0];
    move[1] = push[1];
    move[2] = push[2];
    move[3] = push[3];
    BtlBakuganMoveCollide(self, move, flags & 0xff);
    if (wasBusy == 0) {
        self->collider1->flags &= ~0x40u;
    }
}
