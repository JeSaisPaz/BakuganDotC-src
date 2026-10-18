// bdc 0x08990114 UiCollectionFigureResetCursor
#include "bdc.h"

/* Resets the cursor of the figure collection screen (task 314, `maybe_UiScreen314Ctor`): resets
   the shared cursor glow (`UiCursorGlowReset`) and clears `pulse`; only when `cursorMode` is 0 it
   then resets the cursor pulse (`tweens[6]`), shows the cell cursor sprite (sprite 6) centred at
   scale 1, alpha 1, add colour {0.3, 0.3, 0.3, 1}, depth `spriteZ[6]`, on the position of the
   selected cell sprite (`cursor`), starts the highlight pulse on ghost sprite 0x44 (`tweens[68]`),
   resets the six cell sprites (scale 1, depth `spriteZ[i]`, add colour 0.3 on the selected cell and
   0 on the others, `UiCollectionFigureSetCellFrame`) and sprites 25..30 (scale 1, depth), rebuilds
   the root matrix of every loaded cell model as rotY(3.14) * rotX(-0) scaled by `baseScale` (also
   copied to `cellScale[i]`), sets its height `pos[1]` to
   `tintBase + (1 - cos(tintTimer * pi)) * 0.5 * 0.2`, copies `pos` into the translation row with
   w = 1, and finally enables the model spin (`UiCollectionFigureSetModelSpin`). */

void UiCollectionFigureResetCursor(UiCollectionFigure *self)

{
  float rot[16];
  float src[16];
  GfxSprite **sprites;
  GfxModel *model;
  float *matrix;
  float s;
  float c;
  float sn;
  float angle;
  int i;
  int k;
  int l;

  UiCursorGlowReset();
  self->pulse = 0.0f;
  if (self->cursorMode > 0 || self->cursorMode < 0) {
    return;
  }

  UiPulseReset((UiPulse *)&self->tweens[6]);
  sprites = (GfxSprite **)self->base.data;
  sprites[6]->flags |= 1;
  GfxSpriteCenterPivot(sprites[6]);
  sprites = (GfxSprite **)self->base.data;
  UiSpriteSetScaleRotation(sprites[6], 1.0f, 1.0f, 0.0f);
  sprites = (GfxSprite **)self->base.data;
  sprites[6]->alpha = 1.0f;
  sprites[6]->addColor[3] = 1.0f;
  sprites[6]->addColor[0] = 0.3f;
  sprites[6]->addColor[1] = 0.3f;
  sprites[6]->addColor[2] = 0.3f;
  sprites[6]->posZ = self->spriteZ[6];
  sprites[6]->posX = sprites[self->cursor]->posX;
  sprites[6]->posY = sprites[self->cursor]->posY;
  UiPulseInit(sprites[6], sprites[68], (UiPulse *)&self->tweens[68]);

  /* the six grid cells */
  for (i = 0; i < 6; i++) {
    sprites = (GfxSprite **)self->base.data;
    UiSpriteSetScaleRotation(sprites[i], 1.0f, 1.0f, 0.0f);
    sprites = (GfxSprite **)self->base.data;
    sprites[i]->posZ = self->spriteZ[i];
    if (i == self->cursor) {
      sprites[i]->addColor[0] = 0.3f;
      sprites[i]->addColor[1] = 0.3f;
      sprites[i]->addColor[2] = 0.3f;
      sprites[i]->addColor[3] = 1.0f;
      UiCollectionFigureSetCellFrame(self, sprites[i], true);
    } else {
      sprites[i]->addColor[0] = 0.0f;
      sprites[i]->addColor[1] = 0.0f;
      sprites[i]->addColor[2] = 0.0f;
      sprites[i]->addColor[3] = 1.0f;
      UiCollectionFigureSetCellFrame(self, sprites[i], false);
    }
  }

  for (i = 25; i < 31; i++) {
    sprites = (GfxSprite **)self->base.data;
    UiSpriteSetScaleRotation(sprites[i], 1.0f, 1.0f, 0.0f);
    sprites = (GfxSprite **)self->base.data;
    sprites[i]->posZ = self->spriteZ[i];
  }

  for (i = 0; i < 6; i++) {
    if (self->models[i] == NULL) {
      continue;
    }

    /* rootMatrix = rotation about Y by 3.14 */
    angle = 3.14f;
    c = VfCosQuarter(VfMul(angle, 0.636619747f));
    sn = VfSinQuarter(VfMul(angle, 0.636619747f));
    matrix = self->models[i]->data->rootMatrix;
    matrix[0] = c;
    matrix[1] = 0.0f;
    matrix[2] = -sn;
    matrix[3] = 0.0f;
    matrix[4] = 1.0f;
    matrix[5] = 0.0f;
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

    /* rootMatrix = rootMatrix * rotation about X by -0 (vmmul.q E200, E100, E000, row-major):
       element [k][l] of the result = sum over j of rootMatrix[k][j] * rot[j][l] */
    angle = -0.0f;
    c = VfCosQuarter(VfMul(angle, 0.636619747f));
    sn = VfSinQuarter(VfMul(angle, 0.636619747f));
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
    matrix = self->models[i]->data->rootMatrix;
    for (k = 0; k < 16; k++) {
      src[k] = matrix[k];
    }
    for (k = 0; k < 4; k++) {
      for (l = 0; l < 4; l++) {
        matrix[k * 4 + l] = VfDot4(src[k * 4 + 0], src[k * 4 + 1], src[k * 4 + 2], src[k * 4 + 3], rot[0 * 4 + l], rot[1 * 4 + l], rot[2 * 4 + l], rot[3 * 4 + l]);
      }
    }

    /* scale the three axis rows by baseScale */
    s = self->baseScale;
    self->cellScale[i] = s;
    matrix = self->models[i]->data->rootMatrix;
    for (k = 0; k < 12; k++) {
      matrix[k] = VfMul(matrix[k], s);
    }

    /* height = tintBase + (1 - cos(tintTimer * pi)) * 0.5 * 0.2 */
    s = self->tintBase;
    c = VfCosQuarter(VfMul(self->tintTimer * 3.1415927f, 0.636619747f));
    self->models[i]->pos[1] = s + (1.0f - c) * 0.5f * 0.2f;

    /* translation row = pos (all four words), w = 1 */
    model = self->models[i];
    model->data->rootMatrix[12] = model->pos[0];
    model->data->rootMatrix[13] = model->pos[1];
    model->data->rootMatrix[14] = model->pos[2];
    model->data->rootMatrix[15] = model->pos[3];
    self->models[i]->data->rootMatrix[15] = 1.0f;
  }

  UiCollectionFigureSetModelSpin(self, 1);
  return;
}
