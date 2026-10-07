// bdc 0x08975b30 UiCollectionMenuStartItemBoxZoom
#include "bdc.h"

/* Starts the item-box zoom of `UiCollectionMenu`: clears the zoom record
   (`itemBoxTween` through `spinAngle`, 0x28 bytes). With `out` = 0 (zoom in) it puts the item box
   at x = 40 with alpha 0, zeroes `spinT`/`spinScale`, rebuilds the model's root matrix as a Y
   rotation of 0 times an X rotation of 0.5 rad, scales rows 0-2 by 1.2, copies the box position
   into row 3 (w = 1), then calls the item-box virtual methods at vtable entries 6
   (GfxModelSetMotionSpeed, with 100.0f) and 7 (GfxModelUpdateAndApplyMotion). With `out` != 0 it
   only sets `spinScale` and alpha to 1 and `spinT` to 0. */

void UiCollectionMenuStartItemBoxZoom(UiCollectionMenu *self, u8 out)
{
  const VtblEntry *entry;
  float *m;
  float *pos;
  float c;
  float s;
  float y;
  float z;
  int i;

  /* itemBoxTween .. spinAngle (0x524..0x54b) */
  memset(self->itemBoxTween, 0, 0x28);
  if (out == 0) {
    self->itemBox->pos[0] = 40.0f;
    self->itemBox->ambient[3] = 0.0f;
    self->spinScale = 0.0f;
    self->spinT = 0.0f;
    /* rootMatrix = vrot Y rotation of 0: rows (cos 0, 0, -sin 0, 0), (0, 1, 0, 0), (sin 0, 0, cos 0, 0),
       (0, 0, 0, 1); -sin 0 is -0.0f. */
    m = self->itemBox->data->rootMatrix;
    m[0] = 1.0f;
    m[1] = 0.0f;
    m[2] = -0.0f;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 1.0f;
    m[11] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
    /* Each row *= X rotation of 0.5 rad (0.5 * S703 quarter turns): R0 = (1, 0, 0, 0),
       R1 = (0, c, s, 0), R2 = (0, -s, c, 0), R3 = (0, 0, 0, 1) (vmmul.q E200, E100, E000). */
    m = self->itemBox->data->rootMatrix;
    c = __builtin_cosf(0.5f);
    s = __builtin_sinf(0.5f);
    for (i = 0; i < 4; i++) {
      y = m[i * 4 + 1];
      z = m[i * 4 + 2];
      m[i * 4 + 1] = y * c - z * s;
      m[i * 4 + 2] = y * s + z * c;
    }
    /* rows 0-2 (all four lanes) *= 1.2 */
    m = self->itemBox->data->rootMatrix;
    for (i = 0; i < 12; i++) {
      m[i] = m[i] * 1.20000005f;
    }
    /* row 3 = pos */
    pos = self->itemBox->pos;
    m = self->itemBox->data->rootMatrix;
    m[12] = pos[0];
    m[13] = pos[1];
    m[14] = pos[2];
    m[15] = pos[3];
    self->itemBox->data->rootMatrix[15] = 1.0f;
    entry = &((const VtblEntry *)self->itemBox->base.vtable)[6];
    ((void (*)(void *, float))entry->fn)((u8 *)self->itemBox + entry->delta, 100.0f);
    entry = &((const VtblEntry *)self->itemBox->base.vtable)[7];
    ((void (*)(void *))entry->fn)((u8 *)self->itemBox + entry->delta);
  }
  else {
    self->spinScale = 1.0f;
    self->itemBox->ambient[3] = 1.0f;
    self->spinT = 0.0f;
  }
}
