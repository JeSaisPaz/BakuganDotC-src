// bdc 0x0884b088 BtlCameraRestoreFromOrigin
#include "bdc.h"

/* Undo of `BtlCameraRebaseToOrigin` on the active camera `g_gfxActiveCamera`: copies the
   16-byte eye vector back from `saved`, adds `saved`'s x/y/z to the look-at `target` (its w is
   kept), and rebuilds the view with `GfxCameraUpdate` (flags 1). `saved` must be 16-byte
   aligned. */
void BtlCameraRestoreFromOrigin(float *saved)
{
    float *eye = g_gfxActiveCamera->eye;
    float *target;

    eye[0] = saved[0];
    eye[1] = saved[1];
    eye[2] = saved[2];
    eye[3] = saved[3];

    target = g_gfxActiveCamera->target;
    target[0] = target[0] + saved[0];
    target[1] = target[1] + saved[1];
    target[2] = target[2] + saved[2];

    GfxCameraUpdate(g_gfxActiveCamera, 1);
}
