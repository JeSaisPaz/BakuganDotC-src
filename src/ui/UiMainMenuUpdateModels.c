// bdc 0x089a4070 UiMainMenuUpdateModels
#include "bdc.h"

/* Per-frame model update of the main menu (from `UiMainMenuUpdate`), for each non-NULL model of
   `models[0..4]`:
   - model 0: scrolls the line/sea materials (`UiMainMenuScrollLine`, `UiMainMenuScrollSea`),
     then spins its GMO root matrix about Y by 0.02 rad (each row r becomes
     (c*x + s*z, y, c*z - s*x, w)), re-orthonormalises the three axes (z = x cross y, x = y cross z,
     each scaled by 1/sqrt of its squared length; rows 0 and 2 get w = 0, row 1 keeps its w), scales
     the three axis rows (all four lanes) by 0.5 and restores the translation row; no virtual update;
   - models 1..3: the model's virtual update (vtable `+0x38/+0x3c`);
   - model 4 (item box): when the cursor differs from `modelCursor`, plays the open motion
     (`UiItemBoxGetMotionName` 0) when the cursor moved onto item 4 or the close motion (1) when it
     left item 4 (`GfxModelEnableMotion`, `GfxModelPlayMotionByName` at frame 0.2, no loop),
     sets motion speed 1.0 (vtable `+0x30/+0x34`) and copies the cursor into `modelCursor`; then
     the virtual update.
   The rotation was `vrot` of 0.02 times the bank's 2/pi (S703), i.e. `cosf`/`sinf` of 0.02. */

void UiMainMenuUpdateModels(UiMainMenu *self)
{
  char name[64];
  float savedPos[4];
  float ax[3], ay[3], az[3];
  float c, sn, x, y, z, w, k;
  GfxModel *model;
  const GfxModelVtable *vt;
  float *m;
  int i;
  int r;

  for (i = 0; i < 5; i++) {
    if (self->models[i] == NULL) {
      continue;
    }
    if (i == 1 || i == 2 || i == 3) {
      model = (GfxModel *)self->models[i];
      vt = (const GfxModelVtable *)model->base.vtable;
      vt->update((char *)model + vt->updateAdjust);
    } else if (i == 4) {
      if (self->cursor != self->modelCursor) {
        if (self->cursor == 4) {
          UiItemBoxGetMotionName(0, name);
          GfxModelEnableMotion((GfxModel *)self->models[i]);
          GfxModelPlayMotionByName(0.2f, (GfxModel *)self->models[i], name, false);
          model = (GfxModel *)self->models[i];
          vt = (const GfxModelVtable *)model->base.vtable;
          vt->setMotionSpeed((char *)model + vt->setMotionSpeedAdjust, 1.0f);
        } else if (self->modelCursor == 4) {
          UiItemBoxGetMotionName(1, name);
          GfxModelEnableMotion((GfxModel *)self->models[i]);
          GfxModelPlayMotionByName(0.2f, (GfxModel *)self->models[i], name, false);
          model = (GfxModel *)self->models[i];
          vt = (const GfxModelVtable *)model->base.vtable;
          vt->setMotionSpeed((char *)model + vt->setMotionSpeedAdjust, 1.0f);
        }
        self->modelCursor = self->cursor;
      }
      model = (GfxModel *)self->models[i];
      vt = (const GfxModelVtable *)model->base.vtable;
      vt->update((char *)model + vt->updateAdjust);
    } else {
      UiMainMenuScrollLine(self);
      UiMainMenuScrollSea(self);
      m = ((GfxModel *)self->models[i])->data->rootMatrix;
      savedPos[0] = m[12];
      savedPos[1] = m[13];
      savedPos[2] = m[14];
      savedPos[3] = m[15];
      /* rotate every row about Y by 0.02 rad (vmmul.q E200,E100,E000); row 3 is restored below */
      c = __builtin_cosf(0.02f); /* 0x3ca3d70a */
      sn = __builtin_sinf(0.02f);
      for (r = 0; r < 4; r++) {
        x = m[r * 4 + 0];
        y = m[r * 4 + 1];
        z = m[r * 4 + 2];
        w = m[r * 4 + 3];
        m[r * 4 + 0] = x * c + y * 0.0f + z * sn + w * 0.0f;
        m[r * 4 + 1] = x * 0.0f + y * 1.0f + z * 0.0f + w * 0.0f;
        m[r * 4 + 2] = x * -sn + y * 0.0f + z * c + w * 0.0f;
        m[r * 4 + 3] = x * 0.0f + y * 0.0f + z * 0.0f + w * 1.0f;
      }
      /* re-orthonormalise: z = row0 x row1, x = row1 x z, y = row1 */
      ay[0] = m[4];
      ay[1] = m[5];
      ay[2] = m[6];
      az[0] = m[1] * ay[2] - m[2] * ay[1];
      az[1] = m[2] * ay[0] - m[0] * ay[2];
      az[2] = m[0] * ay[1] - m[1] * ay[0];
      ax[0] = ay[1] * az[2] - ay[2] * az[1];
      ax[1] = ay[2] * az[0] - ay[0] * az[2];
      ax[2] = ay[0] * az[1] - ay[1] * az[0];
      k = VfRsq(ax[0] * ax[0] + ax[1] * ax[1] + ax[2] * ax[2]);
      m[0] = ax[0] * k;
      m[1] = ax[1] * k;
      m[2] = ax[2] * k;
      m[3] = 0.0f;
      k = VfRsq(ay[0] * ay[0] + ay[1] * ay[1] + ay[2] * ay[2]);
      m[4] = ay[0] * k;
      m[5] = ay[1] * k;
      m[6] = ay[2] * k;
      k = VfRsq(az[0] * az[0] + az[1] * az[1] + az[2] * az[2]);
      m[8] = az[0] * k;
      m[9] = az[1] * k;
      m[10] = az[2] * k;
      m[11] = 0.0f;
      /* scale the three axis rows (all four lanes) by 0.5 */
      for (r = 0; r < 12; r++) {
        m[r] = m[r] * 0.5f;
      }
      /* restore the translation row */
      m[12] = savedPos[0];
      m[13] = savedPos[1];
      m[14] = savedPos[2];
      m[15] = savedPos[3];
    }
  }
}
