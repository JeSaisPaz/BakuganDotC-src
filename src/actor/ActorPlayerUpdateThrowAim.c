// bdc 0x088e4048 ActorPlayerUpdateThrowAim
#include "bdc.h"

/* Aiming step of the Bakugan throw of the player actor (`ActorPlayer`), run by
   `ActorPlayerStateThrow`. Clears `ballTarget`; does nothing more while a gimmick is locked
   (`lockTarget`) or no ball is held. Otherwise it builds the aim from the active camera: origin =
   player position +18 in y plus the model's (-15,0,0) offset, far point = camera eye + 150 x the
   normalised camera direction, `aimPoint` = eye + 100 x direction; it fills the ray query
   `g_collisionRayDesc` (origin, direction, reciprocal direction) and sets the ball's ambient
   alpha to 1. With an aim sprite it casts the segment origin->far (`g_collisionSegmentDesc`)
   and pulls the far point to the hit, shows `"ret_aim"` there, and stores `aimOrigin`/`aimHit`.
   Unless a lock appeared meanwhile or motion bits 0x300001 are set, it searches a lock-on target
   within `g_throwAimRangeSq` (150^2, initialised on first use): active field gimmicks of type
   0xbbe/0xbc7/0xbdc and core points 0x1778 (hidden ones only while the player scans), skipping a
   0xbdc whose effect is active; the probe point is the shape centre pulled 1.5 radii toward the
   origin, the gimmick must be within ~18 degrees (dot > 0.95) of the far point seen from the
   field camera, unblocked by a 1.68 swept sphere, and within 0.2356 rad of the aim direction;
   the nearest one wins and its target point (vtable entry 18) becomes the lock point. Then
   every active actor with `ActorNpc` `aiState` != 8, its matrix point +12 in y in range, within
   0.157 rad of the aim direction, facing the camera direction (dot > cos(pi/3)) and with a
   clear segment to the far point: the nearest one becomes `ballTarget` (clearing the gimmick
   choice) with the lock point 6.3 short of it. A chosen gimmick is stored in `lockTarget`; with
   any choice the sprite switches to `"ret_aim_rock"` and the lock point is copied to `aimHit`,
   the sprite position, `aimPoint` and `lockPoint`. Every path past the early returns ends by
   copying the field camera's `pitch` into `stateVec[0]`.
   VFPU bank constants: S713 (0) is the zero-length fallback of every normalise/reciprocal and the
   w of their results; S703 (2/pi) scales pi/3 before `vcos.s`, so the limit is cosf(pi/3). */

static inline void ThrowAimCopy4(float *d, const float *s)
{
  d[0] = s[0];
  d[1] = s[1];
  d[2] = s[2];
  d[3] = s[3];
}

/* d.xyz = clamp(v.xyz / |v.xyz|, -1, 1) (zero vector when |v| == 0), d.w = 0 (vpfxd + S713). */
static inline void ThrowAimNormalize(float *d, const float *v)
{
  float x = v[0];
  float y = v[1];
  float z = v[2];
  float lenSq = x * x + y * y + z * z;
  float k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);

  d[0] = VfSat1(x * k);
  d[1] = VfSat1(y * k);
  d[2] = VfSat1(z * k);
  d[3] = 0.0f;
}

/* |a.xyz - b.xyz|^2 */
static inline float ThrowAimDistSq(const float *a, const float *b)
{
  float dx = a[0] - b[0];
  float dy = a[1] - b[1];
  float dz = a[2] - b[2];

  return dx * dx + dy * dy + dz * dz;
}

void ActorPlayerUpdateThrowAim(ActorPlayer *self)
{
  float eye[4];
  float dir[4];
  float origin[4];
  float offset[4];
  float far[4];
  float lockPoint[4];
  float toCenter[4];
  float probe[4];
  float camEye[4];
  float viewFar[4];
  float viewGim[4];
  float aimDir[4];
  float centerDir[4];
  float gimPoint[4];
  float actorPoint[4];
  float actorDir[4];
  float fwd[4];
  float tmp[4];
  const float *m;
  const float *center;
  GameFieldTask *field;
  GameGimmick *gimmick;
  GameGimmick *bestGimmick;
  CollisionSphere *shape;
  const VtblEntry *vtbl;
  GfxSprite *sprite;
  Actor *actor;
  ActorPlayer *player;
  u32 mask;
  u8 scanOn;
  u8 found;
  float bestDist;
  float dist2;
  float hitDist;
  float reach;
  float k;
  float lenSq;
  float cosAngle;
  float cosLimit;
  s32 i;

  self->ballTarget = NULL;
  if (self->lockTarget != NULL) {
    return;
  }
  if (self->ball == NULL) {
    return;
  }
  mask = ((1u << (((CollisionCollider *)self->base.collider2)->layer & 0x1f)) ^ 0x01bf077eu) |
         0x08000000u | 0x10000000u;

  /* eye = camera eye; dir = normalize(camera dir), w = 0 */
  ThrowAimCopy4(eye, g_gfxActiveCamera->eye);
  ThrowAimCopy4(dir, g_gfxActiveCamera->dir);
  ThrowAimNormalize(dir, dir);
  self->base.aimTargetKind = 0;
  /* origin = camera target, overwritten at once by the player position */
  ThrowAimCopy4(origin, g_gfxActiveCamera->target);
  ThrowAimCopy4(origin, self->base.base.pos);
  origin[1] = origin[1] + 18.0f;
  offset[0] = -15.0f;
  offset[1] = 0.0f;
  offset[2] = 0.0f;
  offset[3] = 0.0f;
  /* offset.xyz = rootMatrix (3x3, vtfm3 E form) * offset; origin.xyz += offset.xyz */
  m = self->base.base.data->rootMatrix;
  tmp[0] = m[0] * offset[0] + m[4] * offset[1] + m[8] * offset[2];
  tmp[1] = m[1] * offset[0] + m[5] * offset[1] + m[9] * offset[2];
  tmp[2] = m[2] * offset[0] + m[6] * offset[1] + m[10] * offset[2];
  offset[0] = tmp[0];
  offset[1] = tmp[1];
  offset[2] = tmp[2];
  origin[0] = origin[0] + offset[0];
  origin[1] = origin[1] + offset[1];
  origin[2] = origin[2] + offset[2];
  /* far = eye + dir * 150 (w = eye.w) */
  far[0] = eye[0] + dir[0] * 150.0f;
  far[1] = eye[1] + dir[1] * 150.0f;
  far[2] = eye[2] + dir[2] * 150.0f;
  far[3] = eye[3];
  /* ray query: origin, dir and per-axis 1/dir (0 for a zero axis), invDir.w = 0 */
  g_collisionRayDesc.origin.x = origin[0];
  g_collisionRayDesc.origin.y = origin[1];
  g_collisionRayDesc.origin.z = origin[2];
  g_collisionRayDesc.origin.w = origin[3];
  g_collisionRayDesc.dir.x = dir[0];
  g_collisionRayDesc.dir.y = dir[1];
  g_collisionRayDesc.dir.z = dir[2];
  g_collisionRayDesc.dir.w = dir[3];
  g_collisionRayDesc.invDir.x =
      (g_collisionRayDesc.dir.x == 0.0f) ? 0.0f : VfRcp(g_collisionRayDesc.dir.x);
  g_collisionRayDesc.invDir.y =
      (g_collisionRayDesc.dir.y == 0.0f) ? 0.0f : VfRcp(g_collisionRayDesc.dir.y);
  g_collisionRayDesc.invDir.z =
      (g_collisionRayDesc.dir.z == 0.0f) ? 0.0f : VfRcp(g_collisionRayDesc.dir.z);
  g_collisionRayDesc.invDir.w = 0.0f;
  /* aimPoint = eye + dir * 100 (w = eye.w) */
  self->base.aimPoint[0] = eye[0] + dir[0] * 100.0f;
  self->base.aimPoint[1] = eye[1] + dir[1] * 100.0f;
  self->base.aimPoint[2] = eye[2] + dir[2] * 100.0f;
  self->base.aimPoint[3] = eye[3];
  if (self->base.aimTargetKind != 0) {
    ThrowAimCopy4(self->base.aimPoint, far);
  }
  ((ActorBall *)self->ball)->base.ambient[3] = 1.0f;
  self->aimState[1] = 0;
  if (self->aimSprite == NULL) {
    goto setPitch;
  }

  /* segment origin -> far (dir.w = far.w) */
  ThrowAimCopy4(g_collisionSegmentDesc.start, origin);
  g_collisionSegmentDesc.dir[0] = far[0] - origin[0];
  g_collisionSegmentDesc.dir[1] = far[1] - origin[1];
  g_collisionSegmentDesc.dir[2] = far[2] - origin[2];
  g_collisionSegmentDesc.dir[3] = far[3];
  if (CollisionRaycast(mask, &g_collisionSegmentBlock2, 0) != NULL) {
    far[0] = g_collisionHitResult.point.x;
    far[1] = g_collisionHitResult.point.y;
    far[2] = g_collisionHitResult.point.z;
    far[3] = g_collisionHitResult.point.w;
  }
  if (self->aimSprite != NULL) {
    sprite = (GfxSprite *)self->aimSprite;
    sprite->texture = GfxFindTexture("ret_aim");
    ((GfxSprite *)self->aimSprite)->flags |= 1;
    sprite = (GfxSprite *)self->aimSprite;
    sprite->posX = far[0];
    sprite->posY = far[1];
    sprite->posZ = far[2];
    sprite->posW = far[3];
  }
  ThrowAimCopy4(self->aimOrigin, origin);
  ThrowAimCopy4(self->aimHit, far);
  if (self->lockTarget != NULL) {
    goto setPitch;
  }
  if ((self->base.motion & 0x300001) != 0) {
    goto setPitch;
  }
  if (g_throwAimRangeInit == 0) {
    g_throwAimRangeInit = 1;
    g_throwAimRangeSq = 22500.0f;
  }
  bestDist = 1000000.0f;
  field = (GameFieldTask *)GameFieldFindTask();
  gimmick = field->gimmicks;
  lockPoint[2] = 0.0f;
  lockPoint[1] = 0.0f;
  bestGimmick = NULL;
  lockPoint[0] = 0.0f;
  lockPoint[3] = 0.0f;
  scanOn = 0;
  player = (ActorPlayer *)ActorFindPlayer();
  if (player != NULL && player->scan != 0) {
    scanOn = 1;
  }

  /* gimmick candidates */
  for (; gimmick != NULL; gimmick = (GameGimmick *)gimmick->base.base.next) {
    if (gimmick->active == 0) {
      continue;
    }
    switch (gimmick->typeId) {
    case 0xbbe:
    case 0xbc7:
    case 0xbdc:
      break;
    case 0x1778:
      if (((GameGimmickCorePoint *)gimmick)->invisible != 0 && scanOn == 0) {
        continue;
      }
      break;
    default:
      continue;
    }
    vtbl = &((const VtblEntry *)gimmick->base.base.vtable)[19];
    shape = ((CollisionSphere * (*)(void *)) vtbl->fn)((u8 *)gimmick + vtbl->delta);
    if (shape == NULL) {
      continue;
    }
    if (gimmick->typeId == 0xbdc && ((GameGimmickCollidable *)gimmick)->effectActive != 0) {
      continue;
    }
    center = shape->center;
    /* toCenter = normalize(centre - origin) */
    tmp[0] = center[0] - origin[0];
    tmp[1] = center[1] - origin[1];
    tmp[2] = center[2] - origin[2];
    ThrowAimNormalize(toCenter, tmp);
    reach = shape->radius * 1.5f;
    /* toCenter rescaled to length reach (no clamp); probe = centre - toCenter */
    lenSq = toCenter[0] * toCenter[0] + toCenter[1] * toCenter[1] + toCenter[2] * toCenter[2];
    k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
    k = k * reach;
    toCenter[0] = toCenter[0] * k;
    toCenter[1] = toCenter[1] * k;
    toCenter[2] = toCenter[2] * k;
    probe[0] = center[0] - toCenter[0];
    probe[1] = center[1] - toCenter[1];
    probe[2] = center[2] - toCenter[2];
    probe[3] = shape->radiusSq;
    dist2 = ThrowAimDistSq(probe, origin);
    if (g_throwAimRangeSq <= dist2) {
      continue;
    }
    /* cosAngle = dot(normalize(far - camEye), normalize(gimmick pos - camEye)) */
    ThrowAimCopy4(camEye, self->base.camera->base.eye);
    tmp[0] = far[0] - camEye[0];
    tmp[1] = far[1] - camEye[1];
    tmp[2] = far[2] - camEye[2];
    ThrowAimNormalize(viewFar, tmp);
    tmp[0] = gimmick->base.pos[0] - camEye[0];
    tmp[1] = gimmick->base.pos[1] - camEye[1];
    tmp[2] = gimmick->base.pos[2] - camEye[2];
    ThrowAimNormalize(viewGim, tmp);
    cosAngle = viewFar[0] * viewGim[0] + viewFar[1] * viewGim[1] + viewFar[2] * viewGim[2];
    if (cosAngle <= 0.95f) {
      continue;
    }
    /* swept sphere (radius 1.68) from origin toward the shape centre:
       start.w = radius^2, dir.w = |dir.xyz| */
    g_collisionSweptSphereDesc.radius = 1.68000007f;
    g_collisionSweptSphereDesc.start.x = origin[0];
    g_collisionSweptSphereDesc.start.y = origin[1];
    g_collisionSweptSphereDesc.start.z = origin[2];
    g_collisionSweptSphereDesc.start.w = origin[3];
    g_collisionSweptSphereDesc.dir.x = center[0] - origin[0];
    g_collisionSweptSphereDesc.dir.y = center[1] - origin[1];
    g_collisionSweptSphereDesc.dir.z = center[2] - origin[2];
    g_collisionSweptSphereDesc.dir.w = shape->radiusSq;
    g_collisionSweptSphereDesc.start.w =
        g_collisionSweptSphereDesc.radius * g_collisionSweptSphereDesc.radius;
    g_collisionSweptSphereDesc.dir.w = __builtin_sqrtf(
        g_collisionSweptSphereDesc.dir.x * g_collisionSweptSphereDesc.dir.x +
        g_collisionSweptSphereDesc.dir.y * g_collisionSweptSphereDesc.dir.y +
        g_collisionSweptSphereDesc.dir.z * g_collisionSweptSphereDesc.dir.z);
    hitDist = dist2;
    if (CollisionRaycast(mask, g_collisionSweptSphereDesc.shapeBlock, 0) != NULL) {
      hitDist = ThrowAimDistSq(&g_collisionHitResult.point.x, origin);
    }
    if (!(dist2 <= hitDist)) {
      continue;
    }
    /* cosAngle = dot(normalize(far - origin), normalize(centre - origin)) */
    for (i = 0; i < 4; i++) {
      aimDir[i] = 0.0f;
      centerDir[i] = 0.0f;
    }
    tmp[0] = far[0] - origin[0];
    tmp[1] = far[1] - origin[1];
    tmp[2] = far[2] - origin[2];
    ThrowAimNormalize(aimDir, tmp);
    tmp[0] = center[0] - origin[0];
    tmp[1] = center[1] - origin[1];
    tmp[2] = center[2] - origin[2];
    ThrowAimNormalize(centerDir, tmp);
    cosAngle = aimDir[0] * centerDir[0] + aimDir[1] * centerDir[1] + aimDir[2] * centerDir[2];
    if (!(acosf(cosAngle) < 0.235619467f)) {
      continue;
    }
    if (!(dist2 < bestDist)) {
      continue;
    }
    bestGimmick = gimmick;
    vtbl = &((const VtblEntry *)gimmick->base.base.vtable)[18];
    ((void (*)(void *, float *))vtbl->fn)((u8 *)gimmick + vtbl->delta, gimPoint);
    ThrowAimCopy4(lockPoint, gimPoint);
    bestDist = dist2;
  }

  /* actor candidates */
  actor = *(Actor **)ActorGetList();
  found = 0;
  for (; actor != NULL; actor = (Actor *)actor->base.base.next) {
    vtbl = &((const VtblEntry *)actor->base.base.vtable)[11];
    if (((s32 (*)(void *))vtbl->fn)((u8 *)actor + vtbl->delta) == 0) {
      continue;
    }
    if (((ActorNpc *)actor)->aiState == 8) {
      continue;
    }
    ThrowAimCopy4(actorPoint, &actor->mtx[12]);
    actorPoint[1] = actorPoint[1] + 12.0f;
    dist2 = ThrowAimDistSq(actorPoint, origin);
    if (g_throwAimRangeSq <= dist2) {
      continue;
    }
    /* cosAngle = dot(normalize(far - origin), actorDir = normalize(actorPoint - origin)) */
    for (i = 0; i < 4; i++) {
      aimDir[i] = 0.0f;
      actorDir[i] = 0.0f;
    }
    tmp[0] = far[0] - origin[0];
    tmp[1] = far[1] - origin[1];
    tmp[2] = far[2] - origin[2];
    ThrowAimNormalize(aimDir, tmp);
    tmp[0] = actorPoint[0] - origin[0];
    tmp[1] = actorPoint[1] - origin[1];
    tmp[2] = actorPoint[2] - origin[2];
    ThrowAimNormalize(actorDir, tmp);
    cosAngle = aimDir[0] * actorDir[0] + aimDir[1] * actorDir[1] + aimDir[2] * actorDir[2];
    if (!(acosf(cosAngle) < 0.157079637f)) {
      continue;
    }
    /* fwd = normalize(rootMatrix (3x3) * (0,0,1)); dir is re-normalised in place;
       cosAngle = dot(dir, fwd); cosLimit = cos(pi/3) (vcos.s of pi/3 x S703) */
    fwd[0] = 0.0f;
    fwd[1] = 0.0f;
    fwd[2] = 1.0f;
    fwd[3] = 0.0f;
    m = actor->base.data->rootMatrix;
    tmp[0] = m[0] * fwd[0] + m[4] * fwd[1] + m[8] * fwd[2];
    tmp[1] = m[1] * fwd[0] + m[5] * fwd[1] + m[9] * fwd[2];
    tmp[2] = m[2] * fwd[0] + m[6] * fwd[1] + m[10] * fwd[2];
    ThrowAimNormalize(fwd, tmp);
    ThrowAimNormalize(dir, dir);
    cosAngle = dir[0] * fwd[0] + dir[1] * fwd[1] + dir[2] * fwd[2];
    cosLimit = __builtin_cosf(1.04719758f);
    if (cosAngle <= cosLimit) {
      continue;
    }
    /* segment actorPoint -> far must be clear (dir.w = far.w) */
    ThrowAimCopy4(g_collisionSegmentDesc.start, actorPoint);
    g_collisionSegmentDesc.dir[0] = far[0] - actorPoint[0];
    g_collisionSegmentDesc.dir[1] = far[1] - actorPoint[1];
    g_collisionSegmentDesc.dir[2] = far[2] - actorPoint[2];
    g_collisionSegmentDesc.dir[3] = far[3];
    if (CollisionRaycast(mask, &g_collisionSegmentBlock2, 0) != NULL) {
      continue;
    }
    if (!(dist2 < bestDist)) {
      continue;
    }
    found = 1;
    bestGimmick = NULL;
    reach = __builtin_sqrtf(dist2) - 6.3f;
    /* lockPoint = origin + actorDir * reach (w = origin.w) */
    lockPoint[0] = origin[0] + actorDir[0] * reach;
    lockPoint[1] = origin[1] + actorDir[1] * reach;
    lockPoint[2] = origin[2] + actorDir[2] * reach;
    lockPoint[3] = origin[3];
    bestDist = dist2;
    self->ballTarget = actor;
  }

  if (bestGimmick != NULL) {
    self->lockTarget = bestGimmick;
  } else if (found == 0) {
    goto setPitch;
  }
  if (self->aimSprite != NULL) {
    sprite = (GfxSprite *)self->aimSprite;
    sprite->texture = GfxFindTexture("ret_aim_rock");
    ((GfxSprite *)self->aimSprite)->flags |= 1;
  }
  /* aimHit = sprite position = aimPoint = lockPoint (field) = chosen lock point */
  sprite = (GfxSprite *)self->aimSprite;
  ThrowAimCopy4(self->aimHit, lockPoint);
  sprite->posX = self->aimHit[0];
  sprite->posY = self->aimHit[1];
  sprite->posZ = self->aimHit[2];
  sprite->posW = self->aimHit[3];
  self->base.aimPoint[0] = sprite->posX;
  self->base.aimPoint[1] = sprite->posY;
  self->base.aimPoint[2] = sprite->posZ;
  self->base.aimPoint[3] = sprite->posW;
  ThrowAimCopy4(self->lockPoint, self->base.aimPoint);

setPitch:
  self->base.stateVec[0] = self->base.camera->pitch;
}
