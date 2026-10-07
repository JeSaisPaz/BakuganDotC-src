// bdc 0x0898f86c UiCollectionFigureZoomModelByPad
#include "bdc.h"

/* Zooms the model in the second detail view of `UiCollectionFigure` with
   the held shoulder buttons (`pad->buttons` 0x100 L / 0x200 R): changes the zoom `+0x1290` by
   +0.015 (0x100 L) or -0.015 (0x200 R) per frame, clamped to 0.6..1.4 and resets the idle timer `+0x128c` on change. */

void UiCollectionFigureZoomModelByPad(UiCollectionFigure *self)
{
  PadState *pad;
  float old;
  float zoom;

  pad = self->base.pad;
  old = self->zoom;
  if ((pad->buttons & 0x100) == 0) {
    zoom = old;
    if ((pad->buttons & 0x200) != 0) {
      zoom = old - 0.015f;
      self->zoom = zoom;
      if (zoom < 0.6f) {
        self->zoom = 0.6f;
        zoom = 0.6f;
      }
    }
  } else {
    zoom = old + 0.015f;
    self->zoom = zoom;
    if (!(zoom <= 1.4f)) {
      self->zoom = 1.4f;
      zoom = 1.4f;
    }
  }
  if (old != zoom) {
    self->idleTimer = 0.0f;
  }
}
