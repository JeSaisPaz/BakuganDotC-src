// bdc 0x089aa8fc UiMainMenuStepCarousel
#include "bdc.h"

/* Steps the carousel rotation started by `UiMainMenuStartCarouselMove`. Holding left (pad
   `repeat` 0x80, moveDir 0) or right (0x20, moveDir 1) in the move direction sets `moveSpeed` to 4;
   pressing it again also sets `moveQueued`. `moveT` advances by 1/moveSpeed; with u = moveT - 1 and
   e = 1 - u^2 it slides the old and new item sprites (`data[5+item]`) to slideStart +/- e*slideDelta,
   turns the base model to baseStartAngle +/- e*1.256 (`UiWrapAngle`) and rebuilds `baseMatrix` as
   rotX(-pi/2) * rotY(baseAngle) keeping its translation, and moves each item model around its
   `itemHome` on a radius-70 orbit (`UiOrbitPoint`) at startAngle -/+ e*1.256, copying the
   position into the model's root matrix. While moveT < 1 returns 0. Otherwise snaps the sprites to
   slideEnd, the base to baseTargetAngle and the items to targetAngle, stores each item's carousel
   distance to the cursor (`UiMainMenuItemDistance`) in `from.slot`, sets the frame sprite
   `data[1]` alpha (and `frameAlpha`) to 1 and returns 1. */

/* Rebuilds m (row-major 4x4) as rotX(-pi/2) * rotY(yaw), keeping its translation row. */
static inline __attribute__((always_inline)) void UiMainMenuStepCarouselSetBaseMatrix(float *m, float yaw)
{
  float saved[4];
  float a[4][4];
  float r[4][4];
  float cp, sp, cy, sy;
  int row, col, k;

  for (k = 0; k < 4; k++)
    saved[k] = m[12 + k];

  cp = __builtin_cosf(-1.5707964f);
  sp = __builtin_sinf(-1.5707964f);
  m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f;
  m[4] = 0.0f; m[5] = cp;   m[6] = sp;   m[7] = 0.0f;
  m[8] = 0.0f; m[9] = -sp;  m[10] = cp;  m[11] = 0.0f;
  m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;

  cy = __builtin_cosf(yaw);
  sy = __builtin_sinf(yaw);
  r[0][0] = cy;   r[0][1] = 0.0f; r[0][2] = -sy;  r[0][3] = 0.0f;
  r[1][0] = 0.0f; r[1][1] = 1.0f; r[1][2] = 0.0f; r[1][3] = 0.0f;
  r[2][0] = sy;   r[2][1] = 0.0f; r[2][2] = cy;   r[2][3] = 0.0f;
  r[3][0] = 0.0f; r[3][1] = 0.0f; r[3][2] = 0.0f; r[3][3] = 1.0f;

  for (row = 0; row < 4; row++)
    for (col = 0; col < 4; col++)
      a[row][col] = m[row * 4 + col];
  for (row = 0; row < 4; row++)
    for (col = 0; col < 4; col++)
      m[row * 4 + col] = a[row][0] * r[0][col] + a[row][1] * r[1][col] +
                         a[row][2] * r[2][col] + a[row][3] * r[3][col];

  for (k = 0; k < 4; k++)
    m[12 + k] = saved[k];
}

int UiMainMenuStepCarousel(UiMainMenu *self)
{
  float pt[2];
  UiTween *tween;
  GfxModel *model;
  float u;
  float angle;
  s8 item;
  int i;
  int k;

  UiFlashStep(0);

  if ((self->base.pad->repeat & 0x80) != 0) {
    if (self->moveDir == 0)
      self->moveSpeed = 4.0f;
  } else if ((self->base.pad->repeat & 0x20) != 0) {
    if (self->moveDir == 1)
      self->moveSpeed = 4.0f;
  }
  if ((self->base.pad->pressed & 0x80) != 0) {
    if (self->moveDir == 0) {
      self->moveSpeed = 4.0f;
      self->moveQueued = 1;
    }
  } else if ((self->base.pad->pressed & 0x20) != 0) {
    if (self->moveDir == 1) {
      self->moveQueued = 1;
      self->moveSpeed = 4.0f;
    }
  }
  self->moveT = self->moveT + 1.0f / self->moveSpeed;

  if (self->moveDir == 0) {
    item = self->prevCursor;
    tween = &self->slots[5 + item].tween;
    u = self->moveT - 1.0f;
    ((GfxSprite **)self->base.data)[5 + item]->posX =
        (float)tween->slideStart + (1.0f - u * u) * (float)tween->slideDelta;
    item = self->cursor;
    tween = &self->slots[5 + item].tween;
    u = self->moveT - 1.0f;
    ((GfxSprite **)self->base.data)[5 + item]->posX =
        (float)tween->slideStart + (1.0f - u * u) * (float)tween->slideDelta;
  } else {
    item = self->prevCursor;
    tween = &self->slots[5 + item].tween;
    u = self->moveT - 1.0f;
    ((GfxSprite **)self->base.data)[5 + item]->posX =
        (float)tween->slideStart - (1.0f - u * u) * (float)tween->slideDelta;
    item = self->cursor;
    tween = &self->slots[5 + item].tween;
    u = self->moveT - 1.0f;
    ((GfxSprite **)self->base.data)[5 + item]->posX =
        (float)tween->slideStart - (1.0f - u * u) * (float)tween->slideDelta;
  }

  u = self->moveT - 1.0f;
  if (self->moveDir == 0)
    self->baseAngle = UiWrapAngle(self->baseStartAngle + (1.0f - u * u) * 1.256f);
  else
    self->baseAngle = UiWrapAngle(self->baseStartAngle - (1.0f - u * u) * 1.256f);
  UiMainMenuStepCarouselSetBaseMatrix(self->baseMatrix, self->baseAngle);

  for (i = 0; i < 5; i++) {
    if (self->models[i] == NULL)
      continue;
    u = self->moveT - 1.0f;
    if (self->moveDir == 0)
      angle = UiWrapAngle(self->items[i].from.startAngle - (1.0f - u * u) * 1.256f);
    else
      angle = UiWrapAngle(self->items[i].from.startAngle + (1.0f - u * u) * 1.256f);
    self->items[i].angle = angle;
    UiOrbitPoint(angle, self->itemHome[i][0], self->itemHome[i][2], 0.0f, 0.0f, pt, 0x46);
    ((GfxModel *)self->models[i])->pos[0] = pt[0];
    ((GfxModel *)self->models[i])->pos[2] = pt[1];
    model = (GfxModel *)self->models[i];
    for (k = 0; k < 4; k++)
      model->data->rootMatrix[12 + k] = model->pos[k];
    ((GfxModel *)self->models[i])->data->rootMatrix[15] = 1.0f;
  }

  if (self->moveT < 1.0f)
    return 0;

  item = self->prevCursor;
  ((GfxSprite **)self->base.data)[5 + item]->posX = (float)self->slots[5 + item].tween.slideEnd;
  item = self->cursor;
  ((GfxSprite **)self->base.data)[5 + item]->posX = (float)self->slots[5 + item].tween.slideEnd;
  self->baseAngle = self->baseTargetAngle;
  UiMainMenuStepCarouselSetBaseMatrix(self->baseMatrix, self->baseAngle);

  for (i = 0; i < 5; i++) {
    if (self->models[i] != NULL) {
      self->items[i].angle = self->items[i].targetAngle;
      self->items[i].from.slot = UiMainMenuItemDistance(self, (u8)i, (u8)self->cursor);
    }
  }

  ((GfxSprite **)self->base.data)[1]->alpha = 1.0f;
  self->frameAlpha = ((GfxSprite **)self->base.data)[1]->alpha;
  return 1;
}
