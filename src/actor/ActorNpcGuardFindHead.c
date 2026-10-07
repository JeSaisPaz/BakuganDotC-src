// bdc 0x088e8d6c ActorNpcGuardFindHead
#include "bdc.h"

/* Vtable slot 32 of the guard classes: stores the position of `Bip01_Head` in `+0x3f0` and the
   `Bone01` node in `+0x3e0`; returns whether `Bone01` exists. */

bool ActorNpcGuardFindHead(ActorNpc *self)

{
  void *node;
  ScePspFVector4 tmp __attribute__((aligned(16)));
  
  node = GfxModelFindNode((GfxModel *)self,"Bip01_Head");
  self->head = node;
  if (node != (void *)0x0) {
    GfxModelGetNodeRecordWorldPos((GfxModel *)self,&tmp,self->head);
    self->headPos[0] = tmp.x;
    self->headPos[1] = tmp.y;
    self->headPos[2] = tmp.z;
    self->headPos[3] = tmp.w;
  }
  node = GfxModelFindNode((GfxModel *)self,"Bone01");
  self->head = node;
  return node != (void *)0x0;
}

