// bdc 0x089a5260 UiCursorGlowStep
#include "bdc.h"

/* Advances the shared cursor-glow oscillator (`g_uiCursorGlow`) between 0 and 0.3 over 40 frames
   (20 at frameSkip != 0) and applies `0.3 - level` as red/green colour-add (blue 0, alpha 1) to
   `sprite`. */

void UiCursorGlowStep(GfxSprite *sprite)
{
  float step;
  float level;

  if (g_gfxDisplay->frameSkip == 0) {
    step = 40.0f;
  }
  else {
    step = 20.0f;
  }
  if (g_uiCursorGlow.falling == 0) {
    g_uiCursorGlow.level = g_uiCursorGlow.level + 0.3f / step;
    if (!(g_uiCursorGlow.level < 0.3f)) {
      g_uiCursorGlow.level = 0.3f;
      g_uiCursorGlow.falling = 1;
    }
  }
  else {
    g_uiCursorGlow.level = g_uiCursorGlow.level - 0.3f / step;
    if (g_uiCursorGlow.level <= 0.0f) {
      g_uiCursorGlow.level = 0.0f;
      g_uiCursorGlow.falling = 0;
    }
  }
  level = 0.3f - g_uiCursorGlow.level;
  sprite->addColor[2] = 0.0f;
  sprite->addColor[3] = 1.0f;
  sprite->addColor[0] = level;
  sprite->addColor[1] = level;
}
