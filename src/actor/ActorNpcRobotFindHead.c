// bdc 0x088e8b70 ActorNpcRobotFindHead
#include "bdc.h"

/* Vtable slot 32 of the robot classes: finds the `Head` node (`GfxModelFindNode`) into `+0x3e0`
   and stores its position in `+0x3f0`. Returns whether it exists. */

bool ActorNpcRobotFindHead(ActorNpc *self)

{
  void *node;
  float pos[4] __attribute__((aligned(16)));

  node = GfxModelFindNode((GfxModel *)self, "Head");
  self->head = node;
  if (node != (void *)0x0) {
    GfxModelGetNodeRecordWorldPos((GfxModel *)self, (ScePspFVector4 *)pos, self->head);
    self->headPos[0] = pos[0];
    self->headPos[1] = pos[1];
    self->headPos[2] = pos[2];
    self->headPos[3] = pos[3];
  }
  return self->head != (void *)0x0;
}
