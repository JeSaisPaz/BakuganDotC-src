// bdc 0x0881ec04 GfxEffectRunCommands
#include "bdc.h"

/* Interpreter of the effect definition bytecode: executes the command list `cmds` of one key
   (frame) of an effect until a yield command, applying each command to the effect object
   (position/size/direction/colour/UV/matrix fields, sprite and mesh-object setup, the vec4
   registers `vecRegs` and float registers `regs`, sound emitters, child-particle emission, hit
   volumes and camera shake). Each command is a 4-byte header `{u16 argKinds; u8 argCount;
   u8 opcode}` followed by `argCount` argument words. Returns -1 when a command asks for the effect
   to be destroyed; otherwise, after a yield command, the pass count (`pass - 1`, or the value set
   by command 0xb4) for `GfxEffectUpdate`; for the persistent block (`isDefault`) the attached
   mesh chain is then stepped (`GfxEffectChainUpdate`) when it is ready.
   VFPU bank constants read by the listing are literals here: S703 (2/π, so `vsin`/`vcos`/`vrot` of
   `angle * S703` are `sinf`/`cosf` of the angle), S713 (0: the scale of a zero-length vector and
   lane w of every `sv.q C710` result), C720 (0 vector: command 200 stores it over `chainAnchor`
   and the following `emitter` word) and S730 (0: quaternion w of the rotated vector). */

/* 1/|v.xyz| as vdot.t + vcmp EZ + vrsq + vcmovt S713: 0 for a zero-length vector. */
static float GfxEffectRunCommandsInvLen(const float *v)
{
  float len2;
  float k;

  len2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
  k = VfRsq(len2);
  if (len2 == 0.0f) {
    k = 0.0f;
  }
  return k;
}

/* v.xyz = v.xyz / |v.xyz| clamped to [-1, 1] (vpfxd [-1:1,-1:1,-1:1,M] + vscl.t); w untouched. */
static void GfxEffectRunCommandsNormalize(float *v)
{
  float k;

  k = GfxEffectRunCommandsInvLen(v);
  v[0] = VfSat1(v[0] * k);
  v[1] = VfSat1(v[1] * k);
  v[2] = VfSat1(v[2] * k);
}

/* d.xyz = a.xyz - b.xyz with d.w = a.w (vsub.t keeps the first operand's w, sv.q stores it). */
static void GfxEffectRunCommandsSub3(float *d, const float *a, const float *b)
{
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3];
}

/* v.xyz scaled to length `len` (vrsq with vcmovt S713, vmul.s, vscl.t into C710): a zero vector
   stays zero; v.w = 0 (lane w of C710 is the bank's S713). */
static void GfxEffectRunCommandsSetLength(float *v, float len)
{
  float s;

  s = GfxEffectRunCommandsInvLen(v) * len;
  v[0] = v[0] * s;
  v[1] = v[1] * s;
  v[2] = v[2] * s;
  v[3] = 0.0f;
}

/* d.xyz = s.xyz × t.xyz (vcrsp.t); d may alias s or t. */
static void GfxEffectRunCommandsCross(float *d, const float *s, const float *t)
{
  float x;
  float y;
  float z;

  x = s[1] * t[2] - s[2] * t[1];
  y = s[2] * t[0] - s[0] * t[2];
  z = s[0] * t[1] - s[1] * t[0];
  d[0] = x;
  d[1] = y;
  d[2] = z;
}

/* vrot [C,0,-S,0] / [S,0,C,0] + two vdot.t: turns v.xz by `angle` radians about Y; v.y, v.w kept. */
static void GfxEffectRunCommandsTurnY(float *v, float angle)
{
  float c;
  float s;
  float x;
  float z;

  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
  x = v[0] * c + v[1] * 0.0f + v[2] * -s;
  z = v[0] * s + v[1] * 0.0f + v[2] * c;
  v[0] = x;
  v[2] = z;
}

/* v = q ⊗ (v.xyz, 0) ⊗ conj(q) with q = (axis.xyz · sin(angle/2), cos(angle/2)) (vcst 1/π, vcos.s,
   vsin.s, vscl.t, vpfxs conjugate, vmov.s S203 = S730, two vqmul.q); all four lanes of v written. */
static void GfxEffectRunCommandsQuatRotate(float *v, const float *axis, float angle)
{
  float half;
  float c;
  float s;
  ScePspFVector4 q;
  ScePspFVector4 u;
  ScePspFVector4 t;

  half = 0.318309873f * angle;
  c = VfCosQuarter(half);
  s = VfSinQuarter(half);
  q.x = axis[0] * s;
  q.y = axis[1] * s;
  q.z = axis[2] * s;
  q.w = c;
  u.x = v[0];
  u.y = v[1];
  u.z = v[2];
  u.w = 0.0f;
  t.x = q.x * u.w + q.y * u.z - q.z * u.y + q.w * u.x;
  t.y = -q.x * u.z + q.y * u.w + q.z * u.x + q.w * u.y;
  t.z = q.x * u.y - q.y * u.x + q.z * u.w + q.w * u.z;
  t.w = -q.x * u.x - q.y * u.y - q.z * u.z + q.w * u.w;
  v[0] = t.x * q.w + t.y * -q.z - t.z * -q.y + t.w * -q.x;
  v[1] = -t.x * -q.z + t.y * q.w + t.z * -q.x + t.w * -q.y;
  v[2] = t.x * -q.y - t.y * -q.x + t.z * q.w + t.w * -q.z;
  v[3] = -t.x * -q.x - t.y * -q.y - t.z * -q.z + t.w * q.w;
}

/* Rotation matrix about X (axis 0), Y (1) or Z (2) by `angle` radians as built by vidt.q/vrot.q,
   stored row by row (m[f*4 + lane] = column f of the VFPU matrix). */
static void GfxEffectRunCommandsRotation(float *m, s32 axis, float angle)
{
  float c;
  float s;
  s32 k;

  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
  for (k = 0; k < 16; k++) {
    m[k] = (k % 5 == 0) ? 1.0f : 0.0f;
  }
  if (axis == 0) {
    m[5] = c;
    m[6] = s;
    m[9] = -s;
    m[10] = c;
  } else if (axis == 1) {
    m[0] = c;
    m[2] = -s;
    m[8] = s;
    m[10] = c;
  } else {
    m[0] = c;
    m[1] = s;
    m[4] = -s;
    m[5] = c;
  }
}

/* vmmul.q M000, M100, M200 with M100 loaded from a and M200 from b:
   d[j*4 + r] = Σ_k a[k*4 + r] · b[j*4 + k] (column-major a · b). d must not alias a or b. */
static void GfxEffectRunCommandsMatMul(float *d, const float *a, const float *b)
{
  s32 j;
  s32 r;

  for (j = 0; j < 4; j++) {
    for (r = 0; r < 4; r++) {
      d[j * 4 + r] = a[r] * b[j * 4] + a[4 + r] * b[j * 4 + 1] + a[8 + r] * b[j * 4 + 2] +
                     a[12 + r] * b[j * 4 + 3];
    }
  }
}

#define READ_F() GfxEffectReadFloatArg(effect, args, &cursor, hdr)
#define READ_I() GfxEffectReadIntArg(effect, iargs, &cursor, hdr)

int GfxEffectRunCommands(GfxEffect *effect, u8 *cmds, u32 isDefault, int pass)
{
  u8 runChain = (u8)isDefault;
  s32 ret = pass - 1;
  u8 yield = 0;
  float *target;
  s32 cursor;
  bool colorDirty;
  float *args;
  s32 *iargs;
  u16 *hdr;
  BtlBakugan *owner;
  ActorPlayer *player;
  GfxMeshObj *mesh;
  float *p;
  float *vb;
  s32 i;
  s32 j;
  s32 n;
  s32 hi;
  s32 lo;
  u32 ref;
  float a;
  float b;
  float c;
  float d;
  float v;
  float horiz;
  float cp;
  float k;
  float dx;
  float dz;
  float *reg;
  float *pp;
  const VtblEntry *slot;
  void *self;
  float axis[4];
  float rot[16];
  float mtx[16];
  float tmp[4];
  float tmp2[4];
  float tmp3[4];

  if (effect->attachMatrix != NULL || effect->attachPos != NULL) {
    target = effect->offset;
  } else {
    target = effect->pos;
  }
  do {
    cursor = 0;
    hdr = (u16 *)cmds;
    args = (float *)cmds + 1;
    iargs = (s32 *)args;
    colorDirty = false;
    switch (cmds[3]) {
    case 1: /* set draw flags and kind; create the sprite/mesh object */
      effect->drawFlags = iargs[1];
      n = 0;
      if ((u32)iargs[0] < 13) {
        n = g_gfxEffectKindMap[iargs[0]];
      }
      effect->kind = n;
      switch ((u32)n) {
      case 4:
        if (effect->attachMatrix != NULL) {
          effect->flags |= 0x10;
        }
        /* fall through */
      case 0:
      case 2:
      case 3:
      case 10:
        effect->texture = effect->mgr->textures[iargs[2]];
        effect->flags |= 0x21;
        if (g_gfxEffectAltMeshOnce != 0) {
          CoreObjectUnlink(&effect->base);
          GfxSpriteLayerAdd(g_worldSpriteLayer, &effect->base);
          effect->flags |= 0x80000000;
          g_gfxEffectAltMeshOnce = 0;
        }
        break;
      case 5:
      case 6:
      case 7:
      case 8:
      case 11:
      case 12:
        if (effect->meshObj != NULL) {
          break;
        }
        effect->texture = effect->mgr->textures[iargs[2]];
        if (g_gfxEffectAltMeshOnce != 0) {
          g_gfxEffectAltMeshOnce = 0;
          if (effect->id < 0x171 || effect->id >= 0x181) {
            effect->meshObj = (GfxMeshObj *)GfxMeshObjCreateList1(2, effect);
          } else {
            effect->meshObj = (GfxMeshObj *)GfxMeshObjCreateList4(2, effect);
          }
        } else if (effect->id >= 0x36 && effect->id < 0x3c) {
          effect->meshObj = (GfxMeshObj *)GfxMeshObjCreateList5(2, effect);
        } else if (effect->id < 0x264 || effect->id >= 0x26a) {
          effect->meshObj = (GfxMeshObj *)GfxMeshObjCreateList0(2, effect);
        } else {
          effect->meshObj = (GfxMeshObj *)GfxMeshObjCreateList2(2, effect);
        }
        if ((effect->drawFlags & 0xa000) == 0xa000) {
          effect->meshObj->cullMode = 3;
        } else if ((effect->drawFlags & 0x2000) != 0) {
          effect->meshObj->cullMode = 2;
        } else if ((effect->drawFlags & 0x8000) != 0) {
          effect->meshObj->cullMode = 1;
        }
        effect->meshObj->geE7Arg = (effect->drawFlags & 0x4000) == 0;
        break;
      default: /* 1, 9 */
        break;
      }
      break;
    case 2: /* mesh chain: point count, physics */
      if (effect->meshObj != NULL) {
        n = READ_I() + 1;
        GfxEffectUpdateWorldPos(effect);
        GfxEffectChainInit(1.0f, &effect->meshObj->effectChain, n,
                           (const ScePspFVector4 *)effect->pos);
        mesh = effect->meshObj;
        mesh->effectChain.physics = READ_I();
        GfxEffectAllocTrailBuffer(effect, n);
      }
      break;
    case 3: /* mesh chain link lengths */
      if (effect->meshObj != NULL) {
        v = 1.0f;
        for (i = 0; i < effect->meshObj->effectChain.count; i++) {
          if (i < (s32)cmds[2]) {
            v = READ_F();
          }
          effect->meshObj->effectChain.linkLengths[i] = v;
        }
      }
      break;
    case 4: /* mesh vertex buffer, first block */
      if (effect->meshObj != NULL) {
        v = 1.0f;
        i = 0;
        vb = (float *)effect->meshObj->vertexBuffer;
        while (i < effect->meshObj->effectChain.count) {
          if (i < (s32)cmds[2]) {
            v = READ_F();
          }
          *vb = v;
          i++;
          vb++;
        }
      }
      break;
    case 5: /* mesh vertex buffer, second block */
      if (effect->meshObj != NULL) {
        i = 0;
        vb = (float *)effect->meshObj->vertexBuffer + effect->meshObj->effectChain.count;
        v = 1.0f;
        while (i < effect->meshObj->effectChain.count) {
          if (i < (s32)cmds[2]) {
            v = READ_F();
          }
          *vb = v;
          i++;
          vb++;
        }
      }
      break;
    case 202: /* mesh vertex buffer, third block */
      if (effect->meshObj != NULL) {
        i = 0;
        vb = (float *)effect->meshObj->vertexBuffer + effect->meshObj->effectChain.count * 2;
        v = 1.0f;
        while (i < effect->meshObj->effectChain.count) {
          if (i < (s32)cmds[2]) {
            v = READ_F();
          }
          *vb = v;
          i++;
          vb++;
        }
      }
      break;
    case 6:
      if (effect->meshObj != NULL) {
        mesh = effect->meshObj;
        mesh->mode = READ_I();
      }
      break;
    case 7:
      if (effect->meshObj != NULL) {
        effect->meshObj->effectChain.damping = READ_F();
        effect->meshObj->effectChain.stiffness = READ_F();
      }
      break;
    case 203:
      if (effect->meshObj != NULL) {
        if (READ_I() != 0) {
          effect->meshObj->effectChain.ready = 1;
        } else {
          effect->meshObj->effectChain.ready = 0;
        }
      }
      break;
    case 8: /* chain point i (and vertex-buffer value) */
      if (effect->meshObj != NULL) {
        i = READ_I();
        effect->meshObj->effectChain.points[i].x = READ_F();
        effect->meshObj->effectChain.points[i].y = READ_F();
        effect->meshObj->effectChain.points[i].z = READ_F();
        if (cmds[2] >= 5) {
          vb = (float *)effect->meshObj->vertexBuffer + effect->meshObj->effectChain.count;
          vb[i] = READ_F();
        }
      }
      break;
    case 206: /* chain velocity i */
      if (effect->meshObj != NULL) {
        i = READ_I();
        effect->meshObj->effectChain.velocities[i].x = READ_F();
        effect->meshObj->effectChain.velocities[i].y = READ_F();
        effect->meshObj->effectChain.velocities[i].z = READ_F();
      }
      break;
    case 200: /* mesh patch grid; chain anchored at chainAnchor, zeroed from the bank's C720 */
      if (effect->meshObj != NULL) {
        i = READ_I();
        j = READ_I();
        n = j + 1;
        if (effect->kind == 11 || effect->kind == 12) {
          n = i * j;
        }
        mesh = effect->meshObj;
        mesh->patchCountU = i;
        mesh->patchCountV = j;
        /* sv.q C720 (bank zero vector): 16 bytes, so the w lane clears `emitter` (+0x1cc) too */
        effect->chainAnchor[0] = 0.0f;
        effect->chainAnchor[1] = 0.0f;
        effect->chainAnchor[2] = 0.0f;
        effect->emitter = NULL;
        GfxEffectChainInit(1.0f, &effect->meshObj->effectChain, n,
                           (const ScePspFVector4 *)effect->chainAnchor);
        effect->meshObj->effectChain.physics = 1;
        GfxEffectAllocTrailBuffer(effect, n);
        effect->meshObj->effectChain.ready = 0;
      }
      break;
    case 201:
      if (effect->meshObj != NULL) {
        i = READ_I();
        j = READ_I();
        mesh = effect->meshObj;
        mesh->patchDivS = i & 0xff;
        mesh->patchDivT = j & 0xff;
      }
      break;
    case 204: /* create a mesh object of the given type */
      effect->flags &= ~1u;
      n = READ_I();
      switch ((u32)n) {
      case 0:
        effect->meshObj = (GfxMeshObj *)GfxMeshObjCreateList2(3, effect);
        break;
      case 1:
        effect->meshObj = (GfxMeshObj *)GfxMeshObjCreateList0(3, effect);
        break;
      case 2:
        effect->meshObj = (GfxMeshObj *)GfxMeshObjCreateList2(6, effect);
        break;
      case 3:
        effect->meshObj = (GfxMeshObj *)GfxMeshObjCreateList0(6, effect);
        break;
      case 4:
        effect->meshObj = (GfxMeshObj *)GfxMeshObjCreateList2(6, effect);
        GfxMeshObjRunStateDefaultVertices(effect->meshObj);
        break;
      case 5:
        effect->meshObj = (GfxMeshObj *)GfxMeshObjCreateList0(6, effect);
        GfxMeshObjRunStateDefaultVertices(effect->meshObj);
        break;
      default:
        break;
      }
      break;
    case 205: /* miscellaneous sub-commands */
      n = READ_I();
      switch ((u32)n) {
      case 0: /* camera shake */
        a = READ_F();
        b = READ_F();
        c = READ_F();
        if (a == 0.0f) {
          GfxCameraStartShake(6.0f, 0.6f, g_gfxActiveCamera, 20);
        } else {
          GfxCameraStartShake(b, c * 0.5f, g_gfxActiveCamera, (s32)a * 2);
        }
        break;
      case 1: /* dust puffs */
        a = READ_F();
        b = READ_F();
        c = READ_F();
        if (a == 0.0f) {
          GfxPuffSpawnDustRingOriented(0.0f, b, c, target, effect->dir, &g_vecUp.x);
        } else if (a == 1.0f) {
          GfxPuffSpawnDustRing(target);
        } else if (a == 2.0f) {
          GfxPuffSpawnDustBurst(target, effect->dir, &g_vecUp.x);
        }
        break;
      case 2:
        g_gfxEffectAltMeshOnce = 1;
        break;
      case 3:
        i = GfxEffectReturnMinus1();
        if (i >= 0) {
          effect->mgr->scriptFlags[i] = 1;
        }
        break;
      case 4:
        i = GfxEffectReturnMinus1();
        if (i >= 0) {
          effect->mgr->scriptFlags[i] = 0;
        }
        break;
      case 6:
        if (g_gfxEffectHitFlag != 0) {
          yield = 1;
          effect->key++;
        }
        break;
      case 7:
        if (g_gfxEffectHitFlag != 0) {
          g_gfxEffectHitFlag = 0;
          yield = 1;
          effect->key++;
        }
        break;
      case 8:
      case 14:
      case 19:
        yield = 1;
        effect->key++;
        break;
      case 9:
        GfxSpriteSetQuadMode((GfxSprite *)effect, 4);
        break;
      case 10: /* owner virtual slot 6 with a float */
        if (effect->ownerBakugan != NULL) {
          owner = (BtlBakugan *)effect->ownerBakugan;
          slot = &((const VtblEntry *)owner->base.base.vtable)[6];
          self = (u8 *)owner + slot->delta;
          a = READ_F();
          ((void (*)(void *, float))slot->fn)(self, a);
        }
        break;
      case 11:
        a = READ_F();
        b = READ_F();
        c = READ_F();
        GfxEffectPushBakugans(a, b, c, effect);
        break;
      case 12:
        effect->flags |= 0x80;
        break;
      case 13:
        if (effect->meshObj != NULL) {
          effect->meshObj->extraCmd = 0xc0000302;
        }
        break;
      case 15: /* attach a shared effect model */
        a = READ_F();
        effect->flags &= ~1u;
        effect->model = g_gfxEffectModels[(s32)a];
        break;
      case 17: /* hit test with the stored volume */
        i = (s32)READ_F();
        j = (s32)READ_F();
        GfxEffectHitTest(g_gfxEffectHitRadius, g_gfxEffectHitLength, effect, i, j,
                         g_gfxEffectHitShape);
        break;
      case 18: /* hit volume shape, radius, length */
        g_gfxEffectHitShape = (s32)READ_F();
        g_gfxEffectHitRadius = READ_F();
        if (g_gfxEffectHitShape != 0) {
          g_gfxEffectHitLength = READ_F();
        }
        break;
      case 20: /* positional sound */
        hi = (s32)READ_F();
        lo = (s32)READ_F();
        GfxEffectUpdateWorldPos(effect);
        lo = (s32)((u32)lo + ((u32)hi << 20));
        if (SndHasListener()) {
          effect->emitter = SndEmitterCreateAtPos(SndGetListener(), lo, effect->pos, 0, 1);
        }
        break;
      case 21: /* rate-driven positional sounds */
        hi = (s32)READ_F();
        lo = (s32)READ_F();
        GfxEffectUpdateWorldPos(effect);
        effect->f1f8 = effect->f1f8 + effect->f1f4;
        if (!(effect->f1f8 <= 32.0f)) {
          effect->f1f8 = 32.0f;
        }
        if (effect->f1f8 < 1.0f) {
          break;
        }
        do {
          lo = (s32)((u32)lo + ((u32)hi << 20));
          if (SndHasListener()) {
            SndEmitterCreateAtPos(SndGetListener(), lo, effect->pos, 0, 1);
          }
          v = effect->f1f8 - 1.0f;
          effect->f1f8 = v;
        } while (!(v < 1.0f));
        break;
      case 22: /* player charge zone */
        a = READ_F();
        b = READ_F();
        c = READ_F();
        player = (ActorPlayer *)ActorFindPlayer();
        if (player == NULL) {
          break;
        }
        /* horizontal distance: vsub.q then vzero.s S011 clears the y lane before vdot.t */
        pp = player->base.base.pos;
        dx = pp[0] - effect->pos[0];
        dz = pp[2] - effect->pos[2];
        k = __builtin_sqrtf(dx * dx + dz * dz);
        if (k <= a) {
          player->inChargeZone = 1;
          effect->size[1] = effect->size[1] + (b - 0.5f) / c;
          if (!(effect->size[1] < b)) {
            effect->size[1] = b;
          }
        } else {
          effect->size[1] = effect->size[1] - (b - 0.5f) / c;
          if (effect->size[1] <= 0.5f) {
            effect->size[1] = 0.5f;
          }
        }
        break;
      case 23: /* camera-relative position, scaled to a length */
        if (effect->attachMatrix != NULL) {
          GfxEffectRunCommandsSub3(target, &effect->attachMatrix[12], g_gfxActiveCamera->eye);
        } else if (effect->attachPos != NULL) {
          GfxEffectRunCommandsSub3(target, effect->attachPos, g_gfxActiveCamera->eye);
        } else {
          GfxEffectRunCommandsSub3(target, target, g_gfxActiveCamera->eye);
        }
        d = 1.0f;
        if (effect->ownerBakugan != NULL &&
            ((BtlBakugan *)effect->ownerBakugan)->base.base.unk08 < 0x23) {
          d = ((BtlBakugan *)effect->ownerBakugan)->combat.stats->colliderRadius * 0.0125f;
        }
        a = READ_F() * d;
        GfxEffectRunCommandsSetLength(target, a);
        break;
      case 24:
        if (SndHasListener()) {
          SndEmitterDestroy(SndGetListener(), effect->emitter);
        }
        break;
      default: /* 5, 16 */
        break;
      }
      break;
    case 250:
      GfxEffectUpdateWorldPos(effect);
      effect->attachPos = NULL;
      effect->attachMatrix = NULL;
      break;
    case 10:
      effect->color[0] = READ_F();
      effect->color[1] = READ_F();
      effect->color[2] = READ_F();
      effect->color[3] = READ_F();
      colorDirty = true;
      break;
    case 11:
    case 12:
    case 13:
    case 14:
      effect->color[cmds[3] - 11] = READ_F();
      colorDirty = true;
      break;
    case 15:
      switch (iargs[0]) {
      case 1:
        effect->blendMode = 1;
        break;
      case 2:
        effect->blendMode = 2;
        break;
      case 3:
        effect->blendMode = 3;
        break;
      default:
        effect->blendMode = 0;
        break;
      }
      break;
    case 16:
      effect->texture = effect->mgr->textures[iargs[0]];
      break;
    case 17:
      effect->color[0] = effect->color[0] + READ_F();
      effect->color[1] = effect->color[1] + READ_F();
      effect->color[2] = effect->color[2] + READ_F();
      effect->color[3] = effect->color[3] + READ_F();
      colorDirty = true;
      break;
    case 18:
    case 19:
    case 20:
    case 21:
      v = READ_F();
      effect->color[cmds[3] - 18] = effect->color[cmds[3] - 18] + v;
      colorDirty = true;
      break;
    case 24: /* colour lower bounds */
      a = READ_F();
      b = READ_F();
      c = READ_F();
      d = READ_F();
      if (effect->color[0] < a) {
        effect->color[0] = a;
      }
      if (effect->color[1] < b) {
        effect->color[1] = b;
      }
      if (effect->color[2] < c) {
        effect->color[2] = c;
      }
      if (effect->color[3] < d) {
        effect->color[3] = d;
      }
      colorDirty = true;
      break;
    case 25:
    case 26:
    case 27:
    case 28:
      v = READ_F();
      if (effect->color[cmds[3] - 25] < v) {
        effect->color[cmds[3] - 25] = v;
      }
      colorDirty = true;
      break;
    case 29: /* colour upper bounds */
      a = READ_F();
      b = READ_F();
      c = READ_F();
      d = READ_F();
      if (!(effect->color[0] <= a)) {
        effect->color[0] = a;
      }
      if (!(effect->color[1] <= b)) {
        effect->color[1] = b;
      }
      if (!(effect->color[2] <= c)) {
        effect->color[2] = c;
      }
      if (!(effect->color[3] <= d)) {
        effect->color[3] = d;
      }
      colorDirty = true;
      break;
    case 30:
    case 31:
    case 32:
    case 33:
      v = READ_F();
      if (!(effect->color[cmds[3] - 30] <= v)) {
        effect->color[cmds[3] - 30] = v;
      }
      colorDirty = true;
      break;
    case 34:
      effect->textureSlot = READ_I();
      break;
    case 47: /* colour from HSV */
      a = READ_F();
      b = READ_F();
      c = READ_F();
      d = READ_F();
      GfxHsvToRgb(a, b, c, d, effect->color);
      colorDirty = true;
      break;
    case 48: /* step through the texture list */
      i = GfxTextureGetIndex(effect->texture);
      a = READ_F();
      effect->texture = GfxTextureGetByIndex(i + (s32)a);
      break;
    case 35: /* UV / mesh scale */
      a = READ_F();
      b = a;
      if (cmds[2] >= 2) {
        b = READ_F();
      }
      if (effect->meshObj != NULL) {
        mesh = effect->meshObj;
        mesh->scaleA = a;
        mesh->scaleB = b;
      } else {
        if (effect->quadMode != 4) {
          GfxSpriteSetQuadMode((GfxSprite *)effect, 3);
        }
        effect->uvScale[0] = a;
        effect->uvScale[1] = b;
      }
      break;
    case 36:
      a = READ_F();
      b = a;
      if (cmds[2] >= 2) {
        b = READ_F();
      }
      if (effect->meshObj != NULL) {
        effect->meshObj->scaleA = effect->meshObj->scaleA + a;
        effect->meshObj->scaleB = effect->meshObj->scaleB + b;
      } else {
        effect->uvScale[0] = effect->uvScale[0] + a;
        effect->uvScale[1] = effect->uvScale[1] + b;
      }
      break;
    case 37:
      a = READ_F();
      b = a;
      if (cmds[2] >= 2) {
        b = READ_F();
      }
      if (effect->meshObj != NULL) {
        effect->meshObj->scaleA = effect->meshObj->scaleA * a;
        effect->meshObj->scaleB = effect->meshObj->scaleB * b;
      } else {
        effect->uvScale[0] = effect->uvScale[0] * a;
        effect->uvScale[1] = effect->uvScale[1] * b;
      }
      break;
    case 38:
      a = READ_F();
      b = a;
      if (cmds[2] >= 2) {
        b = READ_F();
      }
      if (effect->meshObj != NULL) {
        if (!(effect->meshObj->scaleA <= a)) {
          effect->meshObj->scaleA = a;
        }
        if (!(effect->meshObj->scaleB <= b)) {
          effect->meshObj->scaleB = b;
        }
      } else {
        if (!(effect->uvScale[0] <= a)) {
          effect->uvScale[0] = a;
        }
        if (!(effect->uvScale[1] <= b)) {
          effect->uvScale[1] = b;
        }
      }
      break;
    case 39:
      a = READ_F();
      b = a;
      if (cmds[2] >= 2) {
        b = READ_F();
      }
      if (effect->meshObj != NULL) {
        if (effect->meshObj->scaleA < a) {
          effect->meshObj->scaleA = a;
        }
        if (effect->meshObj->scaleB < b) {
          effect->meshObj->scaleB = b;
        }
      } else {
        if (effect->uvScale[0] < a) {
          effect->uvScale[0] = a;
        }
        if (effect->uvScale[1] < b) {
          effect->uvScale[1] = b;
        }
      }
      break;
    case 40: /* UV / mesh texture offset */
      a = READ_F();
      b = a;
      if (cmds[2] >= 2) {
        b = READ_F();
      }
      if (effect->meshObj != NULL) {
        mesh = effect->meshObj;
        mesh->texOffsetU = a;
        mesh->texOffsetV = b;
      } else {
        effect->uvOffset[0] = a;
        effect->uvOffset[1] = b;
      }
      break;
    case 41: /* scroll the texture offset, wrapped into (-10, 10) */
      a = READ_F();
      b = a;
      if (cmds[2] >= 2) {
        b = READ_F();
      }
      if (effect->meshObj != NULL) {
        a = effect->meshObj->texOffsetU + a;
        b = effect->meshObj->texOffsetV + b;
      } else {
        a = effect->uvOffset[0] + a;
        b = effect->uvOffset[1] + b;
      }
      if (!(a < 10.0f)) {
        a = a - 10.0f;
      } else if (a <= -10.0f) {
        a = a + 10.0f;
      }
      if (!(b < 10.0f)) {
        b = b - 10.0f;
      } else if (b <= -10.0f) {
        b = b + 10.0f;
      }
      if (effect->meshObj != NULL) {
        effect->meshObj->texOffsetU = a;
        effect->meshObj->texOffsetV = b;
      } else {
        effect->uvOffset[0] = a;
        effect->uvOffset[1] = b;
      }
      break;
    case 42:
      a = READ_F();
      b = a;
      if (cmds[2] >= 2) {
        b = READ_F();
      }
      if (effect->meshObj != NULL) {
        effect->meshObj->texOffsetU = effect->meshObj->texOffsetU * a;
        effect->meshObj->texOffsetV = effect->meshObj->texOffsetV * b;
      } else {
        effect->uvOffset[0] = effect->uvOffset[0] * a;
        effect->uvOffset[1] = effect->uvOffset[1] * b;
      }
      break;
    case 43:
      a = READ_F();
      b = a;
      if (cmds[2] >= 2) {
        b = READ_F();
      }
      if (effect->meshObj != NULL) {
        if (!(effect->meshObj->texOffsetU <= a)) {
          effect->meshObj->texOffsetU = a;
        }
        if (!(effect->meshObj->texOffsetV <= b)) {
          effect->meshObj->texOffsetV = b;
        }
      } else {
        if (!(effect->uvOffset[0] <= a)) {
          effect->uvOffset[0] = a;
        }
        if (!(effect->uvOffset[1] <= b)) {
          effect->uvOffset[1] = b;
        }
      }
      break;
    case 44:
      a = READ_F();
      b = a;
      if (cmds[2] >= 2) {
        b = READ_F();
      }
      if (effect->meshObj != NULL) {
        if (effect->meshObj->texOffsetU < a) {
          effect->meshObj->texOffsetU = a;
        }
        if (effect->meshObj->texOffsetV < b) {
          effect->meshObj->texOffsetV = b;
        }
      } else {
        if (effect->uvOffset[0] < a) {
          effect->uvOffset[0] = a;
        }
        if (effect->uvOffset[1] < b) {
          effect->uvOffset[1] = b;
        }
      }
      break;
    case 45: /* stencil test; ref 0x20 means the owner's stencil reference */
    case 46: /* stencil write */
      ref = (u32)READ_I();
      if (effect->ownerBakugan != NULL && ref == 0x20) {
        owner = (BtlBakugan *)effect->ownerBakugan;
        if (owner->base.base.unk08 < 0x23) {
          ref = (u32)owner->stencilRef;
        } else {
          ref = owner->flags;
        }
      }
      ref &= 0xff;
      if (cmds[3] == 45) {
        GfxSpriteSetStencilTest((GfxSprite *)effect, true, (u8)ref);
        if (effect->meshObj != NULL) {
          GfxMeshObjSetStencilTest(effect->meshObj, true, ref);
        }
      } else {
        GfxSpriteSetStencilWrite((GfxSprite *)effect, true, (u8)ref);
        if (effect->meshObj != NULL) {
          GfxMeshObjSetStencilWrite(effect->meshObj, true, ref);
        }
      }
      break;
    case 50: /* size */
      effect->size[0] = READ_F();
      if (cmds[2] < 3) {
        v = effect->size[0];
        effect->size[2] = v;
        effect->size[1] = v;
      } else {
        effect->size[1] = READ_F();
        effect->size[2] = READ_F();
      }
      break;
    case 51:
    case 52:
    case 53:
      effect->size[cmds[3] - 51] = READ_F();
      break;
    case 54:
      v = READ_F();
      effect->size[0] = effect->size[0] + v;
      if (cmds[2] < 3) {
        effect->size[1] = effect->size[1] + v;
        effect->size[2] = effect->size[2] + v;
      } else {
        effect->size[1] = effect->size[1] + READ_F();
        effect->size[2] = effect->size[2] + READ_F();
      }
      break;
    case 55:
    case 56:
    case 57:
      v = READ_F();
      effect->size[cmds[3] - 55] = effect->size[cmds[3] - 55] + v;
      break;
    case 60:
      a = READ_F();
      b = READ_F();
      c = READ_F();
      if (effect->size[0] < a) {
        effect->size[0] = a;
      }
      if (effect->size[1] < b) {
        effect->size[1] = b;
      }
      if (effect->size[2] < c) {
        effect->size[2] = c;
      }
      break;
    case 61:
    case 62:
    case 63:
      v = READ_F();
      if (effect->size[cmds[3] - 61] < v) {
        effect->size[cmds[3] - 61] = v;
      }
      break;
    case 64:
      a = READ_F();
      b = READ_F();
      c = READ_F();
      if (!(effect->size[0] <= a)) {
        effect->size[0] = a;
      }
      if (!(effect->size[1] <= b)) {
        effect->size[1] = b;
      }
      if (!(effect->size[2] <= c)) {
        effect->size[2] = c;
      }
      break;
    case 65:
    case 66:
    case 67:
      v = READ_F();
      if (!(effect->size[cmds[3] - 65] <= v)) {
        effect->size[cmds[3] - 65] = v;
      }
      break;
    case 68:
      v = READ_F();
      effect->size[0] = effect->size[0] * v;
      if (cmds[2] < 3) {
        effect->size[1] = effect->size[1] * v;
        effect->size[2] = effect->size[2] * v;
      } else {
        effect->size[1] = effect->size[1] * READ_F();
        effect->size[2] = effect->size[2] * READ_F();
      }
      break;
    case 80: /* position (or attach offset) */
      target[0] = READ_F();
      target[1] = READ_F();
      target[2] = READ_F();
      break;
    case 81:
    case 82:
    case 83:
      target[cmds[3] - 81] = READ_F();
      break;
    case 84:
      target[0] = target[0] + READ_F();
      target[1] = target[1] + READ_F();
      target[2] = target[2] + READ_F();
      break;
    case 85:
    case 86:
    case 87:
      v = READ_F();
      target[cmds[3] - 85] = target[cmds[3] - 85] + v;
      break;
    case 88: /* direction */
      effect->dir[0] = READ_F();
      effect->dir[1] = READ_F();
      effect->dir[2] = READ_F();
      break;
    case 89:
      effect->dir[0] = effect->dir[0] + READ_F();
      effect->dir[1] = effect->dir[1] + READ_F();
      effect->dir[2] = effect->dir[2] + READ_F();
      break;
    case 90:
    case 91:
    case 92:
      v = READ_F();
      effect->dir[cmds[3] - 90] = effect->dir[cmds[3] - 90] + v;
      break;
    case 93:
      a = READ_F();
      b = READ_F();
      c = READ_F();
      if (effect->dir[0] < a) {
        effect->dir[0] = a;
      }
      if (effect->dir[1] < b) {
        effect->dir[1] = b;
      }
      if (effect->dir[2] < c) {
        effect->dir[2] = c;
      }
      break;
    case 94:
    case 95:
    case 96:
      v = READ_F();
      if (effect->dir[cmds[3] - 94] < v) {
        effect->dir[cmds[3] - 94] = v;
      }
      break;
    case 97:
      a = READ_F();
      b = READ_F();
      c = READ_F();
      if (!(effect->dir[0] <= a)) {
        effect->dir[0] = a;
      }
      if (!(effect->dir[1] <= b)) {
        effect->dir[1] = b;
      }
      if (!(effect->dir[2] <= c)) {
        effect->dir[2] = c;
      }
      break;
    case 98:
    case 99:
    case 100:
      v = READ_F();
      if (!(effect->dir[cmds[3] - 98] <= v)) {
        effect->dir[cmds[3] - 98] = v;
      }
      break;
    case 101:
      v = READ_F();
      effect->dir[0] = effect->dir[0] * v;
      if (cmds[2] < 3) {
        effect->dir[1] = effect->dir[1] * v;
        effect->dir[2] = effect->dir[2] * v;
      } else {
        effect->dir[1] = effect->dir[1] * READ_F();
        effect->dir[2] = effect->dir[2] * READ_F();
      }
      break;
    case 102: /* position += direction */
      target[0] = target[0] + effect->dir[0];
      target[1] = target[1] + effect->dir[1];
      target[2] = target[2] + effect->dir[2];
      break;
    case 111:
    case 112:
    case 113:
      v = READ_F();
      effect->dir[cmds[3] - 111] = effect->dir[cmds[3] - 111] * v;
      break;
    case 103: /* vec register i = (x, y, z) */
      i = READ_I();
      v = READ_F();
      p = effect->vecRegs[i];
      p[0] = v;
      p[1] = READ_F();
      p[2] = READ_F();
      break;
    case 104:
      i = READ_I();
      v = READ_F();
      p = effect->vecRegs[i];
      p[0] = p[0] + v;
      p[1] = p[1] + READ_F();
      p[2] = p[2] + READ_F();
      break;
    case 105:
      i = READ_I();
      p = effect->vecRegs[i];
      v = READ_F();
      p[0] = p[0] * v;
      p[1] = p[1] * v;
      p[2] = p[2] * v;
      p[3] = 0.0f; /* sv.q C710: lane w is the bank's S713 */
      break;
    case 106:
      i = READ_I();
      j = READ_I();
      p = effect->vecRegs[i];
      pp = effect->vecRegs[j];
      p[0] = p[0] + pp[0];
      p[1] = p[1] + pp[1];
      p[2] = p[2] + pp[2];
      break;
    case 107:
      i = READ_I();
      j = READ_I();
      p = effect->vecRegs[i];
      pp = effect->vecRegs[j];
      p[0] = pp[0];
      p[1] = pp[1];
      p[2] = pp[2];
      p[3] = pp[3];
      break;
    case 108:
      i = READ_I();
      p = effect->vecRegs[i];
      effect->dir[0] = effect->dir[0] + p[0];
      effect->dir[1] = effect->dir[1] + p[1];
      effect->dir[2] = effect->dir[2] + p[2];
      break;
    case 109:
      i = READ_I();
      p = effect->vecRegs[i];
      p[0] = g_gfxEffectCamDir[0];
      p[1] = g_gfxEffectCamDir[1];
      p[2] = g_gfxEffectCamDir[2];
      p[3] = g_gfxEffectCamDir[3];
      break;
    case 110:
      i = READ_I();
      p = effect->vecRegs[i];
      target[0] = target[0] + p[0];
      target[1] = target[1] + p[1];
      target[2] = target[2] + p[2];
      break;
    case 129:
      i = READ_I();
      p = effect->vecRegs[i];
      p[0] = effect->dir[0];
      p[1] = effect->dir[1];
      p[2] = effect->dir[2];
      p[3] = effect->dir[3];
      break;
    case 119:
      i = READ_I();
      p = effect->vecRegs[i];
      effect->dir[0] = p[0];
      effect->dir[1] = p[1];
      effect->dir[2] = p[2];
      effect->dir[3] = p[3];
      break;
    case 114: /* direction (or vec register) aimed away from the followed point, turned */
    case 115:
      if (cmds[3] == 115) {
        i = READ_I();
      }
      a = READ_F();
      b = READ_F();
      c = READ_F();
      if (target == effect->offset) {
        tmp[0] = -target[0];
        tmp[1] = -target[1];
        tmp[2] = -target[2];
      } else {
        GfxEffectRunCommandsSub3(tmp, &effect->matrix[12], target);
      }
      horiz = __builtin_sqrtf(tmp[0] * tmp[0] + tmp[2] * tmp[2]);
      b = atan2f(tmp[2], tmp[0]) + b;
      c = atan2f(tmp[1], horiz) + c;
      cp = __builtin_cosf(c);
      tmp[0] = __builtin_cosf(b) * cp;
      tmp[1] = __builtin_sinf(c);
      tmp[2] = __builtin_sinf(b) * cp;
      p = cmds[3] == 115 ? effect->vecRegs[i] : effect->dir;
      p[0] = tmp[0] * a;
      p[1] = tmp[1] * a;
      p[2] = tmp[2] * a;
      p[3] = 0.0f;
      break;
    case 116: /* rotate dir about the axis (x, y, z) by an angle */
    case 118: /* rotate vec register i about the axis (x, y, z) by an angle */
      if (cmds[3] == 118) {
        i = READ_I();
      }
      axis[0] = READ_F();
      axis[1] = READ_F();
      axis[2] = READ_F();
      d = READ_F();
      axis[3] = 0.0f;
      p = cmds[3] == 118 ? effect->vecRegs[i] : effect->dir;
      if (axis[0] == 0.0f && axis[1] == 0.0f && axis[2] == 0.0f) { /* all three +-0 (bit test) */
        axis[0] = 1.0f;
      } else {
        GfxEffectRunCommandsNormalize(axis);
      }
      GfxEffectRunCommandsQuatRotate(p, axis, d);
      break;
    case 117: /* rotate dir about vec register i by an angle */
      i = READ_I();
      d = READ_F();
      p = effect->vecRegs[i];
      if (p[0] == 0.0f && p[1] == 0.0f && p[2] == 0.0f) { /* all three +-0 (bit test) */
        effect->vecRegs[i][0] = 1.0f;
      } else {
        GfxEffectRunCommandsNormalize(p);
        p[3] = 0.0f;
      }
      GfxEffectRunCommandsQuatRotate(effect->dir, p, d);
      break;
    case 120: /* matrix = rotX(angle) * matrix */
    case 121: /* matrix = rotY(angle) * matrix */
    case 122: /* matrix = rotZ(angle) * matrix */
    case 123: /* matrix = matrix * rotX(angle) */
    case 124: /* matrix = matrix * rotY(angle) */
    case 125: /* matrix = matrix * rotZ(angle) */
      v = READ_F();
      GfxEffectRunCommandsRotation(rot, (cmds[3] - 120) % 3, v);
      if (cmds[3] < 123) {
        GfxEffectRunCommandsMatMul(mtx, rot, effect->matrix);
      } else {
        GfxEffectRunCommandsMatMul(mtx, effect->matrix, rot);
      }
      for (j = 0; j < 16; j++) {
        effect->matrix[j] = mtx[j];
      }
      break;
    case 126:
      effect->worldDir[2] = READ_F();
      break;
    case 127:
      effect->worldDir[2] = effect->worldDir[2] + READ_F();
      break;
    case 128: /* matrix = camera billboard matrix */
      pp = &g_gfxActiveCamera->billboard.x.x;
      for (j = 0; j < 16; j++) {
        effect->matrix[j] = pp[j];
      }
      break;
    case 130:
      effect->frame = READ_I();
      break;
    case 131: /* jump to key and yield */
      effect->key = READ_I();
      yield = 1;
      break;
    case 132: /* next key and yield */
      effect->key++;
      yield = 1;
      break;
    case 133:
      a = effect->color[3];
      if (a <= READ_F()) {
        effect->key++;
        yield = 1;
      }
      break;
    case 134:
      a = effect->color[3];
      if (!(a < READ_F())) {
        effect->key++;
        yield = 1;
      }
      break;
    case 135:
      i = READ_I();
      if (effect->frame >= i) {
        effect->key++;
        yield = 1;
      }
      break;
    case 136: /* destroy */
      return -1;
    case 137:
      a = effect->color[3];
      if (a <= READ_F()) {
        return -1;
      }
      break;
    case 138:
      a = effect->color[3];
      if (!(a < READ_F())) {
        return -1;
      }
      break;
    case 139:
      i = READ_I();
      if (effect->frame >= i) {
        return -1;
      }
      break;
    case 140: /* yield */
      yield = 1;
      break;
    case 141: /* register i = distance to the followed point */
      if (target == effect->offset) {
        tmp[0] = target[0];
        tmp[1] = target[1];
        tmp[2] = target[2];
      } else {
        GfxEffectRunCommandsSub3(tmp, &effect->matrix[12], target);
      }
      i = READ_I();
      effect->regs[i] = __builtin_sqrtf(tmp[0] * tmp[0] + tmp[1] * tmp[1] + tmp[2] * tmp[2]);
      break;
    case 142:
      i = READ_I();
      effect->regs[i] = READ_F();
      break;
    case 143:
      i = READ_I();
      v = READ_F();
      effect->regs[i] = effect->regs[i] + v;
      break;
    case 144:
      i = READ_I();
      a = effect->regs[i];
      if (a <= READ_F()) {
        effect->key++;
        yield = 1;
      }
      break;
    case 145:
      i = READ_I();
      a = effect->regs[i];
      if (!(a < READ_F())) {
        effect->key++;
        yield = 1;
      }
      break;
    case 146:
      i = READ_I();
      a = effect->regs[i];
      if (a <= READ_F()) {
        return -1;
      }
      break;
    case 147:
      i = READ_I();
      a = effect->regs[i];
      if (!(a < READ_F())) {
        return -1;
      }
      break;
    case 148:
      i = READ_I();
      v = READ_F();
      if (effect->regs[i] < v) {
        effect->regs[i] = v;
      }
      break;
    case 149:
      i = READ_I();
      v = READ_F();
      if (!(effect->regs[i] <= v)) {
        effect->regs[i] = v;
      }
      break;
    case 150:
      i = READ_I();
      v = READ_F();
      effect->regs[i] = effect->regs[i] * v;
      break;
    case 151: /* register i = |dir| * arg */
      i = READ_I();
      d = __builtin_sqrtf(effect->dir[0] * effect->dir[0] + effect->dir[1] * effect->dir[1] +
                          effect->dir[2] * effect->dir[2]);
      v = READ_F();
      effect->regs[i] = d * v;
      break;
    case 152:
      i = READ_I();
      v = READ_F();
      effect->regs[i] = __builtin_sinf(v);
      break;
    case 153:
      i = READ_I();
      v = READ_F();
      effect->regs[i] = __builtin_cosf(v);
      break;
    case 154:
      i = READ_I();
      effect->regs[i] = g_gfxEffectCamYaw;
      break;
    case 155:
    case 156:
    case 157:
      i = READ_I();
      effect->regs[i] = effect->dir[cmds[3] - 155];
      break;
    case 158: /* sign */
      i = READ_I();
      v = READ_F();
      if (!(v <= 0.0f)) {
        v = 1.0f;
      } else if (v < 0.0f) {
        v = -1.0f;
      } else {
        v = 0.0f;
      }
      effect->regs[i] = v;
      break;
    case 159: /* absolute value */
      i = READ_I();
      v = READ_F();
      if (v < 0.0f) {
        v = -v;
      }
      effect->regs[i] = v;
      break;
    case 210: /* floor */
      i = READ_I();
      v = READ_F();
      effect->regs[i] = (float)(s32)__builtin_floorf(v); /* floor.w.s */
      break;
    case 211: /* round */
      i = READ_I();
      v = READ_F();
      effect->regs[i] = (float)(s32)__builtin_floorf(v + 0.5f); /* floor.w.s */
      break;
    case 160:
      effect->drawFlags = iargs[0];
      effect->kind = 9;
      break;
    case 161:
      GfxEffectEmitChildren(effect, args, hdr);
      break;
    case 162:
      effect->f1f4 = READ_F();
      break;
    case 163:
      effect->f1f8 = READ_F();
      break;
    case 164: /* emit children at the rate f1f4 */
      effect->f1f8 = effect->f1f8 + effect->f1f4;
      if (!(effect->f1f8 <= 32.0f)) {
        effect->f1f8 = 32.0f;
      }
      if (effect->f1f8 < 1.0f) {
        break;
      }
      do {
        effect->f1f8 = effect->f1f8 - 1.0f;
        GfxEffectEmitChildren(effect, args, hdr);
      } while (!(effect->f1f8 < 1.0f));
      break;
    case 180: /* passes left */
      ret = READ_I();
      break;
    case 185: /* position = owner position */
      if (effect->ownerBakugan != NULL) {
        pp = ((BtlBakugan *)effect->ownerBakugan)->base.pos;
        target[0] = pp[0];
        target[1] = pp[1];
        target[2] = pp[2];
        target[3] = pp[3];
      }
      break;
    case 186:
    case 187:
    case 188:
      if (effect->ownerBakugan != NULL) {
        target[cmds[3] - 186] = ((BtlBakugan *)effect->ownerBakugan)->base.pos[cmds[3] - 186];
      }
      break;
    case 240: /* move the position towards / away from the camera eye */
      v = READ_F();
      GfxEffectRunCommandsSub3(tmp, target, g_gfxActiveCamera->eye);
      GfxEffectRunCommandsNormalize(tmp);
      target[0] = target[0] + tmp[0] * v;
      target[1] = target[1] + tmp[1] * v;
      target[2] = target[2] + tmp[2] * v;
      break;
    case 241: /* camera-relative position, scaled to a length */
      if (effect->attachMatrix != NULL) {
        GfxEffectRunCommandsSub3(target, &effect->attachMatrix[12], g_gfxActiveCamera->eye);
      } else if (effect->attachPos != NULL) {
        GfxEffectRunCommandsSub3(target, effect->attachPos, g_gfxActiveCamera->eye);
      } else {
        GfxEffectRunCommandsSub3(target, target, g_gfxActiveCamera->eye);
      }
      v = READ_F();
      GfxEffectRunCommandsSetLength(target, v);
      break;
    case 242: /* register i = heading of dir */
      i = READ_I();
      effect->regs[i] = -1.57079637f - atan2f(effect->dir[2], effect->dir[0]);
      break;
    case 243: /* dir = horizontal camera direction turned a quarter turn, normalised */
      effect->dir[0] = g_gfxActiveCamera->dir[0];
      effect->dir[1] = g_gfxActiveCamera->dir[1];
      effect->dir[2] = g_gfxActiveCamera->dir[2];
      effect->dir[3] = g_gfxActiveCamera->dir[3];
      effect->dir[1] = 0.0f;
      GfxEffectRunCommandsTurnY(effect->dir, 1.57079637f);
      GfxEffectRunCommandsNormalize(effect->dir);
      effect->dir[3] = 0.0f;
      break;
    case 244: /* vec register i = horizontal eye->position direction turned a quarter turn */
      i = READ_I();
      p = effect->vecRegs[i];
      GfxEffectRunCommandsSub3(p, target, g_gfxActiveCamera->eye);
      effect->vecRegs[i][1] = 0.0f;
      GfxEffectRunCommandsTurnY(p, 1.57079637f);
      GfxEffectRunCommandsNormalize(p);
      p[3] = 0.0f;
      break;
    case 245: /* script alpha */
      v = GfxEffectClamp01(READ_F());
      effect->f1f0 = v;
      if (effect->meshObj != NULL) {
        GfxMeshObjSetAlphaRef(effect->meshObj, (s32)(effect->f1f0 * 255.0f));
      } else {
        GfxEffectSetAlpha(effect, (s32)(effect->f1f0 * 255.0f));
      }
      break;
    case 246:
      v = READ_F();
      effect->f1f0 = GfxEffectClamp01(effect->f1f0 + v);
      if (effect->meshObj != NULL) {
        GfxMeshObjSetAlphaRef(effect->meshObj, (s32)(effect->f1f0 * 255.0f));
      } else {
        GfxEffectSetAlpha(effect, (s32)(effect->f1f0 * 255.0f));
      }
      break;
    case 247: /* matrix = basis facing the camera (up = g_vecUp) */
      GfxEffectUpdateWorldPos(effect);
      /* forward = normalised eye->position, side = up x forward, up' = forward x side */
      GfxEffectRunCommandsSub3(tmp, effect->pos, g_gfxActiveCamera->eye);
      GfxEffectRunCommandsNormalize(tmp);
      GfxEffectRunCommandsCross(tmp2, &g_vecUp.x, tmp);
      GfxEffectRunCommandsNormalize(tmp2);
      GfxEffectRunCommandsCross(tmp3, tmp, tmp2);
      /* vidt.q R003 clears the w lanes of the three axes, vidt.q C030 is the last row */
      effect->matrix[0] = tmp2[0];
      effect->matrix[1] = tmp2[1];
      effect->matrix[2] = tmp2[2];
      effect->matrix[3] = 0.0f;
      effect->matrix[4] = tmp3[0];
      effect->matrix[5] = tmp3[1];
      effect->matrix[6] = tmp3[2];
      effect->matrix[7] = 0.0f;
      effect->matrix[8] = tmp[0];
      effect->matrix[9] = tmp[1];
      effect->matrix[10] = tmp[2];
      effect->matrix[11] = 0.0f;
      effect->matrix[12] = 0.0f;
      effect->matrix[13] = 0.0f;
      effect->matrix[14] = 0.0f;
      effect->matrix[15] = 1.0f;
      break;
    case 251:
      MathMat4Identity(GfxEffectGetMatrix(effect));
      break;
    case 252: /* register i = pitch of (dir x eye->position) in the dir heading frame */
      i = READ_I();
      GfxEffectUpdateWorldPos(effect);
      tmp[0] = effect->dir[0];
      tmp[1] = effect->dir[1];
      tmp[2] = effect->dir[2];
      GfxEffectRunCommandsSub3(tmp2, effect->pos, g_gfxActiveCamera->eye);
      GfxEffectRunCommandsNormalize(tmp2);
      GfxEffectRunCommandsNormalize(tmp);
      GfxEffectRunCommandsCross(tmp3, tmp, tmp2);
      v = -atan2f(effect->dir[2], effect->dir[0]);
      GfxEffectRunCommandsTurnY(tmp3, v);
      effect->regs[i] = atan2f(tmp3[1], tmp3[2]);
      break;
    case 253: /* register i = scale * (1 - |dir . eye->position|) + bias */
      i = READ_I();
      a = 1.0f;
      b = 0.0f;
      reg = &effect->regs[i];
      if (cmds[2] >= 2) {
        a = READ_F();
      }
      if (cmds[2] >= 3) {
        b = READ_F();
      }
      GfxEffectUpdateWorldPos(effect);
      tmp[0] = effect->dir[0];
      tmp[1] = effect->dir[1];
      tmp[2] = effect->dir[2];
      GfxEffectRunCommandsSub3(tmp2, effect->pos, g_gfxActiveCamera->eye);
      GfxEffectRunCommandsNormalize(tmp2);
      GfxEffectRunCommandsNormalize(tmp);
      d = tmp[0] * tmp2[0] + tmp[1] * tmp2[1] + tmp[2] * tmp2[2];
      *reg = a * (1.0f - __builtin_fabsf(d)) + b;
      break;
    default: /* 0, 9 and unused opcodes */
      break;
    }
    if (colorDirty) {
      MathVec4Saturate(effect->color);
    }
    cmds += cmds[2] * 4 + 4;
  } while (yield == 0);
  if (runChain != 0 && effect->meshObj != NULL &&
      GfxEffectChainIsReady(GfxMeshObjGetChain(effect->meshObj)) != 0) {
    GfxEffectChainUpdate(GfxMeshObjGetChain(effect->meshObj));
  }
  return ret;
}

#undef READ_F
#undef READ_I
