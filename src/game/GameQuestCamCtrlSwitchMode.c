// bdc 0x088fa014 GameQuestCamCtrlSwitchMode
#include "bdc.h"

/* Switches the camera mode of the quest-field camera controller (field camera `+0x5c4`, see
   `GameFieldCameraReset`; tables from `GameQuestParsePathTable`/`GameQuestParseCamTable`) for
   camera id `camId` (the current path node). Does nothing for `camId == -1` or when the current
   camera set of `g_questCamTable` lacks the id (`GameQuestCamTableHasCam`). Otherwise a type-1
   entry (`GameQuestCamTableFindType1`) goes to `GameQuestCamCtrlStartFixedMode`; failing that,
   a type-2 entry (`GameQuestCamTableFindType2`) has its quaternion (`vec10`, +0x10) turned into a
   rotation matrix (product of the quaternion's two 4x4 multiplication matrices, identity row/column 3)
   that rotates the `up` constant of `g_gameQuestCamCtrlAxisConsts`; the result (w = 0) goes to
   `GameQuestCamCtrlStartPointMode`. With neither, `GameQuestCamCtrlStartPathMode` (default mode)
   runs when no mode is set or the current mode's `kind` (+0x70) is nonzero (kind 0 keeps the current
   mode). */

void GameQuestCamCtrlSwitchMode(GameQuestCamCtrl *self, s16 camId)
{
  ScePspFVector4 point __attribute__((aligned(16)));
  ScePspFMatrix4 rot __attribute__((aligned(16)));
  ScePspFVector4 rotated __attribute__((aligned(16)));
  GameQuestCamTable *table;
  GameQuestCamEntry *entry;
  const ScePspFVector4 *up;
  float qx, qy, qz, qw;
  float a[3][4];
  float b[4][4];
  float m[3][3];
  int i, j;

  if (camId == -1) {
    return;
  }
  table = g_questCamTable;
  if (GameQuestCamTableHasCam(table, camId) == 0) {
    return;
  }
  entry = GameQuestCamTableFindType1(table, camId, table->cur);
  if (entry != NULL) {
    GameQuestCamCtrlStartFixedMode(self, entry);
    return;
  }
  entry = GameQuestCamTableFindType2(table, camId, table->cur);
  if (entry != NULL) {
    qx = entry->vec10[0];
    qy = entry->vec10[1];
    qz = entry->vec10[2];
    qw = entry->vec10[3];
    /* left matrix columns (M100) */
    a[0][0] = qw;  a[0][1] = qz;  a[0][2] = -qy; a[0][3] = -qx;
    a[1][0] = -qz; a[1][1] = qw;  a[1][2] = qx;  a[1][3] = -qy;
    a[2][0] = qy;  a[2][1] = -qx; a[2][2] = qw;  a[2][3] = -qz;
    /* right matrix columns (M200) */
    b[0][0] = qw;  b[0][1] = qz;  b[0][2] = -qy; b[0][3] = qx;
    b[1][0] = -qz; b[1][1] = qw;  b[1][2] = qx;  b[1][3] = qy;
    b[2][0] = qy;  b[2][1] = -qx; b[2][2] = qw;  b[2][3] = qz;
    b[3][0] = -qx; b[3][1] = -qy; b[3][2] = -qz; b[3][3] = qw;
    /* column j = M200 * (column j of M100); lane 3 and column 3 become identity */
    for (j = 0; j < 3; j++) {
      for (i = 0; i < 3; i++) {
        m[j][i] = a[j][0] * b[0][i] + a[j][1] * b[1][i] + a[j][2] * b[2][i] + a[j][3] * b[3][i];
      }
    }
    rot.x.x = m[0][0]; rot.x.y = m[0][1]; rot.x.z = m[0][2]; rot.x.w = 0.0f;
    rot.y.x = m[1][0]; rot.y.y = m[1][1]; rot.y.z = m[1][2]; rot.y.w = 0.0f;
    rot.z.x = m[2][0]; rot.z.y = m[2][1]; rot.z.z = m[2][2]; rot.z.w = 0.0f;
    rot.w.x = 0.0f;    rot.w.y = 0.0f;    rot.w.z = 0.0f;    rot.w.w = 1.0f;
    /* rotated = rot (3x3) * up; lane w keeps rot.x.w (0) */
    up = &g_gameQuestCamCtrlAxisConsts.up;
    rotated.x = rot.x.x * up->x + rot.y.x * up->y + rot.z.x * up->z;
    rotated.y = rot.x.y * up->x + rot.y.y * up->y + rot.z.y * up->z;
    rotated.z = rot.x.z * up->x + rot.y.z * up->y + rot.z.z * up->z;
    rotated.w = rot.x.w;
    point.x = rotated.x;
    point.y = rotated.y;
    point.z = rotated.z;
    point.w = rotated.w;
    GameQuestCamCtrlStartPointMode(self, entry, &point.x);
    return;
  }
  if (self->mode == NULL || ((GameQuestCamModeBase *)self->mode)->kind != 0) {
    GameQuestCamCtrlStartPathMode(self);
  }
}
