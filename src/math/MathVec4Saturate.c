// bdc 0x08a296ec MathVec4Saturate
#include "bdc.h"

/* Clamps the four components of `v` to [0, 1] in place (`vsat0.q`). Returns `v`. */
float *MathVec4Saturate(float *v)
{
    v[0] = VfSat0(v[0]);
    v[1] = VfSat0(v[1]);
    v[2] = VfSat0(v[2]);
    v[3] = VfSat0(v[3]);
    return v;
}
