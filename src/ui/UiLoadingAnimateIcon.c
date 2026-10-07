// bdc 0x0890c0f8 UiLoadingAnimateIcon
#include "bdc.h"

/* Per-frame animation of one icon sprite (`index` 0..5) of the now-loading screen (task 10100 /
   0x2774, `UiLoadingCtor`, update `UiLoadingUpdate`, draw `UiLoadingDraw`; shared objects
   from `UiLoadingInitShared`). The icon reuses sprite fields as animation state:
   `maybe_billboardParams80[0]` = phase, `[1]`/`[2]` = home x/y, `[3]` = frame (cell slot),
   `scaleX`/`scaleY` = fly-in start point, `scaleZ` = curve parameter, `angle` = delay timer.
   Row `index` of `g_loadingIconLayout` supplies base position, fly-in offset, exit offset, the
   Bezier y weight and the cell numbers. Phases: 0 sets the delay `index * 80` (+240 from the fourth
   icon); 1 picks a jittered home position (two VFPU randoms per axis) and the start point; 2 counts
   the delay down; 3 flies along a quadratic Bezier (`UiQuadBezier3`) from the start point to home
   until sin(param) >= 0.995; 4 steps the frame up to 3 (wobbling icons 1 and 3), then phase 10;
   10 bobs vertically for 3*pi of param; 11 steps the frame back down to 0, then phase 20; 20 sets
   the exit target; 21 flies out until the curve ends or the icon leaves (-80..560, -80..352), then
   phase 100; 100 restarts at phase 1 with delay 640. Every phase ends by truncating the position to
   whole pixels and selecting the frame's cell (`UiLoadingIconCell`, `GfxSpriteSetCell`).
   Randoms are PlatformRandFloat12() - 1 (vrndf1 minus the bank's 1); the curve and bob angles are
   radians (vsin of angle * 2/pi). */

static void UiIconCopy4(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

void UiLoadingAnimateIcon(UiLoading *self, GfxSprite *icon, s32 index)
{
  /* [0] phase, [1] home x, [2] home y, [3] frame */
  float *param = icon->maybe_billboardParams80;
  const s16 *row = g_loadingIconLayout[index];
  /* row: [0..1] base x/y, [2..3] fly-in start offset, [4..5] exit offset,
     [6..7] float Bezier y weight, [8..9] u8 cell per frame */
  const u8 *cells = (const u8 *)&row[8];
  float curveY;
  float p0[4];
  float p1[4];
  float ctrl[4];
  float a[4];
  float b[4];
  float c[4];
  float out[4];
  float r0, r1;
  float s, t, dx, dy, x, y, col, cellRow;
  s32 offscreen;

  switch ((s32)param[0]) {
  case 0:
    s = (float)(index * 80);
    if (index >= 3) {
      s = s + 240.0f;
    }
    icon->angle = s;
    param[0] = param[0] + 1.0f;
    /* fallthrough */
  case 1:
    r0 = PlatformRandFloat12() - 1.0f;
    r1 = PlatformRandFloat12() - 1.0f;
    param[1] = ((float)row[0] - r1 * 4.0f) + r0 * 8.0f;
    r0 = PlatformRandFloat12() - 1.0f;
    r1 = PlatformRandFloat12() - 1.0f;
    param[2] = ((float)row[1] - r1 * 4.0f) + r0 * 8.0f;
    s = (float)row[2] + param[1];
    icon->scaleX = s;
    icon->posX = s;
    s = (float)row[3] + param[2];
    icon->scaleY = s;
    icon->posY = s;
    icon->scaleZ = 0.0f;
    param[3] = 0.0f;
    param[0] = param[0] + 1.0f;
    /* fallthrough */
  case 2:
    s = icon->angle + -1.0f;
    icon->angle = s;
    if (!(s <= 0.0f)) {
      break;
    }
    param[0] = param[0] + 1.0f;
    /* fallthrough */
  case 3:
    p0[0] = p0[1] = p0[2] = p0[3] = 0.0f;
    p1[0] = p1[1] = p1[2] = p1[3] = 0.0f;
    ctrl[0] = ctrl[1] = ctrl[2] = ctrl[3] = 0.0f;
    UiIconCopy4(p0, &icon->posX);
    p0[0] = icon->scaleX;
    p0[1] = icon->scaleY;
    UiIconCopy4(p1, &icon->posX);
    p1[0] = param[1];
    p1[1] = param[2];
    icon->scaleZ = icon->scaleZ + 0.0179999992f;
    s = icon->scaleZ;
    if (s < 0.0f) {
      s = 0.0f;
    } else if (!(s <= 1.57079637f)) {
      s = 1.57079637f;
    }
    icon->scaleZ = s;
    t = __builtin_sinf(s);
    UiIconCopy4(ctrl, p1);
    ctrl[0] = p1[0] + (p0[0] - p1[0]) * 0.200000003f;
    __builtin_memcpy(&curveY, &row[6], sizeof curveY);
    ctrl[1] = p1[1] + (p0[1] - p1[1]) * curveY;
    UiIconCopy4(a, p0);
    UiIconCopy4(b, ctrl);
    UiIconCopy4(c, p1);
    UiQuadBezier3(t, self, out, a, b, c);
    UiIconCopy4(&icon->posX, out);
    if (t < 0.995000005f) {
      break;
    }
    UiIconCopy4(&icon->posX, p1);
    icon->angle = 0.0f;
    param[0] = param[0] + 1.0f;
    break;
  case 4:
    dy = 0.0f;
    dx = 0.0f;
    s = icon->scaleZ + 1.0f;
    icon->scaleZ = s;
    if (!(s < 4.0f)) {
      icon->scaleZ = 0.0f;
      param[3] = param[3] + 1.0f;
    }
    if (index == 1 && !(param[3] < 2.0f)) {
      dy = -8.0f;
      dx = 6.0f;
    }
    if (index == 3 && !(param[3] < 3.0f)) {
      dx = 3.0f;
    }
    icon->posX = param[1] + dx;
    icon->posY = param[2] + dy;
    if (param[3] < 3.0f) {
      break;
    }
    param[1] = icon->posX;
    param[2] = icon->posY;
    icon->angle = 0.0f;
    param[0] = 10.0f;
    break;
  case 10:
    icon->scaleZ = icon->scaleZ + 0.0500000007f;
    y = param[2];
    icon->posY = y + __builtin_sinf(icon->scaleZ) * 8.0f;
    if (icon->scaleZ < 9.42477798f) {
      break;
    }
    param[1] = icon->posX;
    param[2] = icon->posY;
    param[0] = param[0] + 1.0f;
    break;
  case 11:
    dy = 0.0f;
    dx = 0.0f;
    s = icon->scaleZ + 1.0f;
    icon->scaleZ = s;
    if (!(s < 4.0f)) {
      icon->scaleZ = 0.0f;
      param[3] = param[3] + -1.0f;
    }
    if (index == 1 && !(param[3] < 2.0f)) {
      dy = -8.0f;
      dx = 6.0f;
    }
    if (index == 3 && !(param[3] < 3.0f)) {
      dx = 4.0f;
    }
    icon->posX = param[1] + dx;
    icon->posY = param[2] + dy;
    if (!(param[3] <= 0.0f)) {
      break;
    }
    param[3] = 0.0f;
    icon->angle = 0.0f;
    param[0] = 20.0f;
    break;
  case 20:
    icon->scaleX = icon->posX;
    icon->scaleY = icon->posY;
    param[1] = icon->scaleX + (float)row[4];
    icon->scaleZ = 0.0f;
    param[2] = icon->scaleY + (float)row[5];
    param[0] = param[0] + 1.0f;
    /* fallthrough */
  case 21:
    p0[0] = p0[1] = p0[2] = p0[3] = 0.0f;
    p1[0] = p1[1] = p1[2] = p1[3] = 0.0f;
    ctrl[0] = ctrl[1] = ctrl[2] = ctrl[3] = 0.0f;
    offscreen = 0;
    UiIconCopy4(p0, &icon->posX);
    p0[0] = icon->scaleX;
    p0[1] = icon->scaleY;
    UiIconCopy4(p1, &icon->posX);
    p1[0] = param[1];
    p1[1] = param[2];
    icon->scaleZ = icon->scaleZ + 0.0149999997f;
    s = icon->scaleZ;
    if (s < 0.0f) {
      s = 0.0f;
    } else if (!(s <= 1.57079637f)) {
      s = 1.57079637f;
    }
    icon->scaleZ = s;
    t = __builtin_sinf(s);
    UiIconCopy4(ctrl, p1);
    ctrl[0] = p1[0] + (p0[0] - p1[0]) * 0.200000003f;
    __builtin_memcpy(&curveY, &row[6], sizeof curveY);
    ctrl[1] = p1[1] + (p0[1] - p1[1]) * curveY;
    UiIconCopy4(a, p0);
    UiIconCopy4(b, ctrl);
    UiIconCopy4(c, p1);
    UiQuadBezier3(t, self, out, a, b, c);
    UiIconCopy4(&icon->posX, out);
    if (icon->posX < -80.0f || !(icon->posX <= 560.0f) || icon->posY < -80.0f ||
        !(icon->posY <= 352.0f)) {
      offscreen = 1;
    }
    if (t < 1.0f && offscreen == 0) {
      break;
    }
    icon->angle = 0.0f;
    param[0] = 100.0f;
    break;
  case 100:
    icon->angle = 640.0f;
    param[0] = 1.0f;
    break;
  default:
    break;
  }

  x = icon->posX;
  y = icon->posY;
  icon->posX = (float)(s32)x;
  icon->posY = (float)(s32)y;
  col = 0.0f;
  cellRow = 0.0f;
  UiLoadingIconCell(self, cells[(s32)param[3]], &col, &cellRow);
  GfxSpriteSetCell(icon, col, cellRow);
}
