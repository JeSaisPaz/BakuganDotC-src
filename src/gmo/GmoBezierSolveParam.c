// bdc 0x089dad70 GmoBezierSolveParam
#include "bdc.h"

/* Solves a 1-D cubic Bezier time curve `(x0, c0, c1, x1)` for the curve parameter at which it
   reaches `x`: up to 8 de Casteljau halvings narrow the interval until the segment is close to
   linear, then the remaining quadratic is solved (`sqrt`). Returns the parameter in 0..1. */

float GmoBezierSolveParam(float x0, float c0, float c1, float x1, float x)
{
  float span = x1 - x0;
  float tol = span * 0.01f;
  float negTol;
  float lo;
  float hi;
  float slope;
  float mid;
  float xm;
  float tm;
  float a;
  float b;
  float dx;
  float dt;
  float disc;
  float s;
  int i;

  if (tol < 0.0001f) {
    tol = 0.0001f;
  }
  lo = 0.0f;
  negTol = -tol;
  hi = 1.0f;
  for (i = 0; i < 8; i++) {
    /* segment span already within tolerance */
    if (!(span <= negTol) && !(tol <= span)) {
      break;
    }
    /* control points nearly evenly spaced: segment is close to linear */
    slope = (c1 - c0) / span - 0.33333334f;
    if (!(slope <= -0.01f) && slope < 0.01f) {
      break;
    }
    mid = (c0 + c1) * 0.5f;
    xm = (x0 + c0 * 3.0f + c1 * 3.0f + x1) * 0.125f;
    tm = (lo + hi) * 0.5f;
    if (x < xm) {
      c0 = (x0 + c0) * 0.5f;
      hi = tm;
      x1 = xm;
      span = xm - x0;
      c1 = (c0 + mid) * 0.5f;
    } else {
      c1 = (c1 + x1) * 0.5f;
      lo = tm;
      x0 = xm;
      span = x1 - xm;
      c0 = (mid + c1) * 0.5f;
    }
  }
  dx = x0 - x;
  b = (c0 - x0) * 3.0f;
  a = span - b;
  dt = hi - lo;
  if (a == 0.0f) {
    if (b == 0.0f) {
      return dt * 0.5f + lo;
    }
    return dt * (-dx / b) + lo;
  }
  disc = b * b - a * 4.0f * dx;
  if (disc < 0.0f) {
    disc = 0.0f;
  }
  s = __builtin_sqrtf(disc);
  if (a + b < 0.0f) {
    s = -s;
  }
  return dt * ((s - b) / (a * 2.0f)) + lo;
}
