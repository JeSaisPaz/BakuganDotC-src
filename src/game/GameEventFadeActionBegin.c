// bdc 0x088f28fc GameEventFadeActionBegin
#include "bdc.h"

/* Start method (slot 2) of the screen-fade action (vtable `0x08af42d4`, 0xc bytes): unless flagged
   (`+0xb`), seeds bytes 0 and 2 of the fade record from the active fader's alpha. */

void GameEventFadeActionBegin(GameEventFadeAction *self)

{
  GfxFader *fader;
  GameEventFadeRecord *rec;
  s8 v;

  if (self->skip == '\0') {
    fader = GfxGetActiveFader();
    rec = self->rec;
    v = (s8)(int)((fader->color[3] - 0.5f) * 32.0f);
    self->rec->start = v;
    rec->cur = v;
  }
  return;
}

