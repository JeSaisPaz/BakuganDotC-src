// bdc 0x0884b030 BtlCameraRebaseToOrigin
#include "bdc.h"

/* Saves the active camera's 16-byte eye vector into `saved`, sets the eye to the origin
   (0, 0, 0, 0: the VFPU bank constant C720), shifts the look-at target's x/y/z by minus the saved
   eye (its w is kept), and refreshes the camera with GfxCameraUpdate mode 1, so models can be
   drawn in camera-relative coordinates. Undone by `BtlCameraRestoreFromOrigin`. */
void BtlCameraRebaseToOrigin(float *saved)
{
    float *eye = g_gfxActiveCamera->eye;
    float *target;

    saved[0] = eye[0];
    saved[1] = eye[1];
    saved[2] = eye[2];
    saved[3] = eye[3];

    eye = g_gfxActiveCamera->eye;
    eye[0] = 0.0f;
    eye[1] = 0.0f;
    eye[2] = 0.0f;
    eye[3] = 0.0f;

    target = g_gfxActiveCamera->target;
    target[0] = target[0] - saved[0];
    target[1] = target[1] - saved[1];
    target[2] = target[2] - saved[2];

    GfxCameraUpdate(g_gfxActiveCamera, 1);
}
