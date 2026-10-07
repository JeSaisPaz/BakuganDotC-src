// bdc 0x088fe908 BtlDemoCameraToOrigin
#include "bdc.h"

/* Moves the active 3D camera `g_gfxActiveCamera` to the origin for precise drawing: saves its
   eye (4 floats) into `savedEye`, zeroes the eye (sv.q of the bank zero vector C720), shifts the
   target by `-savedEye` (xyz, w kept) and rebuilds the view (`GfxCameraUpdate` flags 1). Undone
   by `BtlDemoCameraRestore`. */
void BtlDemoCameraToOrigin(float *savedEye)
{
    float *eye = g_gfxActiveCamera->eye;
    float *target;
    float e[4];

    e[0] = eye[0];
    e[1] = eye[1];
    e[2] = eye[2];
    e[3] = eye[3];
    savedEye[0] = e[0];
    savedEye[1] = e[1];
    savedEye[2] = e[2];
    savedEye[3] = e[3];

    eye = g_gfxActiveCamera->eye;
    eye[0] = 0.0f;
    eye[1] = 0.0f;
    eye[2] = 0.0f;
    eye[3] = 0.0f;

    target = g_gfxActiveCamera->target;
    e[0] = savedEye[0];
    e[1] = savedEye[1];
    e[2] = savedEye[2];
    target[0] = target[0] - e[0];
    target[1] = target[1] - e[1];
    target[2] = target[2] - e[2];

    GfxCameraUpdate(g_gfxActiveCamera, 1);
}
