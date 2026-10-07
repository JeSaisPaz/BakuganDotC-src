// bdc 0x088e8880 ActorNpcRobotSlot48
#include "bdc.h"

/* Vtable slot 48 of the robot class (`0x08af3e94`): clears bit 4 of the collider flags
   (`+0x130`) of the collider `+0x174`. */

void ActorNpcRobotSlot48(ActorNpc *self)
{
  CollisionCollider *collider = (CollisionCollider *)self->base.collider2;

  collider->flags = collider->flags & 0xffffffef;
}
