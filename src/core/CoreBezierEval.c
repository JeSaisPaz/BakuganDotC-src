// bdc 0x0889f9ac CoreBezierEval
#include "bdc.h"

/* Evaluates a `CoreBezierCtor` quadratic Bezier at `t`: eases the parameter with
   `s = w*2t(1-t) + t^2` (`w` = `weight`) and writes `(1-s)^2*p0 + 2s(1-s)*p1 + s^2*p2` to
   `result.xyz` (`result.w` is left unchanged). The control points are scaled in place, so the
   curve must be reloaded (`CoreBezierSet`) before the next evaluation, and their `w` lanes are
   cleared to 0 (the `vscl.t` + `sv.q` stores write the bank lane S713 = 0 there). */
void CoreBezierEval(float t, CoreBezier *self)
{
    float s = self->weight * 2.0f * t * (1.0f - t) + t * t;
    float u = 1.0f - s;
    float k0 = u * u;
    float k1 = s * 2.0f * u;
    float k2 = s * s;
    ScePspFVector4 term0;
    ScePspFVector4 term1;

    self->p0.x = self->p0.x * k0;
    self->p0.y = self->p0.y * k0;
    self->p0.z = self->p0.z * k0;
    self->p0.w = 0.0f;
    term0.x = self->p0.x;
    term0.y = self->p0.y;
    term0.z = self->p0.z;

    self->p1.x = self->p1.x * k1;
    self->p1.y = self->p1.y * k1;
    self->p1.z = self->p1.z * k1;
    self->p1.w = 0.0f;
    term1.x = self->p1.x;
    term1.y = self->p1.y;
    term1.z = self->p1.z;

    self->p2.x = self->p2.x * k2;
    self->p2.y = self->p2.y * k2;
    self->p2.z = self->p2.z * k2;
    self->p2.w = 0.0f;
    self->result.x = self->p2.x;
    self->result.y = self->p2.y;
    self->result.z = self->p2.z;

    self->result.x = self->result.x + term0.x;
    self->result.y = self->result.y + term0.y;
    self->result.z = self->result.z + term0.z;
    self->result.x = self->result.x + term1.x;
    self->result.y = self->result.y + term1.y;
    self->result.z = self->result.z + term1.z;
}
