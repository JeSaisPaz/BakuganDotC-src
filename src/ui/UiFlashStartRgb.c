// bdc 0x089a4fb4 UiFlashStartRgb
#include "bdc.h"

/* Variant of `UiFlashStart` that selects which colour channels flash (`r`, `g`, `b` flags) for
   flash slot `slot`. */

void UiFlashStartRgb(float frames, GfxSprite *sprite, u8 startLit, u8 slot, u8 r, u8 g, u8 b)

{
  UiFlashSlot *s = &g_uiFlashSlots[slot];

  memset(s, 0, sizeof(UiFlashSlot));
  s->sprite = sprite;
  s->frames = frames;
  s->r = r;
  s->g = g;
  s->b = b;
  if (startLit == 1) {
    s->t = 1.0f;
    s->sprite->addColor[0] = 1.0f;
    s->sprite->addColor[1] = 1.0f;
    s->sprite->addColor[2] = 1.0f;
    s->sprite->addColor[3] = 1.0f;
    s->phase = 1;
  }
}
