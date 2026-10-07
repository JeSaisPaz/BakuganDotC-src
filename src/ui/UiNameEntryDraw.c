// bdc 0x08804b70 UiNameEntryDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the player name entry screen (task id 3000). When the sprite layer
   exists it is drawn in three passes (layer masks 1, 2, 4 in render packets at z 50, 20, 60).
   The key-grid and name text boxes, when present, get `g_colorWhite` as outline colour and are
   drawn in a packet at z 60 each. A present camera is fully updated (`GfxCameraUpdate`, flags
   -1). When the avatar exists its root matrix (row-major, row vectors) is rebuilt as
   scale * rotY(pi/2 - rot.y) * rotX(pi/2 - rot.x) * rotZ(pi/2 - rot.z) * tilt, with tilt the
   rotation of the quaternion turning +Y onto `g_vecUp`, angles wrapped into (-pi, pi], and the
   translation row = `pos` with w = 1; the motion is stepped and applied (`GfxModelUpdateMotion`,
   `GfxModelApplyMotion`) and the model drawn in a packet at z 40 with the light state of
   `g_gfxActiveCamera` and the fog of `g_gfxFogParams`. The pedestal `baseModel`, when present,
   is drawn the same way at z 30. */

/* Wraps an angle into (-pi, pi]. */
static float UiNameEntryWrapAngle(float a)
{
  if (a <= 3.14159274f) {
    if (a <= -3.14159274f) {
      a = a + 6.28318548f;
    }
  } else {
    a = a - 6.28318548f;
  }
  return a;
}

/* out = a * b for row-major 4x4 matrices; `out` may be `a` or `b`. */
static void UiNameEntryMatMul(float *out, const float *a, const float *b)
{
  float tmp[16];
  int row;
  int col;

  for (row = 0; row < 4; row++) {
    for (col = 0; col < 4; col++) {
      tmp[row * 4 + col] = a[row * 4 + 0] * b[0 * 4 + col] + a[row * 4 + 1] * b[1 * 4 + col] +
                           a[row * 4 + 2] * b[2 * 4 + col] + a[row * 4 + 3] * b[3 * 4 + col];
    }
  }
  for (row = 0; row < 16; row++) {
    out[row] = tmp[row];
  }
}

/* GE command: the float's raw bits shifted right by 8 (24-bit float) under `cmd`. */
static u32 UiNameEntryGeFloat24(u32 cmd, float value)
{
  union {
    float f;
    u32 u;
  } bits;

  bits.f = value;
  return (bits.u >> 8) | cmd;
}

/* Opens a chunk on a new packet at `sortKey`, writes light state and fog, calls the model's draw
   method (vtable entry 8) and closes the chunk. */
static void UiNameEntryDrawModel(GfxModel **slot, float sortKey)
{
  RenderPacket *packet;
  BtlArenaFog *fog;
  GfxModel *model;
  const VtblEntry *draw;
  u32 *list;

  packet = GfxNewRenderPacket(sortKey);
  list = GfxPacketBeginChunk(packet);
  list = GfxDlWriteLightState(list, g_gfxActiveCamera, 1);
  fog = g_gfxFogParams;
  list[0] = (fog->color & 0xffffff) | 0xcf000000;
  list[1] = UiNameEntryGeFloat24(0xcd000000, fog->range);
  list[2] = UiNameEntryGeFloat24(0xce000000, fog->scale);
  list += 3;
  model = *slot;
  draw = &((const VtblEntry *)model->base.vtable)[8];
  ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, &list);
  GfxPacketEndChunk(packet, list);
}

void UiNameEntryDraw(UiNameEntry *self)
{
  RenderPacket *packet;
  UiTextPrinter *text;
  GfxModel *model;
  float quat[4];
  float tilt[16];
  float rot[16];
  float left[4][4];
  float right[4][4];
  float *q;
  float *root;
  float angle;
  float c;
  float s;
  int i;
  int k;

  if (self->base.spriteLayer != NULL) {
    packet = GfxNewRenderPacket(50.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(20.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 2);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(60.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 4);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  if (self->keyText != NULL) {
    text = (UiTextPrinter *)self->keyText;
    text->outlineColor[0] = g_colorWhite.x;
    text->outlineColor[1] = g_colorWhite.y;
    text->outlineColor[2] = g_colorWhite.z;
    text->outlineColor[3] = g_colorWhite.w;
    text = (UiTextPrinter *)self->keyText;
    GfxSpriteLayerDraw(&text->layer, GfxNewRenderPacket(60.0f));
  }
  if (self->nameText != NULL) {
    text = (UiTextPrinter *)self->nameText;
    text->outlineColor[0] = g_colorWhite.x;
    text->outlineColor[1] = g_colorWhite.y;
    text->outlineColor[2] = g_colorWhite.z;
    text->outlineColor[3] = g_colorWhite.w;
    text = (UiTextPrinter *)self->nameText;
    GfxSpriteLayerDraw(&text->layer, GfxNewRenderPacket(60.0f));
  }
  if (self->camera != NULL) {
    GfxCameraUpdate((GfxCamera *)self->camera, 0xffffffff);
  }

  if (self->avatar != NULL) {
    q = MathQuatFromUpToDir(quat, (const float *)&g_vecUp);
    /* Quaternion -> rotation matrix as the product of its left and right multiplication
       matrices (rows of `left`/`right` are the asm's columns); the w row/column is reset to
       identity. */
    left[0][0] = q[3];  left[0][1] = q[2];  left[0][2] = -q[1]; left[0][3] = -q[0];
    left[1][0] = -q[2]; left[1][1] = q[3];  left[1][2] = q[0];  left[1][3] = -q[1];
    left[2][0] = q[1];  left[2][1] = -q[0]; left[2][2] = q[3];  left[2][3] = -q[2];
    left[3][0] = q[0];  left[3][1] = q[1];  left[3][2] = q[2];  left[3][3] = q[3];
    right[0][0] = q[3];  right[0][1] = q[2];  right[0][2] = -q[1]; right[0][3] = q[0];
    right[1][0] = -q[2]; right[1][1] = q[3];  right[1][2] = q[0];  right[1][3] = q[1];
    right[2][0] = q[1];  right[2][1] = -q[0]; right[2][2] = q[3];  right[2][3] = q[2];
    right[3][0] = -q[0]; right[3][1] = -q[1]; right[3][2] = -q[2]; right[3][3] = q[3];
    for (i = 0; i < 3; i++) {
      for (k = 0; k < 3; k++) {
        tilt[i * 4 + k] = left[0][k] * right[i][0] + left[1][k] * right[i][1] +
                          left[2][k] * right[i][2] + left[3][k] * right[i][3];
      }
      tilt[i * 4 + 3] = 0.0f;
    }
    tilt[12] = 0.0f;
    tilt[13] = 0.0f;
    tilt[14] = 0.0f;
    tilt[15] = 1.0f;

    /* rootMatrix = scale * rotY(pi/2 - rot.y) */
    model = (GfxModel *)self->avatar;
    root = model->data->rootMatrix;
    angle = UiNameEntryWrapAngle(1.57079637f - model->rot[1]);
    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    root[0] = c * model->scale[0];
    root[1] = 0.0f;
    root[2] = -s * model->scale[0];
    root[3] = 0.0f;
    root[4] = 0.0f;
    root[5] = model->scale[1];
    root[6] = 0.0f;
    root[7] = 0.0f;
    root[8] = s * model->scale[2];
    root[9] = 0.0f;
    root[10] = c * model->scale[2];
    root[11] = 0.0f;
    root[12] = 0.0f;
    root[13] = 0.0f;
    root[14] = 0.0f;
    root[15] = 1.0f;

    /* rootMatrix = rootMatrix * rotX(pi/2 - rot.x) */
    model = (GfxModel *)self->avatar;
    root = model->data->rootMatrix;
    angle = UiNameEntryWrapAngle(1.57079637f - model->rot[0]);
    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    for (i = 0; i < 16; i++) {
      rot[i] = 0.0f;
    }
    rot[0] = 1.0f;
    rot[5] = c;
    rot[6] = s;
    rot[9] = -s;
    rot[10] = c;
    rot[15] = 1.0f;
    UiNameEntryMatMul(root, root, rot);

    /* rootMatrix = rootMatrix * rotZ(pi/2 - rot.z) */
    model = (GfxModel *)self->avatar;
    root = model->data->rootMatrix;
    angle = UiNameEntryWrapAngle(1.57079637f - model->rot[2]);
    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    for (i = 0; i < 16; i++) {
      rot[i] = 0.0f;
    }
    rot[0] = c;
    rot[1] = s;
    rot[4] = -s;
    rot[5] = c;
    rot[10] = 1.0f;
    rot[15] = 1.0f;
    UiNameEntryMatMul(root, root, rot);

    /* rootMatrix = rootMatrix * tilt */
    model = (GfxModel *)self->avatar;
    root = model->data->rootMatrix;
    UiNameEntryMatMul(root, root, tilt);

    /* Translation row = pos, w = 1. */
    model = (GfxModel *)self->avatar;
    root = model->data->rootMatrix;
    root[12] = model->pos[0];
    root[13] = model->pos[1];
    root[14] = model->pos[2];
    root[15] = model->pos[3];
    ((GfxModel *)self->avatar)->data->rootMatrix[15] = 1.0f;

    GfxModelUpdateMotion((GfxModel *)self->avatar);
    GfxModelApplyMotion((GfxModel *)self->avatar);
    UiNameEntryDrawModel((GfxModel **)&self->avatar, 40.0f);
  }
  if (self->baseModel != NULL) {
    UiNameEntryDrawModel((GfxModel **)&self->baseModel, 30.0f);
  }
}
