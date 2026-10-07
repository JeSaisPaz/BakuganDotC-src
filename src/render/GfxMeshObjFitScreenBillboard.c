// bdc 0x088268f8 GfxMeshObjFitScreenBillboard
#include "bdc.h"

/* Turns a mesh object (`GfxMeshObjCtor`) into a camera-facing billboard of size `scale` and maps
   its texture to the screen area behind it: copies the active camera's billboard matrix
   (`GfxCamera.billboard`, `g_gfxActiveCamera`) into the world matrix, scales the X/Y rows by
   `-scale`, projects the position with `GfxCameraProjectPointFacing` and sets the texture
   scale/offset (`+0x170..+0x17c`) so the screen-copy texture lines up (FOV correction from
   `GfxCamera.fov`). Does nothing while sub-state `+0x1c` <= 0. Used by the distortion/heat-haze
   states. */

void GfxMeshObjFitScreenBillboard(float scale, GfxMeshObj *self)
{
  const float *bb;
  s32 i;
  float screen[4] __attribute__((aligned(16)));
  float size;
  float scaleU;
  float scaleV;

  if (self->step <= 0)
    return;

  bb = (const float *)&g_gfxActiveCamera->billboard;
  for (i = 0; i < 16; i++)
    self->basis[i] = bb[i];
  /* rows 0 and 1 (all four lanes) *= -scale; row 2 *= 1 is unchanged */
  for (i = 0; i < 8; i++)
    self->basis[i] = self->basis[i] * -scale;
  GfxCameraProjectPointFacing(g_gfxActiveCamera, screen, self->pos);
  size = scale * ((70.0f - g_gfxActiveCamera->fov) * 0.162f * 0.0333333f + 0.178f);
  scaleU = screen[3] * size * 0.56666666f;
  self->texScaleU = scaleU;
  scaleV = screen[3] * size;
  self->texScaleV = scaleV;
  self->texOffsetU = screen[0] * 256.0f * 0.0020833334f * 0.00390625f - scaleU * 0.469f;
  self->texOffsetV = screen[1] * 256.0f * 0.0036764706f * 0.00390625f - scaleV * 0.459f;
}
