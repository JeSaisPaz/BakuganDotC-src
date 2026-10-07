// bdc 0x08a16948 GmoBezierSolveParamB
#include "bdc.h"

/* Second copy of `GmoBezierSolveParam`: solves a 1-D cubic Bezier time curve `(x0, c0, c1, x1)`
   for the parameter at which it reaches `x` (up to 8 de Casteljau halvings, then the remaining
   quadratic). Returns the parameter in 0..1. The float literals live in .rodata at
   0x08aa52cc..0x08aa52f0. */

float GmoBezierSolveParamB(float x0, float c0, float c1, float x1, float x)
{
  float span = x1 - x0;
  float tol = span * 0.01f;
  float negTol;
  float lo;
  float hi;
  float slope;
  float mid;
  float xm;
  float a;
  float b;
  float dx;
  float disc;
  float s;
  int i;

  if (tol < 0.0001f) {
    tol = 0.0001f;
    negTol = -0.0001f;
  } else {
    negTol = -tol;
  }
  hi = 1.0f;
  lo = 0.0f;
  for (i = 0; i < 8; i++) {
    span = x1 - x0;
    /* segment span already within tolerance */
    if (negTol < span && span < tol) {
      break;
    }
    /* control points nearly evenly spaced: segment is close to linear */
    slope = (c1 - c0) / span - 0.33333334f;
    if (-0.01f < slope && slope < 0.01f) {
      break;
    }
    xm = (c0 * 3.0f + x0 + c1 * 3.0f + x1) * 0.125f;
    mid = (c0 + c1) * 0.5f;
    if (x < xm) {
      c0 = (c0 + x0) * 0.5f;
      x1 = xm;
      hi = (hi + lo) * 0.5f;
      c1 = (mid + c0) * 0.5f;
    } else {
      c1 = (c1 + x1) * 0.5f;
      x0 = xm;
      lo = (hi + lo) * 0.5f;
      c0 = (mid + c1) * 0.5f;
    }
  }
  if (i == 8) {
    span = x1 - x0;
  }
  b = (c0 - x0) * 3.0f;
  a = span - b;
  dx = x0 - x;
  if (a == 0.0f) {
    if (b == 0.0f) {
      return 0.5f * (hi - lo) + lo;
    }
    return (-dx / b) * (hi - lo) + lo;
  }
  disc = b * b + dx * (a * -4.0f);
  if (disc < 0.0f) {
    disc = 0.0f;
  }
  s = __builtin_sqrtf(disc);
  if (b + a < 0.0f) {
    s = -s;
  }
  return ((s - b) / (a + a)) * (hi - lo) + lo;
}
