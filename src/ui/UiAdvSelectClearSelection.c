// bdc 0x0891a6f8 UiAdvSelectClearSelection
#include "bdc.h"

/* Releases the Bakugan model (`UiAdvSelectReleaseModel`), shows the selected candidate's partner
   as a shadow picture (`UiAdvSelectSetBakuganPicture`) and starts hiding the name panels
   (`UiAdvSelectTweenNamePanels`). */

void UiAdvSelectClearSelection(UiAdvSelect *self)

{
  UiAdvSelectReleaseModel(self);
  UiAdvSelectSetBakuganPicture(self, ((GfxSprite **)self->base.data)[3],
                               self->candidates[self->cursor].partner, false);
  UiAdvSelectTweenNamePanels(self, 1, 0);
}
