// bdc 0x089a4cf4 UiOrbitPoint
#include "bdc.h"

/* Computes a point on an ellipse-like orbit: `out[0] = cx + (radius - px) * cos(angle)`, `out[1] =
   cy + (radius - py) * sin(angle)` (VFPU `vcos`/`vsin` on `angle` scaled to quarter turns by the bank's
   2/π). `radius` is an unsigned integer converted to float. Used to place the main menu's item models
   around the carousel (`UiMainMenuCreateModels`, `UiMainMenuStepCarousel`). */

void UiOrbitPoint(float angle, float cx, float cy, float px, float py, float *out, u32 radius)
{
    float c = __builtin_cosf(angle);
    float s = __builtin_sinf(angle);

    out[0] = cx + ((float)radius - px) * c;
    out[1] = cy + ((float)radius - py) * s;
}
