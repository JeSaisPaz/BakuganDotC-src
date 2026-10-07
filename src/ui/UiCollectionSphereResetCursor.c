// bdc 0x0897fe48 UiCollectionSphereResetCursor
#include "bdc.h"

/* Resets the cursor of the sphere (Bakugan figure) collection screen
   (`UiCollectionSphere`, task 312): resets the cursor glow
   (`UiCursorGlowReset`) and `pulse`; only in cursor mode 0 it then resets the cursor pulse
   (`tweens[6]`, `UiPulseReset`), shows the cursor sprite 6 centred, unscaled, alpha 1, add colour
   0.3 grey, at its saved depth over the selected cell, starts its ghost pulse with sprite 70
   (`UiPulseInit`, `tweens[70]`), resets the cell sprites 0..5 (scale 1, saved depth, add colour
   0.3 grey and frame "selected" on the cursor cell, black and unselected elsewhere,
   `UiCollectionSphereSetCellFrame`) and sprites 27..32 (scale 1, saved depth), rebuilds the GMO
   root matrix of every loaded cell model as the Y rotation by 3.14 multiplied (vmmul) with rotX(tilt)
   (`UiCollectionSphereGetModelTilt`) scaled by 0.4 / 0.18 / 0.2 for cell type 0 / 1 / 2 (stored
   in `modelScale`; other types unscaled), sets its height to
   `bobBaseY + (1 - cos(bobT * pi)) * 0.5 * 0.2` and copies `pos` into the translation row with
   w = 1, and finally enables the preview (`UiCollectionSphereEnablePreview`).
   The VFPU trig (bank S703 = 2/pi times the angle, quarter-turn vrot/vcos) is lifted to cosf/sinf. */

void UiCollectionSphereResetCursor(UiCollectionSphere *self)

{
  float rot[16];
  float prod[16];
  GfxSprite **sprites;
  GfxSprite *cursor;
  GfxModel *model;
  float *matrix;
  float baseY;
  float cosine;
  float tilt;
  float c;
  float sn;
  float s;
  int i;
  int j;
  int k;
  u8 type;

  UiCursorGlowReset();
  self->pulse = 0.0f;
  if (self->cursorMode != 0) {
    return;
  }
  UiPulseReset((UiPulse *)&self->tweens[6]);
  cursor = ((GfxSprite **)self->base.data)[6];
  cursor->flags |= 1;
  GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[6]);
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[6], 1.0f, 1.0f, 0.0f);
  ((GfxSprite **)self->base.data)[6]->alpha = 1.0f;
  cursor = ((GfxSprite **)self->base.data)[6];
  cursor->addColor[3] = 1.0f;
  cursor->addColor[0] = 0.3f;
  cursor->addColor[1] = 0.3f;
  cursor->addColor[2] = 0.3f;
  ((GfxSprite **)self->base.data)[6]->posZ = self->spriteZ[6];
  sprites = (GfxSprite **)self->base.data;
  sprites[6]->posX = sprites[self->cursor]->posX;
  sprites = (GfxSprite **)self->base.data;
  sprites[6]->posY = sprites[self->cursor]->posY;
  sprites = (GfxSprite **)self->base.data;
  UiPulseInit(sprites[6], sprites[70], (UiPulse *)&self->tweens[70]);

  /* cell sprites 0..5 */
  for (i = 0; i < 6; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    if (i == self->cursor) {
      cursor = ((GfxSprite **)self->base.data)[i];
      cursor->addColor[0] = 0.3f;
      cursor->addColor[1] = 0.3f;
      cursor->addColor[2] = 0.3f;
      cursor->addColor[3] = 1.0f;
      UiCollectionSphereSetCellFrame(self, ((GfxSprite **)self->base.data)[i], 1);
    } else {
      cursor = ((GfxSprite **)self->base.data)[i];
      cursor->addColor[0] = 0.0f;
      cursor->addColor[1] = 0.0f;
      cursor->addColor[2] = 0.0f;
      cursor->addColor[3] = 1.0f;
      UiCollectionSphereSetCellFrame(self, ((GfxSprite **)self->base.data)[i], 0);
    }
  }

  /* sprites 27..32 */
  for (i = 27; i < 33; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }

  for (i = 0; i < 7; i++) {
    if (self->models[i] == NULL) {
      continue;
    }

    /* rootMatrix = rotation about Y by 3.14 (vrot rows [C,0,-S,0] / identity / [S,0,C,0] / identity) */
    matrix = self->models[i]->data->rootMatrix;
    c = __builtin_cosf(3.14f);
    sn = __builtin_sinf(3.14f);
    matrix[0] = c;
    matrix[1] = 0.0f;
    matrix[2] = -sn;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = sn;
    matrix[9] = 0.0f;
    matrix[10] = c;
    matrix[11] = 0.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;

    /* vmmul.q E200, E100 (matrix), E000 (rot): field j lane k = sum_m matrix[j][m] * rot[m][k],
       i.e. each row (x,y,z,w) -> (x, c*y - s*z, s*y + c*z, w)
       (pointer read before the call) */
    matrix = self->models[i]->data->rootMatrix;
    tilt = UiCollectionSphereGetModelTilt(self, (u8)i);
    c = __builtin_cosf(tilt);
    sn = __builtin_sinf(tilt);
    rot[0] = 1.0f;
    rot[1] = 0.0f;
    rot[2] = 0.0f;
    rot[3] = 0.0f;
    rot[4] = 0.0f;
    rot[5] = c;
    rot[6] = sn;
    rot[7] = 0.0f;
    rot[8] = 0.0f;
    rot[9] = -sn;
    rot[10] = c;
    rot[11] = 0.0f;
    rot[12] = 0.0f;
    rot[13] = 0.0f;
    rot[14] = 0.0f;
    rot[15] = 1.0f;
    for (j = 0; j < 4; j++) {
      for (k = 0; k < 4; k++) {
        prod[j * 4 + k] = matrix[j * 4 + 0] * rot[0 * 4 + k] + matrix[j * 4 + 1] * rot[1 * 4 + k] +
                          matrix[j * 4 + 2] * rot[2 * 4 + k] + matrix[j * 4 + 3] * rot[3 * 4 + k];
      }
    }
    for (j = 0; j < 16; j++) {
      matrix[j] = prod[j];
    }

    /* scale the first three rows (all four lanes) by the cell type's model scale */
    type = self->cells[i].type;
    if (type < 3) {
      s = (type == 0) ? 0.4f : (type == 1) ? 0.18f : 0.2f;
      self->modelScale[i] = s;
      matrix = self->models[i]->data->rootMatrix;
      for (j = 0; j < 12; j++) {
        matrix[j] = matrix[j] * s;
      }
    }

    /* bob height: bobBaseY + (1 - cos(bobT * pi)) * 0.5 * 0.2 */
    baseY = self->bobBaseY;
    cosine = __builtin_cosf(self->bobT * 3.1415927f);
    self->models[i]->pos[1] = baseY + (1.0f - cosine) * 0.5f * 0.2f;

    /* translation row = pos (all four words), then w = 1 */
    model = self->models[i];
    matrix = model->data->rootMatrix;
    matrix[12] = model->pos[0];
    matrix[13] = model->pos[1];
    matrix[14] = model->pos[2];
    matrix[15] = model->pos[3];
    self->models[i]->data->rootMatrix[15] = 1.0f;
  }
  UiCollectionSphereEnablePreview(self, 1);
}
