// bdc 0x0889f97c CoreBezierSet
#include "bdc.h"

/* Loads a `CoreBezierCtor` curve: stores the easing weight and copies the three control points
   (16-byte vectors, `lv.q`/`sv.q` through C000) into `p0`, `p1`, `p2`. */
void CoreBezierSet(float weight, CoreBezier *self, const float *p0, const float *p1, const float *p2)
{
    self->weight = weight;
    self->p0 = *(const ScePspFVector4 *)p0;
    self->p1 = *(const ScePspFVector4 *)p1;
    self->p2 = *(const ScePspFVector4 *)p2;
}
