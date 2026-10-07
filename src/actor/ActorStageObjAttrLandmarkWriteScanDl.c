// bdc 0x088a6788 ActorStageObjAttrLandmarkWriteScanDl
#include "bdc.h"

/* Per-mesh display-list callback of the attribute landmark's hologram pass
   (`ActorStageObjAttrLandmarkDraw` installs it with `GfxModelSetMaterialAnimCallbackByIndex(obj, mesh, cb, obj+0x3bc)`):
   binds the scan texture `g_attrLandmarkScanTex` (`GfxTextureWriteCall`, slot 0), emits the
   fixed state words `0xdf0000a2`, `0xe0000000`, `0xe1ffffff`, then builds a texture matrix:
   identity with the diagonal scale `g_attrLandmarkScanScale` (0, 3, 1) and the translation
   column `g_attrLandmarkScanOffset` (-4 - `scroll[0]`, 0, `scroll[1]`, 0), both statics filled
   lazily on first use, multiplied with a quarter-turn rotation about Y (vrot of `pi/2 * 2/pi`,
   the bank S703). The 4x3 part goes out as `0x40000000` plus 12 TMATRIX data words (top byte of
   `g_actorTexMatrixCmd`, float bits >> 8), followed by `0xcf000000`, the two 1.0f words
   `0x493f8000`/`0x483f8000`, `0xde000002` and `0xc0000001`. Advances `*dl` past everything. */

void ActorStageObjAttrLandmarkWriteScanDl(u32 **dl, const float *scroll)
{
  float mat[4][4];
  float rot[4][4];
  float out[4][4];
  union {
    float f;
    u32 u;
  } bits;
  u32 *p;
  u32 cmd;
  float c;
  float s;
  float sum;
  int col;
  int row;
  int k;

  p = GfxTextureWriteCall(g_attrLandmarkScanTex, *dl, 0);
  *dl = p;
  p[0] = 0xdf0000a2;
  p[1] = 0xe0000000;
  p[2] = 0xe1ffffff;
  *dl = p + 3;

  if (g_attrLandmarkScanOffsetInit == 0) {
    g_attrLandmarkScanOffsetInit = 1;
    g_attrLandmarkScanOffset[1] = 0.0f;
    g_attrLandmarkScanOffset[0] = -4.0f;
    g_attrLandmarkScanOffset[2] = 0.0f;
    g_attrLandmarkScanOffset[3] = 0.0f;
  }
  if (g_attrLandmarkScanScaleInit == 0) {
    g_attrLandmarkScanScaleInit = 1;
    g_attrLandmarkScanScale[0] = 0.0f;
    g_attrLandmarkScanScale[2] = 1.0f;
    g_attrLandmarkScanScale[1] = 3.0f;
    g_attrLandmarkScanScale[3] = 0.0f;
  }
  g_attrLandmarkScanOffset[2] = scroll[1];

  /* mat: identity columns, diagonal scale, translation column. */
  for (col = 0; col < 4; col++) {
    for (row = 0; row < 4; row++) {
      mat[col][row] = col == row ? 1.0f : 0.0f;
    }
  }
  mat[0][0] = g_attrLandmarkScanScale[0];
  mat[1][1] = g_attrLandmarkScanScale[1];
  mat[2][2] = g_attrLandmarkScanScale[2];
  mat[3][0] = g_attrLandmarkScanOffset[0];
  mat[3][1] = g_attrLandmarkScanOffset[1];
  mat[3][2] = g_attrLandmarkScanOffset[2];
  mat[3][3] = g_attrLandmarkScanOffset[3];
  mat[3][0] = mat[3][0] - scroll[0];

  /* rot columns: (c, 0, -s, 0), (0, 1, 0, 0), (s, 0, c, 0), (0, 0, 0, 1) for the angle pi/2. */
  c = __builtin_cosf(VF_PI_2);
  s = __builtin_sinf(VF_PI_2);
  for (col = 0; col < 4; col++) {
    for (row = 0; row < 4; row++) {
      rot[col][row] = 0.0f;
    }
  }
  rot[0][0] = c;
  rot[0][2] = -s;
  rot[1][1] = 1.0f;
  rot[2][0] = s;
  rot[2][2] = c;
  rot[3][3] = 1.0f;

  /* vmmul.q E200, E100, E000: out[col][row] = sum_k rot[k][row] * mat[k][col]. */
  for (col = 0; col < 4; col++) {
    for (row = 0; row < 4; row++) {
      sum = 0.0f;
      for (k = 0; k < 4; k++) {
        sum = sum + rot[k][row] * mat[k][col];
      }
      out[col][row] = sum;
    }
  }

  p = *dl;
  p[0] = 0x40000000;
  cmd = g_actorTexMatrixCmd & 0xff000000;
  for (col = 0; col < 4; col++) {
    for (row = 0; row < 3; row++) {
      bits.f = out[col][row];
      p[1 + col * 3 + row] = cmd | bits.u >> 8;
    }
  }
  *dl = p + 13;
  p[13] = 0xcf000000;
  bits.f = 1.0f;
  (*dl)[1] = bits.u >> 8 | 0x49000000;
  bits.f = 1.0f;
  (*dl)[2] = bits.u >> 8 | 0x48000000;
  (*dl)[3] = 0xde000002;
  (*dl)[4] = 0xc0000001;
  *dl = *dl + 5;
}
