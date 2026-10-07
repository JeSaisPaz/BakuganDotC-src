// bdc 0x089a43d4 UiMainMenuUpdateItemBoxLights
#include "bdc.h"

/* Animates the lights of the item-box model (`models[4]`) with a small state machine (`lightMode`,
   `lightStep`, `lightDelay`). Mode 0: step 0 switches `Mark_light`, `Reset_light`, `button_light`
   and `Rock_light` off and arms a 20-frame delay; step 1 counts the delay down, then switches
   `Reset_light`, `button_light`, `Rock_light` back on; step 2 pulses `Mark_light` with
   `(1 - cos(t*pi)) / 2` (`lightPulse[1]` += 1/30 per frame, the level kept in `lightPulse[0]`).
   Any other mode: step 0 switches all four lights off once. Nothing happens without a model.
   The cosine was `vcos.s` of the angle times the bank's 2/pi (S703), i.e. `cosf` of the angle. */

void UiMainMenuUpdateItemBoxLights(UiMainMenu *self)

{
  GfxModel *model;
  unsigned char step;
  float level;

  model = self->models[4];
  if (model == NULL) {
    return;
  }
  step = self->lightStep;
  if (self->lightMode == 0) {
    if (step == 0) {
      GfxModelScaleAmbientColorByName(0.0f, model, "Mark_light");
      GfxModelScaleAmbientColorByName(0.0f, self->models[4], "Reset_light");
      GfxModelScaleAmbientColorByName(0.0f, self->models[4], "button_light");
      GfxModelScaleAmbientColorByName(0.0f, self->models[4], "Rock_light");
      self->lightDelay = 20;
      self->lightStep = self->lightStep + 1;
    } else if (step < 2) {
      if (self->lightDelay != 0) {
        self->lightDelay = self->lightDelay - 1;
        return;
      }
      GfxModelScaleAmbientColorByName(1.0f, model, "Reset_light");
      GfxModelScaleAmbientColorByName(1.0f, self->models[4], "button_light");
      GfxModelScaleAmbientColorByName(1.0f, self->models[4], "Rock_light");
      self->lightStep = self->lightStep + 1;
    } else if (step < 3) {
      self->lightPulse[1] = self->lightPulse[1] + 0.033333335f;
      level = (1.0f - __builtin_cosf(self->lightPulse[1] * 3.1415927f)) * 0.5f;
      self->lightPulse[0] = level;
      GfxModelScaleAmbientColorByName(level, model, "Mark_light");
    }
  } else if (step == 0) {
    GfxModelScaleAmbientColorByName(0.0f, model, "Mark_light");
    GfxModelScaleAmbientColorByName(0.0f, self->models[4], "Reset_light");
    GfxModelScaleAmbientColorByName(0.0f, self->models[4], "button_light");
    GfxModelScaleAmbientColorByName(0.0f, self->models[4], "Rock_light");
    self->lightStep = self->lightStep + 1;
  }
}
