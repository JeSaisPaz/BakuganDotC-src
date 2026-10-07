// bdc 0x088bdf2c GameFieldCameraRecenterBegin
#include "bdc.h"

/* Starts camera-relative drawing: saves the field camera's (`g_gfxActiveCamera`) eye into `savedEye`,
   moves the eye to the origin, shifts the look-at by the same amount and rebuilds the view
   (`GfxCameraUpdate` 1). Undone by `GameFieldCameraRecenterEnd`. */
void GameFieldCameraRecenterBegin(float *savedEye)
{
    float *target;

    savedEye[0] = g_gfxActiveCamera->eye[0];
    savedEye[1] = g_gfxActiveCamera->eye[1];
    savedEye[2] = g_gfxActiveCamera->eye[2];
    savedEye[3] = g_gfxActiveCamera->eye[3];
    /* sv.q C720: the bank's zero quad. */
    g_gfxActiveCamera->eye[0] = 0.0f;
    g_gfxActiveCamera->eye[1] = 0.0f;
    g_gfxActiveCamera->eye[2] = 0.0f;
    g_gfxActiveCamera->eye[3] = 0.0f;
    /* vsub.t: only x/y/z are shifted, w keeps its value. */
    target = g_gfxActiveCamera->target;
    target[0] = target[0] - savedEye[0];
    target[1] = target[1] - savedEye[1];
    target[2] = target[2] - savedEye[2];
    GfxCameraUpdate(g_gfxActiveCamera, 1);
}
