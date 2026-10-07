// bdc 0x0885e724 BtlBakuganInitColliders
#include "bdc.h"

/* Constructor helper of `BtlBakuganCtor`: allocates (from the low end of the heap, under
   `MemLock`) the kind-1 body collider (400 bytes, `CollisionColliderCtor`) into `collider0`
   (NULL if the allocation fails) with the capsule `bodyShape`: start 0, axis of length
   `stats->colliderLength` along +Y for kinds 10, 0x18, 0x1b, along -Y for kind 0x19 and along +X
   otherwise, radius `stats->colliderRadius` (squared into `radiusSq`), `axisLen` = |axis| (VFPU),
   attached to `anchorMatrix` (`CollisionColliderInit`). Then the kind-2 push collider into
   `collider1` with the sphere `pushShape` (center = bank C720 = 0, radius `stats->reachRadius`,
   squared into `center.w`); its shape descriptor's center is set to the model root matrix row 3
   raised by its radius and refreshed through its vtable entry 9, and the collider gets flag bit 0
   and `hitTimer = 0`. */

void BtlBakuganInitColliders(BtlBakugan *self)
{
    bool fromLow;
    CollisionCollider *mem;
    CollisionCollider *collider;
    CollisionSphereQuery *sphere;
    const VtblEntry *update;
    float len;
    float r;
    const float *root;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = (CollisionCollider *)MemAlloc(sizeof(CollisionCollider), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    collider = NULL;
    if (mem != NULL) {
        CollisionColliderCtor(&mem->node, 1);
        collider = mem;
    }
    self->collider0 = collider;
    self->bodyShape.start[0] = 0.0f;
    self->bodyShape.start[1] = 0.0f;
    self->bodyShape.start[2] = 0.0f;
    self->bodyShape.radiusSq = 0.0f;
    switch (self->base.base.unk08) {
    case 10:
    case 0x18:
    case 0x1b:
        len = self->combat.stats->colliderLength;
        self->bodyShape.axis[0] = 0.0f;
        self->bodyShape.axis[1] = len;
        self->bodyShape.axis[2] = 0.0f;
        self->bodyShape.axisLen = 0.0f;
        break;
    case 0x19:
        len = -self->combat.stats->colliderLength;
        self->bodyShape.axis[0] = 0.0f;
        self->bodyShape.axis[1] = len;
        self->bodyShape.axis[2] = 0.0f;
        self->bodyShape.axisLen = 0.0f;
        break;
    default:
        self->bodyShape.axis[0] = self->combat.stats->colliderLength;
        self->bodyShape.axis[1] = 0.0f;
        self->bodyShape.axis[2] = 0.0f;
        self->bodyShape.axisLen = 0.0f;
        break;
    }
    r = self->combat.stats->colliderRadius;
    self->bodyShape.radius = r;
    self->bodyShape.radiusSq = r * r;
    /* vdot.t + vsqrt.s: |axis.xyz| */
    self->bodyShape.axisLen = __builtin_sqrtf(self->bodyShape.axis[0] * self->bodyShape.axis[0] +
                                              self->bodyShape.axis[1] * self->bodyShape.axis[1] +
                                              self->bodyShape.axis[2] * self->bodyShape.axis[2]);
    CollisionColliderInit(&self->collider0->node, (const u32 *)&self->bodyShape, 0, self, 0);
    self->collider0->byte104 = 0;
    collider = self->collider0;
    collider->attachMatrix = self->anchorMatrix;
    collider->attachDirty = 1;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = (CollisionCollider *)MemAlloc(sizeof(CollisionCollider), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    collider = NULL;
    if (mem != NULL) {
        CollisionColliderCtor(&mem->node, 2);
        collider = mem;
    }
    self->collider1 = collider;
    /* sv.q of bank C720 (0, 0, 0, 0) */
    self->pushShape.center.x = 0.0f;
    self->pushShape.center.y = 0.0f;
    self->pushShape.center.z = 0.0f;
    self->pushShape.center.w = 0.0f;
    r = self->combat.stats->reachRadius;
    self->pushShape.radius = r;
    self->pushShape.center.w = r * r;
    CollisionColliderInit(&self->collider1->node, (const u32 *)&self->pushShape, 0, self, 0);
    self->collider1->byte104 = 0;
    sphere = (CollisionSphereQuery *)self->collider1->shapeDesc;
    root = self->base.data->rootMatrix;
    /* sphere center = root matrix row 3 (lv.q/sv.q quad copy) */
    sphere->center.x = root[12];
    sphere->center.y = root[13];
    sphere->center.z = root[14];
    sphere->center.w = root[15];
    sphere->center.y = sphere->center.y + sphere->radius;
    update = &sphere->vtbl[9];
    ((void (*)(void *))update->fn)((u8 *)sphere + update->delta);
    collider = self->collider1;
    collider->flags |= 1;
    collider->hitTimer = 0;
}
