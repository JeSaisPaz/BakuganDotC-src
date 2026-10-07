// bdc 0x088ef500 GameEventInitFadeState
#include "bdc.h"

/* Initialises the four fade bytes `*(ev+0x24)` from the active fader's alpha (`(a - 0.5) * 32`) and
   clears the 4 bytes at `*(ev+0x28)`. */

void GameEventInitFadeState(GameEvent *self)
{
  GfxFader *f;
  u8 v;

  f = GfxGetActiveFader();
  v = (u8)(s32)((f->color[3] - 0.5f) * 32.0f);
  self->fades->pad3 = v;
  self->fades->start = v;
  self->fades->target = v;
  self->fades->cur = v;
  memset(self->fadeWork, 0, 4);
}
