// bdc 0x088acab8 ActorStageObjAttachLight
#include "bdc.h"

/* Adds a light billboard sprite to the object's light list (`lights`, 16 slots allocated on first
   use from the low heap, count `lightCount`); for kind 0x1b (`F0_PIPEUNIT01`) its size
   (`width/height/depth`) is scaled by 0.6. The original's 4-lane store also writes the bank zero
   (S713) into `maybe_sizeW` (+0x7c). Called by `ActorStageObjCreateLights`. */

void ActorStageObjAttachLight(ActorStageObjBase *self, GfxSprite *sprite)
{
  bool fromLow;
  void *s;

  if (self->lights == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    s = MemAlloc(16 * sizeof(GfxSprite *), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->lights = s;
    memset(s, 0, 16 * sizeof(GfxSprite *));
    self->lightCount = 0;
  }
  if (self->lightCount < 0x10) {
    self->lights[self->lightCount++] = sprite;
    if (self->kind == 0x1b) {
      /* vscl.t by 0.6f; lane 3 of the sv.q is the bank constant S713 = 0.0f. */
      sprite->width = sprite->width * 0.6f;
      sprite->height = sprite->height * 0.6f;
      sprite->depth = sprite->depth * 0.6f;
      sprite->maybe_sizeW = 0.0f;
    }
  }
}
