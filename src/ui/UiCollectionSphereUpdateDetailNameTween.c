// bdc 0x0897d08c UiCollectionSphereUpdateDetailNameTween
#include "bdc.h"

/* Advances by one frame the alpha tweens (scale 1.0, 16 frames, flags 1) of the detail name
   sprites 0x2d and 0x38 of `UiCollectionSphere` started by
   `UiCollectionSphereStartDetailNameTween`, fading in or out per `out`. Returns true when at
   least one of the tweens reported finished this frame (they are started together, so this is
   when the name is done). */

bool UiCollectionSphereUpdateDetailNameTween(UiCollectionSphere *self, u8 out)
{
  u8 finished;
  int i;

  finished = 0;
  for (i = 0x2d; i < 0x2e; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  }
  for (i = 0x38; i < 0x39; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  }
  return finished != 0;
}
