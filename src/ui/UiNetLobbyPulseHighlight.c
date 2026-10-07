// bdc 0x08941cf0 UiNetLobbyPulseHighlight
#include "bdc.h"

/* Pulses the alpha of the highlight rectangle `+0x98` of `UiNetLobby`: a 120-frame
   triangle wave between 0.8 and 0.2 (counter `+0x78`; `reset` restarts it at 0.6), scaled by the
   master alpha `+0x84`; the rectangle is hidden while the result is 0 (`GfxRectSetVisible`). */

void UiNetLobbyPulseHighlight(UiNetLobby *self, u8 reset)

{
  float alpha;
  int step;

  alpha = 0.6f;
  if (reset == '\0') {
    step = self->pulseTimer + 1;
    self->pulseTimer = step;
    if (0x77 < step) {
      self->pulseTimer = 0;
    }
    step = self->pulseTimer;
    if (0x3b < step) {
      step = 0x78 - step;
    }
    alpha = (0.6f + 0.2f) - 0.6f * (float)step * 0.016666668f;
  }
  else {
    self->pulseTimer = 0;
  }
  alpha = alpha * self->listAlpha;
  ((GfxRect *)self->highlightRect)->color[3] = alpha;
  if (alpha != 0.0f) {
    GfxRectSetVisible((GfxRect *)self->highlightRect,'\x01');
    return;
  }
  GfxRectSetVisible((GfxRect *)self->highlightRect,'\0');
  return;
}
