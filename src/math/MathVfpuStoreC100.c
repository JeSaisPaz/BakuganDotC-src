// bdc 0x089beeb4 MathVfpuStoreC100
#include "bdc.h"

/* Stores the 4-float vector `v` (the VFPU column C100 its callers leave loaded, typically the
   clip-space result of GfxCameraProjectPoint) to `out`. */
void MathVfpuStoreC100(float *out, ScePspFVector4 v)
{
    out[0] = v.x;
    out[1] = v.y;
    out[2] = v.z;
    out[3] = v.w;
}
