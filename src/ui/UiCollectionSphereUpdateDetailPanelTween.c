// bdc 0x0897cc50 UiCollectionSphereUpdateDetailPanelTween
#include "bdc.h"

/* Advances by one frame the alpha tweens (scale 1.0, 16 frames, flags 1) of the detail panel
   sprites 0x2b-0x2c and 0x2e-0x2f of `UiCollectionSphere` started by
   `UiCollectionSphereStartDetailPanelTween`, fading in or out per `out`. Returns true when at
   least one of the tweens reported finished this frame (they are started together, so this is
   when the whole panel is done). */

bool UiCollectionSphereUpdateDetailPanelTween(UiCollectionSphere *self, u8 out)
{
  u8 finished;
  int i;

  finished = 0;
  for (i = 0x2b; i < 0x2d; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  }
  for (i = 0x2e; i < 0x30; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  }
  return finished != 0;
}
