// bdc 0x089055bc BtlDemoScenePlayerReset
#include "bdc.h"

/* Clears the playback fields of the battle demo scene player task (task id 0x66, 0x70 bytes, vtable
   `0x08af46a4`, `BtlDemoScenePlayerCtor`): frame/cursor words `+0x10`, `+0x18..+0x4c`,
   `+0x60..+0x68` and the flag bytes `+0x54..+0x57`, `+0x6c`, `+0x6d`. */

void BtlDemoScenePlayerReset(BtlDemoScenePlayer *self)
{
  self->unit = NULL;
  self->actor = NULL;
  self->follower = NULL;
  self->sphereNode = NULL;
  self->state = 0;
  self->frame = 0;
  self->demoId = 0;
  self->camStep = 0;
  self->eyeCursor = 0;
  self->lensCursor = 0;
  self->reserved40 = 0;
  self->reserved44 = 0;
  self->stagePool = 0;
  self->reserved54 = 0;
  self->suppressDemoEnd = 0;
  self->finished = 0;
  self->scene = NULL;
  self->poseA = 0;
  self->poseB = 0;
  self->motionEvents.tail = NULL;
  self->motionEvents.head = NULL;
  self->motionEvents.count = 0;
  self->specialId = 0;
  self->specialSoundPlayed = 0;
}
