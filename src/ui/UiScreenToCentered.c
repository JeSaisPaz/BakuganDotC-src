// bdc 0x089a5aac UiScreenToCentered
#include "bdc.h"

/* Converts a screen position to coordinates relative to the screen centre: `out = (x - 240, y -
   136)`. */

void UiScreenToCentered(float x, float y, float *out)

{
  *out = x - 240.0f;
  out[1] = y - 136.0f;
  return;
}

