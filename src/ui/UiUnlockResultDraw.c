// bdc 0x08938380 UiUnlockResultDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the unlock result screen (task id 375). When the sprite layer exists:
   the background (`UiScreenDrawBg`) and sprite layer mask 1 in a render packet at z 2000. Then
   the name and help texts (`UiUnlockResultDrawNameText`, `UiUnlockResultDrawHelpText`).
   When `model` exists: a packet at z 1900 with the light state of `camera`, a depth-clear pass of
   32 through-mode sprite vertices over the screen, fog from the alpha of `g_colorWhite`; the
   model's root matrix (column-major) is rebuilt as tilt(quat +Y -> `g_vecUp`) * rotZ * rotX *
   rotY * scale with each angle `pi/2 - rot`, wrapped into (-pi, pi], translation column = `pos`
   with w = 1; the motion is stepped and applied (`GfxModelUpdateMotion`,
   `GfxModelApplyMotion`) and the model's draw method called. After the chunk is closed, a
   non-NULL `savedCamera` becomes `g_gfxActiveCamera`. */

/* Wraps pi/2 - angle into (-pi, pi]. */
static float UiUnlockResultWrapAngle(float a)
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

/* Column-major 4x4 product out = a * b (vmmul): column j of out = sum over k of b[j][k] times
   column k of a. Both inputs are read before out is written, so out may alias either. */
static void UiUnlockResultMatMul(float *out, const float *a, const float *b)
{
  float tmp[16];
  s32 j;
  s32 r;

  for (j = 0; j < 4; j++) {
    for (r = 0; r < 4; r++) {
      tmp[j * 4 + r] = b[j * 4 + 0] * a[0 * 4 + r] + b[j * 4 + 1] * a[1 * 4 + r] +
                       b[j * 4 + 2] * a[2 * 4 + r] + b[j * 4 + 3] * a[3 * 4 + r];
    }
  }
  for (r = 0; r < 16; r++) {
    out[r] = tmp[r];
  }
}

/* One byte of the fog colour: vsat0, scale by 255, vf2iz 23, vi2uc. */
static u32 UiUnlockResultColorByte(float x)
{
  return VfI2uc(VfF2iz(VfSat0(x) * 255.0f, 23));
}

void UiUnlockResultDraw(UiUnlockResult *self)
{
  RenderPacket *packet;
  GfxModel *model;
  const VtblEntry *draw;
  u32 *list;
  u32 *dl;
  u32 *after;
  GfxGeColorVertex16 *verts;
  GfxGeColorVertex16 *v;
  union { float f; u32 u; } fog1, fog2;
  float quat[4];
  float tilt[16];
  float left[16];
  float right[16];
  float rot[16];
  float *q;
  float *root;
  float alpha;
  float far;
  float angle;
  float c;
  float s;
  u32 color;
  u32 packed;
  s32 i;

  if (self->base.spriteLayer != NULL) {
    UiScreenDrawBg(&self->base);
    packet = GfxNewRenderPacket(2000.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  UiUnlockResultDrawNameText(self);
  UiUnlockResultDrawHelpText(self);
  if (self->model == NULL) {
    return;
  }

  packet = GfxNewRenderPacket(1900.0f);
  list = GfxPacketBeginChunk(packet);
  list = GfxDlWriteLightState(list, (GfxCamera *)self->camera, 1);

  /* Depth clear: jump over the inline vertex data, then draw it as sprites. */
  dl = list;
  verts = (GfxGeColorVertex16 *)(dl + 2);
  after = (u32 *)((u8 *)verts + 32 * sizeof(GfxGeColorVertex16));
  dl[0] = ((PspAddr(after) >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
  dl[1] = (PspAddr(after) & 0xffffff) | 0x08000000;           /* JUMP */
  v = verts;
  for (i = 0; i < 32; i++) {
    v->x = (s16)((i / 2 + i % 2) * 32);
    v->y = (s16)((i % 2) * 272);
    v->z = 0;
    v++;
  }
  after[0] = 0xd3000401; /* CLEAR on (depth) */
  after[1] = 0x1280011c; /* VTYPE through, 8888, s16 */
  after += 2;
  if (verts != NULL) {
    after[0] = ((PspAddr(verts) >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
    after[1] = (PspAddr(verts) & 0xffffff) | 0x01000000;          /* VADDR */
    after += 2;
  }
  after[0] = 0x04060020; /* PRIM sprites x32 */
  after[1] = 0xd3000000; /* CLEAR off */
  dl = after + 2;

  /* Fog from the alpha of g_colorWhite. */
  alpha = g_colorWhite.w;
  if (!(alpha <= 1.0f)) {
    alpha = 1.0f;
  }
  if (alpha <= 0.0001f) {
    color = 0xcf000000;
    fog1.f = 10000.0f;
    fog2.f = 10000.0f;
  } else {
    far = 1.0f + (1.0f - alpha) * 800.0f;
    fog1.f = far + 20000.0f;
    if (!(far <= 0.0f)) {
      fog2.f = 1.0f / far;
    } else if (far < 0.0f) {
      fog2.f = 1.0f / far;
    } else {
      fog2.f = 0.0f;
    }
    packed = UiUnlockResultColorByte(g_colorWhite.x) | UiUnlockResultColorByte(g_colorWhite.y) << 8 |
             UiUnlockResultColorByte(g_colorWhite.z) << 16 |
             UiUnlockResultColorByte(g_colorWhite.w) << 24;
    color = (packed & 0xffffff) | 0xcf000000;
  }
  dl[0] = color;                      /* FOGCOLOR */
  dl[1] = (fog1.u >> 8) | 0xcd000000; /* FOG1 */
  dl[2] = (fog2.u >> 8) | 0xce000000; /* FOG2 */
  list = dl + 3;

  /* Quaternion -> rotation matrix as the product of its left and right multiplication matrices
     (columns as below); row 3 and column 3 are reset to identity. */
  q = MathQuatFromUpToDir(quat, (const float *)&g_vecUp);
  left[0] = q[3];   left[1] = q[2];   left[2] = -q[1];  left[3] = -q[0];
  left[4] = -q[2];  left[5] = q[3];   left[6] = q[0];   left[7] = -q[1];
  left[8] = q[1];   left[9] = -q[0];  left[10] = q[3];  left[11] = -q[2];
  left[12] = q[0];  left[13] = q[1];  left[14] = q[2];  left[15] = q[3];
  right[0] = q[3];  right[1] = q[2];  right[2] = -q[1]; right[3] = q[0];
  right[4] = -q[2]; right[5] = q[3];  right[6] = q[0];  right[7] = q[1];
  right[8] = q[1];  right[9] = -q[0]; right[10] = q[3]; right[11] = q[2];
  right[12] = -q[0]; right[13] = -q[1]; right[14] = -q[2]; right[15] = q[3];
  UiUnlockResultMatMul(tilt, left, right);
  tilt[3] = 0.0f;
  tilt[7] = 0.0f;
  tilt[11] = 0.0f;
  tilt[12] = 0.0f;
  tilt[13] = 0.0f;
  tilt[14] = 0.0f;
  tilt[15] = 1.0f;

  /* rootMatrix = rotY(pi/2 - rot.y) * scale */
  model = (GfxModel *)self->model;
  root = model->data->rootMatrix;
  angle = UiUnlockResultWrapAngle(1.57079637f - model->rot[1]);
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

  /* rootMatrix = rotX(pi/2 - rot.x) * rootMatrix */
  model = (GfxModel *)self->model;
  root = model->data->rootMatrix;
  angle = UiUnlockResultWrapAngle(1.57079637f - model->rot[0]);
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
  UiUnlockResultMatMul(root, rot, root);

  /* rootMatrix = rotZ(pi/2 - rot.z) * rootMatrix */
  model = (GfxModel *)self->model;
  root = model->data->rootMatrix;
  angle = UiUnlockResultWrapAngle(1.57079637f - model->rot[2]);
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
  UiUnlockResultMatMul(root, rot, root);

  /* rootMatrix = tilt * rootMatrix */
  model = (GfxModel *)self->model;
  root = model->data->rootMatrix;
  UiUnlockResultMatMul(root, tilt, root);

  /* Translation column = pos, w = 1. */
  model = (GfxModel *)self->model;
  root = model->data->rootMatrix;
  root[12] = model->pos[0];
  root[13] = model->pos[1];
  root[14] = model->pos[2];
  root[15] = model->pos[3];
  ((GfxModel *)self->model)->data->rootMatrix[15] = 1.0f;

  GfxModelUpdateMotion((GfxModel *)self->model);
  GfxModelApplyMotion((GfxModel *)self->model);
  model = (GfxModel *)self->model;
  draw = &((const VtblEntry *)model->base.vtable)[8];
  ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, &list);
  GfxPacketEndChunk(packet, list);
  if (self->savedCamera != NULL) {
    g_gfxActiveCamera = self->savedCamera;
  }
}
