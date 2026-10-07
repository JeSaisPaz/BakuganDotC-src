// bdc 0x088bdf84 GameFieldCameraRecenterEnd
#include "bdc.h"

/* Ends camera-relative drawing (`GameFieldCameraRecenterBegin`): restores the eye from
   `savedEye`, shifts the look-at back and rebuilds the view. */
void GameFieldCameraRecenterEnd(float *savedEye)
{
    float *target;

    g_gfxActiveCamera->eye[0] = savedEye[0];
    g_gfxActiveCamera->eye[1] = savedEye[1];
    g_gfxActiveCamera->eye[2] = savedEye[2];
    g_gfxActiveCamera->eye[3] = savedEye[3];
    /* vadd.t: only x/y/z are shifted, w keeps its value. */
    target = g_gfxActiveCamera->target;
    target[0] = target[0] + savedEye[0];
    target[1] = target[1] + savedEye[1];
    target[2] = target[2] + savedEye[2];
    GfxCameraUpdate(g_gfxActiveCamera, 1);
}
