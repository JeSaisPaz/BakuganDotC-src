// bdc 0x089a4f0c UiFlashStart
#include "bdc.h"

/* Starts a white flash on `sprite` in flash slot `slot` (16-byte records at `0x08b01050`): stores
   the sprite and the duration in frames, enables all three colour channels and, when `startLit` is
   1, starts from full white (colour-add 1,1,1) fading back. Step with `UiFlashStep`. */

void UiFlashStart(float frames, GfxSprite *sprite, u8 startLit, u8 slot)

{
  UiFlashSlot *s = &g_uiFlashSlots[slot];

  memset(s, 0, sizeof(UiFlashSlot));
  s->sprite = sprite;
  s->frames = frames;
  s->r = 1;
  s->g = 1;
  s->b = 1;
  if (startLit == 1) {
    s->t = 1.0f;
    s->sprite->addColor[0] = 1.0f;
    s->sprite->addColor[1] = 1.0f;
    s->sprite->addColor[2] = 1.0f;
    s->sprite->addColor[3] = 1.0f;
    s->phase = 1;
  }
}
