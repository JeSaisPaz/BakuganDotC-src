// bdc 0x088fe960 BtlDemoCameraRestore
#include "bdc.h"

/* Restores the active 3D camera `g_gfxActiveCamera` after `BtlDemoCameraToOrigin`: eye =
   `savedEye` (4 floats), target xyz += `savedEye` (w kept), then `GfxCameraUpdate` flags 1. */
void BtlDemoCameraRestore(float *savedEye)
{
    float *eye = g_gfxActiveCamera->eye;
    float *target;
    float s[4];

    s[0] = savedEye[0];
    s[1] = savedEye[1];
    s[2] = savedEye[2];
    s[3] = savedEye[3];
    eye[0] = s[0];
    eye[1] = s[1];
    eye[2] = s[2];
    eye[3] = s[3];

    target = g_gfxActiveCamera->target;
    s[0] = savedEye[0];
    s[1] = savedEye[1];
    s[2] = savedEye[2];
    target[0] = target[0] + s[0];
    target[1] = target[1] + s[1];
    target[2] = target[2] + s[2];

    GfxCameraUpdate(g_gfxActiveCamera, 1);
}
