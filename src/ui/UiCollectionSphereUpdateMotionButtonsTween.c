// bdc 0x0897e198 UiCollectionSphereUpdateMotionButtonsTween
#include "bdc.h"

/* Advances the motion-view button tweens of `UiCollectionSphere` (sprites
   0x3a..0x3d and 0x3e..0x41) started by `UiCollectionSphereStartMotionButtonsTween`; returns true
   when any of them reports finished (they run in lockstep, so in practice: all done). */

bool UiCollectionSphereUpdateMotionButtonsTween(UiCollectionSphere *self, u8 out)
{
  u8 done = 0;
  int i;

  for (i = 0x3a; i < 0x3e; i++)
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  for (i = 0x3e; i < 0x42; i++)
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  return done != 0;
}
