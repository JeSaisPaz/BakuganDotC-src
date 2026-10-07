// bdc 0x088b5478 ActorStageObjDebrisUpdate
#include "bdc.h"

/* Update method (vtable `0x08af2c34` slot 7) of the break-debris model
   (`ActorStageObjDebrisCtor`). Unless `frame` is 0 or 2, steps every active fragment (bit in
   `activeMask`) of `fragments` (`base.partCount` of them) through its virtual slot 2; a fragment
   whose step result is <= 0.5 for 61 frames in a row (`settleFrames`, reset otherwise) is retired
   from `activeMask`. A fragment whose `stepCounter` (< 16) differs from `fragLast[i]` records it
   there; on the first such change after a stable frame (`contactFlag[i]` 0) with the counter a
   multiple of 5 it plays `impactSoundId` (if nonzero) at the fragment's `centre`, gated by
   `ActorStageObjTryLockSound(1)` and `SndHasListener` (`SndEmitterCreateAtPos`, no loop,
   auto-free). `contactFlag[i]` is set while the counter keeps changing and cleared when it is
   unchanged or >= 16. When no active fragment was stepped or `frame` exceeds `lifetime`, `fade`
   drops by 1/30 and is copied into the alpha `base.ambient[3]`; at `fade <= 0` the model is
   deleted (`CoreObjectDeferDelete`, delay 0) and `frame` is left alone. Otherwise `frame`
   is incremented. */

void ActorStageObjDebrisUpdate(ActorStageObjDebris *self)
{
  CollisionPhysBox *frag;
  const VtblEntry *e;
  SndListener *listener;
  u32 bit;
  s32 counter;
  float r;
  int stepped;
  int i;

  stepped = 0;
  if (self->frame != 0 && self->frame != 2) {
    for (i = 0; i < self->base.partCount; i++) {
      bit = 1u << i;
      if ((self->activeMask & bit) == 0) {
        continue;
      }
      frag = &self->fragments[i];
      e = &frag->vtbl[2];
      r = ((float (*)(void *))e->fn)((char *)frag + e->delta);
      if (r <= 0.5f) {
        self->settleFrames[i] = (u8)(self->settleFrames[i] + 1);
      } else {
        self->settleFrames[i] = 0;
      }
      if (!(self->settleFrames[i] < 61)) {
        self->activeMask = self->activeMask & ~bit;
      }
      counter = self->fragments[i].stepCounter;
      stepped = 1;
      if (counter < 16 && self->fragLast[i] != counter) {
        if (self->contactFlag[i] == 0) {
          if (counter % 5 == 0 && self->impactSoundId != 0 && ActorStageObjTryLockSound(1) != 0 &&
              SndHasListener()) {
            listener = SndGetListener();
            SndEmitterCreateAtPos(listener, self->impactSoundId,
                                  (float *)&self->fragments[i].centre, 0, 1);
          }
          self->contactFlag[i] = (u8)(self->contactFlag[i] + 1);
        }
        self->fragLast[i] = counter;
      } else {
        self->contactFlag[i] = 0;
      }
    }
  }
  if (!stepped || self->lifetime < self->frame) {
    self->fade = self->fade - 0.0333333351f;
    if (self->fade <= 0.0f) {
      CoreObjectDeferDelete(&self->base.base, 0);
      return;
    }
    self->base.ambient[3] = self->fade;
  }
  self->frame = self->frame + 1;
}
