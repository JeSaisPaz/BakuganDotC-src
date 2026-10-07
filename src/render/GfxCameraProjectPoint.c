// bdc 0x089e3554 GfxCameraProjectPoint
#include "bdc.h"

/* Projects the world point `pos` to screen coordinates in `out` using the camera's cached
   view-projection matrix (`GfxCameraUpdateViewProj`, `GfxProjectToScreen`). Returns the
   clip-space vector `GfxProjectToScreen` returns (VFPU C100, read by the HUD callers). */

ScePspFVector4 GfxCameraProjectPoint(GfxCamera *cam, float *out, const float *pos)
{
    GfxCameraUpdateViewProj(cam);
    return GfxProjectToScreen(out, &cam->viewProj, (float *)pos);
}
