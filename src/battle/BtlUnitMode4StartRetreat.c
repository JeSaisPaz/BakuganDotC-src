// bdc 0x0885db74 BtlUnitMode4StartRetreat
#include "bdc.h"

/* Starts the scripted retreat of the mode-4 unit (`BtlUnitMode4UpdateRetreat`): sets the
   `retreating` byte, retreat step 1 and the retreat point (0, 0, -1185.0, 0), then makes the unit
   untouchable like `BtlBakuganSetInert`: the body collider gets flag bit 0 with a permanent hit
   timer (-1, skipped by hit queries) and the push collider flag 0x40 (no push-apart). */
void BtlUnitMode4StartRetreat(BtlUnitMode4 *self)
{
    CollisionCollider *body;
    CollisionCollider *push;

    self->retreating = 1;
    self->retreatStep = 1;
    self->retreatPoint[0] = 0.0f;
    self->retreatPoint[1] = 0.0f;
    self->retreatPoint[2] = -1185.0f; /* 0xc4942000 */
    self->retreatPoint[3] = 0.0f;

    body = (CollisionCollider *)self->base.collider0;
    if (body != NULL) {
        body->flags |= 1;
        body->hitTimer = -1;
    }
    push = (CollisionCollider *)self->base.collider1;
    if (push != NULL) {
        push->flags |= 0x40;
    }
}
