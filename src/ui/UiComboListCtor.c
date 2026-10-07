// bdc 0x089b2edc UiComboListCtor
#include "bdc.h"

/* Constructor of the combo list screen, task id 3002 (0xbba) (base `UiScreenCtor`, vtable
   `0x08af51dc`). Object size 0x98. Clears its fields, saves the display frame-skip
   (`g_gfxDisplay->frameSkip`) at `+0x84` and forces it to 0, and sets the zoom scale `+0x80` to
   1.0. The screen shows one of the combo list pages (`"combolist_001"`..., loaded from
   `"data/2d/<lang>/combolist/%s.lzs"`); the pause menu opens it as task 3002
   (`UiPauseOpenComboListPhase`). */

UiComboList *UiComboListCtor(UiComboList *self)

{
  s32 skip;
  
  UiScreenCtor((CoreTask *)self);
  (self->base).base.vtable = g_uiComboListVtbl;
  self->pack = (CoreNode *)0x0;
  self->combo = 0;
  self->step = 0;
  self->animFrame = 0;
  self->zoom = 0.0;
  self->zoomY = 0.0;
  skip = g_gfxDisplay->frameSkip;
  self->bounceAmplitude = 1.0;
  self->savedFrameSkip = skip;
  self->bgFrame = 0;
  g_gfxDisplay->frameSkip = 0;
  return self;
}

