// bdc 0x088cbd80 UiTalkBalloonComputeFrameSize
#include "bdc.h"

/* Computes the balloon frame size into `size`: measured text (`UiTalkBalloonMeasureText`) widened
   by 3 % twice plus 8 (and 24 with a portrait), height ×1.5 plus 27 + line height `+0x54`. */

void UiTalkBalloonComputeFrameSize(UiTalkBalloon *self, float *size)

{
  float width;
  float height;

  UiTalkBalloonMeasureText(self, size);
  width = size[0] * 1.03f;
  size[0] = width;
  if (self->hasIcon == 1) {
    width = width + 24.0f;
    size[0] = width;
  }
  height = size[1];
  size[0] = width * 1.03f + 8.0f;
  size[1] = self->lineHeight + 27.0f + height * 1.5f;
}
