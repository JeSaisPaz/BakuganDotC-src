// bdc 0x089ac648 UiPauseSettingsStartTitleTween
#include "bdc.h"

/* Shows the title sprite (`data+0x68`, layout entry 0x1a) and starts its `titleTween` (scale
   1.5). */

void UiPauseSettingsStartTitleTween(UiPauseSettings *self, u8 closing)
{
  GfxSprite *title = ((GfxSprite **)self->base.data)[0x1a];

  title->flags |= 1;
  UiTweenBegin(1.5f, closing, ((GfxSprite **)self->base.data)[0x1a], &self->titleTween, 3);
}
