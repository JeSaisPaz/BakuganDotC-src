// bdc 0x0890c02c UiQuadBezier3
#include "bdc.h"

/* Evaluates a quadratic Bezier curve at `t` for 3D points: `out = (1-t)^2 * p0 + 2t(1-t) * p1 + t^2
   * p2`. */

void UiQuadBezier3(float t, void *unused, float *out, float *p0, float *p1, float *p2)
{
  float inv = 1.0f - t;
  float w0 = inv * inv;
  float w2 = t * t;
  float w1 = (t * 2.0f) * inv;

  out[0] = w0 * p0[0] + w1 * p1[0] + w2 * p2[0];
  out[1] = w0 * p0[1] + w1 * p1[1] + w2 * p2[1];
  out[2] = w0 * p0[2] + w1 * p1[2] + w2 * p2[2];
}
