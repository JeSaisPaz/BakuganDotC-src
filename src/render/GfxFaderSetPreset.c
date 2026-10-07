// bdc 0x089eddec GfxFaderSetPreset
#include "bdc.h"

/* Sets a fader's colours from a preset: 1 fade to black, 2 fade to the display clear colour, 0 fade
   to white (end alpha 1, start = end with alpha 0), 4 swap start and end (reverse); 3 and > 4 do
   nothing. */

void GfxFaderSetPreset(GfxFader *self, u32 preset)
{
  float tmp[4];
  int i;

  if (preset > 4) {
    return;
  }
  if (preset == 1) {
    /* fade to black */
    self->end[0] = 0.0f;
    self->end[1] = 0.0f;
    self->end[2] = 0.0f;
    self->end[3] = 1.0f;
    for (i = 0; i < 4; i++) {
      self->start[i] = self->end[i];
    }
    self->start[3] = 0.0f;
  } else if (preset == 2) {
    /* fade to the display clear colour */
    for (i = 0; i < 4; i++) {
      self->end[i] = g_gfxDisplay->clearColor[i];
    }
    self->end[3] = 1.0f;
    for (i = 0; i < 4; i++) {
      self->start[i] = self->end[i];
    }
    self->start[3] = 0.0f;
  } else if (preset == 3) {
    return;
  } else if (preset == 4) {
    /* reverse: swap start and end through a stack temp */
    for (i = 0; i < 4; i++) {
      tmp[i] = self->end[i];
    }
    for (i = 0; i < 4; i++) {
      self->end[i] = self->start[i];
    }
    for (i = 0; i < 4; i++) {
      self->start[i] = tmp[i];
    }
  } else {
    /* preset 0: fade to white */
    self->end[0] = 1.0f;
    self->end[1] = 1.0f;
    self->end[2] = 1.0f;
    self->end[3] = 1.0f;
    for (i = 0; i < 4; i++) {
      self->start[i] = self->end[i];
    }
    self->start[3] = 0.0f;
  }
}
