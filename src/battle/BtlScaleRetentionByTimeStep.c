// bdc 0x08863b54 BtlScaleRetentionByTimeStep
#include "bdc.h"

/* Adapts a per-frame retention factor to the current time step: returns `1 - (1 - keep) * step`,
   where `step` is the global motion time scale from `GfxGetMotionTimeScale`, so velocities damp
   frame-rate independently. */
float BtlScaleRetentionByTimeStep(float keep)
{
    float step;

    step = GfxGetMotionTimeScale();
    return 1.0f - (1.0f - keep) * step;
}
