// bdc 0x088cbd10 UiTalkBalloonComputeTextSize
#include "bdc.h"

/* Measures the text (`UiTalkBalloonMeasureText`) into `size` and adds one line (16 + `lineHeight`)
   plus 24 px when a portrait is shown (`hasIcon == 1`). */

void UiTalkBalloonComputeTextSize(UiTalkBalloon *self, float *size)
{
  UiTalkBalloonMeasureText(self, size);
  size[1] = self->lineHeight + 16.0f + size[1];
  if (self->hasIcon == 1) {
    size[0] = size[0] + 24.0f;
  }
}
