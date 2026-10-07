// bdc 0x088aa35c ActorStageObjSaveRestPose
#include "bdc.h"

/* Saves the rest pose of a stage object: the root node's local translation (`localMatrix[12..15]`)
   to `restPos`, X/Z to `shakeX/shakeZ` (shake centre), `height` to `restHeight`, and resets `state` to 0.
   Called by `ActorStageObjBaseCtor`. */

void ActorStageObjSaveRestPose(ActorStageObjBase *self)
{
  GmoNode *node;
  float z;

  if (self->rootNode != NULL) {
    node = (GmoNode *)self->rootNode;
    self->restPos[0] = node->localMatrix[12];
    self->restPos[1] = node->localMatrix[13];
    self->restPos[2] = node->localMatrix[14];
    self->restPos[3] = node->localMatrix[15];
  }
  z = self->base.pos[2];
  self->shakeX = self->base.pos[0];
  self->shakeZ = z;
  self->restHeight = self->height;
  self->state = 0;
}
