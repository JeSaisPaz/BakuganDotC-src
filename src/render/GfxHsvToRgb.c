// bdc 0x089eebe4 GfxHsvToRgb
#include "bdc.h"

/* Converts a colour from HSV (`h` in degrees, wrapped into 0..360; `s`, `v` in 0..1) plus alpha
   into an RGBA float vector `out` (grey when `s` is 0). Returns `out`. */

float *GfxHsvToRgb(float h, float s, float v, float a, float *out)
{
  int sector;
  float frac;
  float p;
  float q;
  float t;

  out[3] = a;
  if (s == 0.0f) {
    out[2] = v;
    out[1] = v;
    out[0] = v;
    return out;
  }
  if (h < 0.0f) {
    h = h + 360.0f;
  }
  if (!(h < 360.0f)) {
    h = h - 360.0f;
  }
  if (!(h < 360.0f)) {
    h = h - 360.0f;
  }
  h = h * 0.016666668f;
  sector = (int)h;
  frac = h - (float)sector;
  p = v * (1.0f - s);
  q = v * (1.0f - frac * s);
  t = v * (1.0f - s * (1.0f - frac));
  switch (sector) {
  case 1:
    out[0] = q;
    out[1] = v;
    out[2] = p;
    return out;
  case 2:
    out[0] = p;
    out[1] = v;
    out[2] = t;
    return out;
  case 3:
    out[0] = p;
    out[1] = q;
    out[2] = v;
    return out;
  case 4:
    out[0] = t;
    out[1] = p;
    out[2] = v;
    return out;
  case 5:
    out[0] = v;
    out[1] = p;
    out[2] = q;
    return out;
  default:
    out[0] = v;
    out[1] = t;
    out[2] = p;
    return out;
  }
}
