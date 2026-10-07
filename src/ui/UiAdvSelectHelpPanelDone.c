// bdc 0x0891974c UiAdvSelectHelpPanelDone
#include "bdc.h"

/* Advances the help-panel tweens of sprites 0x22..0x24 (UiTweenUpdate 1.0 -> 1.0 over 16 frames, `hide` as
   fade-out) and returns true once any of them has finished (UiTweenUpdate returns true when done). */

bool UiAdvSelectHelpPanelDone(UiAdvSelect *self, u8 hide)
{
  GfxSprite **sprites = (GfxSprite **)(self->base).data;
  u8 finished = 0;
  s32 i;

  for (i = 0x22; i < 0x25; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  return finished != 0;
}
