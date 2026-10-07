// bdc 0x088f779c GameQuestCamUpdatePitch
#include "bdc.h"

/* Pitch step of the path camera mode (slot 6 helper of `GameQuestCamPathModeUpdateEye`). Does
   nothing in state 2 (`stateIndex`) or when both nodes of the attached `segment` have a negative
   pitch (`node+0x40`). Otherwise a negative node pitch defaults to pi/2 and the two are lerped by
   the segment parameter `t`. The horizontal direction from the eye goal to the followed target
   (vtable entry 2 position) is normalised (posZ of `g_gameQuestCamPathModeAxisConsts` when its
   length is <= 1e-6) and compared with the horizontal segment direction: angle = acos(dot), folded
   into [0, pi/2]. When the pitch is not below that angle (or NaN), `pitchDist` becomes the
   horizontal target distance. Otherwise the pitch is signed by the y of cross(dir, segDir), the eye
   goal is set to the lerped segment point minus `segDir * cos(pitch) * pitchDist` (keeping the old
   goal y), and the tracked entry object's eye vector (`*entry + 0x40`) becomes goal plus segment
   `dir` rotated through the never-initialised stack matrix (X step by 0*pitch; the Y and Z steps take
   their quarter-turn angles from lanes y and z of that matrix's first column, as in the binary),
   times `pitchDist`, with y taken from the controller's `lookAt.y`. Both non-returning
   paths finish by copying `targetDist` to `pitchDist` when the controller snaps (`ctrl->snap`). */

typedef struct PitchNode {
  ScePspFVector4 pos; /* +0x00 */
  u8 _unk10[0x30];
  float pitch; /* +0x40 negative = default pi/2 */
} PitchNode;

typedef struct PitchEntryObj {
  u8 _unk00[0x40];
  ScePspFVector4 eye; /* +0x40 */
} PitchEntryObj;

typedef struct PitchOwner {
  void *_unk00;
  const VtblEntry *vtbl; /* +0x04 */
} PitchOwner;

void GameQuestCamUpdatePitch(GameQuestCamPathMode *self)

{
  float mtx[4][4]; /* [column][lane]; never initialised, as in the binary */
  float rot[3][4][4];
  float nm[4][4];
  ScePspFVector4 dir;
  ScePspFVector4 segDir;
  ScePspFVector4 offs;
  ScePspFVector4 pA;
  ScePspFVector4 pB;
  ScePspFVector4 eye;
  GameQuestPathSegment *seg;
  const PitchOwner *owner;
  const VtblEntry *e;
  const ScePspFVector4 *pos;
  PitchEntryObj *entryObj;
  float pitchA;
  float pitchB;
  float t;
  float pitch;
  float len;
  float inv;
  float d;
  float k;
  float dot;
  float ang;
  float absAng;
  float savedY;
  float lookY;
  float crossY;
  float scl;
  float ax;
  float ay;
  float az;
  float cx;
  float sx;
  float cy;
  float sy;
  float cz;
  float sz;
  int step;
  int c;
  int r;

  if ((self->base).stateIndex == 2) {
    return;
  }
  seg = (GameQuestPathSegment *)self->segment;
  pitchA = ((PitchNode *)seg->from)->pitch;
  pitchB = ((PitchNode *)seg->to)->pitch;
  if (pitchA < 0.0f && pitchB < 0.0f) {
    return;
  }
  if (pitchA < 0.0f) {
    pitchA = 1.5707964f;
  }
  if (pitchB < 0.0f) {
    pitchB = 1.5707964f;
  }
  t = ((GameQuestPathSegment *)self->segment)->t;
  pitch = (1.0f - t) * pitchA + t * pitchB;

  /* dir = horizontal (target - goal), normalised */
  owner = (const PitchOwner *)(self->base).base.base.followed;
  e = &owner->vtbl[2];
  pos = ((const ScePspFVector4 *(*)(void *))e->fn)((char *)owner + e->delta);
  dir.x = pos->x - (self->base).base.base.goal.x;
  dir.y = pos->y - (self->base).base.base.goal.y;
  dir.z = pos->z - (self->base).base.base.goal.z;
  dir.w = pos->w;
  if (g_staticZeroFGuard == 0) {
    g_staticZeroFGuard = 1;
    g_staticZeroF = 0.0f;
  }
  dir.y = 0.0f;
  len = __builtin_sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
  if (len <= 1e-06f) {
    dir = g_gameQuestCamPathModeAxisConsts.posZ;
  } else {
    inv = 1.0f / len;
    dir.x = dir.x * inv;
    dir.y = dir.y * inv;
    dir.z = dir.z * inv;
    dir.w = 0.0f;
    d = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
    k = VfRsq(d);
    if (d == 0.0f) {
      k = 0.0f;
    }
    dir.x = VfSat1(dir.x * k);
    dir.y = VfSat1(dir.y * k);
    dir.z = VfSat1(dir.z * k);
  }

  /* segDir = horizontal segment direction, normalised; dot with dir */
  segDir = ((GameQuestPathSegment *)self->segment)->dir;
  if (g_staticZeroFGuard == 0) {
    g_staticZeroFGuard = 1;
    g_staticZeroF = 0.0f;
  }
  segDir.y = 0.0f;
  d = segDir.x * segDir.x + segDir.y * segDir.y + segDir.z * segDir.z;
  k = VfRsq(d);
  if (d == 0.0f) {
    k = 0.0f;
  }
  segDir.x = VfSat1(segDir.x * k);
  segDir.y = VfSat1(segDir.y * k);
  segDir.z = VfSat1(segDir.z * k);
  segDir.w = 0.0f;
  dot = dir.x * segDir.x + dir.y * segDir.y + dir.z * segDir.z;
  ang = (float)acos((double)dot);
  if (!(ang <= 1.5707964f)) {
    ang = 3.1415927f - ang;
  }
  absAng = ang;
  if (absAng < 0.0f) {
    absAng = -ang;
  }
  if (!(pitch < absAng)) {
    self->pitchDist = len;
  } else {
    if (g_staticZeroFGuard == 0) {
      g_staticZeroFGuard = 1;
      g_staticZeroF = 0.0f;
    }
    savedY = (self->base).base.base.goal.y;
    if (g_staticZeroF2Guard == 0) {
      g_staticZeroF2Guard = 1;
      g_staticZeroF2 = 0.0f;
    }
    lookY = (self->base).base.base.ctrl->lookAt.y;
    /* y of cross(dir, segDir) */
    crossY = dir.z * segDir.x - dir.x * segDir.z;
    if (g_staticZeroFGuard == 0) {
      g_staticZeroFGuard = 1;
      g_staticZeroF = 0.0f;
    }
    if (!(crossY < 0.0f)) {
      pitch = -pitch;
    }
    scl = (float)cos((double)pitch) * self->pitchDist;

    /* offs = -(segment dir * cos(pitch) * pitchDist) */
    offs = ((GameQuestPathSegment *)self->segment)->dir;
    scl = -scl;
    offs.x = offs.x * scl;
    offs.y = offs.y * scl;
    offs.z = offs.z * scl;
    offs.w = 0.0f;

    /* goal = lerp(from->pos, to->pos, t) + offs, old goal y kept */
    pA = ((PitchNode *)((GameQuestPathSegment *)self->segment)->from)->pos;
    scl = 1.0f - ((GameQuestPathSegment *)self->segment)->t;
    pA.x = pA.x * scl;
    pA.y = pA.y * scl;
    pA.z = pA.z * scl;
    pA.w = 0.0f;
    pB = ((PitchNode *)((GameQuestPathSegment *)self->segment)->to)->pos;
    scl = ((GameQuestPathSegment *)self->segment)->t;
    pB.x = pB.x * scl;
    pB.y = pB.y * scl;
    pB.z = pB.z * scl;
    pA.x = pA.x + pB.x;
    pA.y = pA.y + pB.y;
    pA.z = pA.z + pB.z;
    (self->base).base.base.goal.x = pA.x + offs.x;
    (self->base).base.base.goal.y = pA.y + offs.y;
    (self->base).base.base.goal.z = pA.z + offs.z;
    (self->base).base.base.goal.w = pA.w;
    if (g_staticZeroFGuard == 0) {
      g_staticZeroFGuard = 1;
      g_staticZeroF = 0.0f;
    }
    (self->base).base.base.goal.y = savedY;

    /* Angle vector (0, 1, 0) * pitch, scaled by S703: only its x lane (0*pitch radians) drives the
       X step. The Y and Z vrots read S101/S102, lanes y and z of the uninitialised matrix's first
       column (C100, loaded before any step), in quarter turns. Each step replaces the matrix M
       (fields as columns) by R * transpose(M) (vmmul.q E200, E200, E000). */
    offs = ((GameQuestPathSegment *)self->segment)->dir;
    ax = 0.0f * pitch;
    ay = mtx[0][1];
    az = mtx[0][2];
    cx = __builtin_cosf(ax);
    sx = __builtin_sinf(ax);
    cy = VfCosQuarter(ay);
    sy = VfSinQuarter(ay);
    cz = VfCosQuarter(az);
    sz = VfSinQuarter(az);
    for (step = 0; step < 3; step++) {
      for (c = 0; c < 4; c++) {
        for (r = 0; r < 4; r++) {
          rot[step][c][r] = (c == r) ? 1.0f : 0.0f;
        }
      }
    }
    /* about X: columns (1,0,0,0), (0,c,s,0), (0,-s,c,0), (0,0,0,1) */
    rot[0][1][1] = cx;
    rot[0][1][2] = sx;
    rot[0][2][1] = -sx;
    rot[0][2][2] = cx;
    /* about Y: columns (c,0,-s,0), (0,1,0,0), (s,0,c,0), (0,0,0,1) */
    rot[1][0][0] = cy;
    rot[1][0][2] = -sy;
    rot[1][2][0] = sy;
    rot[1][2][2] = cy;
    /* about Z: columns (c,s,0,0), (-s,c,0,0), (0,0,1,0), (0,0,0,1) */
    rot[2][0][0] = cz;
    rot[2][0][1] = sz;
    rot[2][1][0] = -sz;
    rot[2][1][1] = cz;
    for (step = 0; step < 3; step++) {
      for (c = 0; c < 4; c++) {
        for (r = 0; r < 4; r++) {
          nm[c][r] = mtx[0][c] * rot[step][0][r] + mtx[1][c] * rot[step][1][r] +
                     mtx[2][c] * rot[step][2][r] + mtx[3][c] * rot[step][3][r];
        }
      }
      for (c = 0; c < 4; c++) {
        for (r = 0; r < 4; r++) {
          mtx[c][r] = nm[c][r];
        }
      }
    }
    /* vtfm3.t (E form): rotated = sum of column k * dir[k]; w lane left 0 by the last vrot */
    pA.x = mtx[0][0] * offs.x + mtx[1][0] * offs.y + mtx[2][0] * offs.z;
    pA.y = mtx[0][1] * offs.x + mtx[1][1] * offs.y + mtx[2][1] * offs.z;
    pA.z = mtx[0][2] * offs.x + mtx[1][2] * offs.y + mtx[2][2] * offs.z;
    scl = self->pitchDist;
    offs.x = pA.x * scl;
    offs.y = pA.y * scl;
    offs.z = pA.z * scl;
    eye.x = (self->base).base.base.goal.x + offs.x;
    eye.y = (self->base).base.base.goal.y + offs.y;
    eye.z = (self->base).base.base.goal.z + offs.z;
    eye.w = (self->base).base.base.goal.w;
    if (g_staticZeroFGuard == 0) {
      g_staticZeroFGuard = 1;
      g_staticZeroF = 0.0f;
    }
    eye.y = lookY;
    entryObj = *(PitchEntryObj **)(self->base).entry;
    entryObj->eye = eye;
  }
  if ((self->base).base.base.ctrl->snap != 0) {
    self->pitchDist = self->targetDist;
  }
}
