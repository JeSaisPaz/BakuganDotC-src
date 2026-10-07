// bdc 0x08825a2c GfxMeshObjDrawList
#include "bdc.h"

/* Emits GE commands for every visible mesh object (`GfxMeshObjCtor`) of the list starting at
   `first` into a chunk of render packet `packet` (`GfxPacketBeginChunk` … `GfxPacketEndChunk`);
   an empty list (`first` NULL) emits nothing. Header: light state with the active camera
   (`GfxDlWriteLightState`), a call of g_gfxMeshObjLightList, light 0 direction from the camera
   `dir`, depth test g_gfxMeshObjDepthTest and fog enable `fog`. Per object it binds the texture/CLUT
   only when they change (`GfxTextureWriteCall`), on a blend-mode change emits the blend (0 opaque,
   1 alpha, 2 additive, 3 subtractive) and fog colour (g_gfxFogParams colour for 0/1, black for
   2/3), then the world matrix (`basis` 3x3 + `pos`, 24-bit GE floats), stencil, depth test (off in
   state 7), material colour `color` (VFPU-packed; material ambient, or scene ambient when
   `emissive`), alpha test, culling, texture scale/offset, lighting and the PRIM/BEZIER/SPLINE
   command for `drawKind` (0 spline patch, 1 indexed prim, 2 bezier; cull mode 3 draws a patch
   twice with the front face flipped). Ends by resetting texture scale 1/1 and offset 0/0. */

static u32 FloatBits(float f)
{
  union { float f; u32 u; } v;
  v.f = f;
  return v.u;
}

/* GE BASE command for the top address nibble of `addr`. */
static u32 GeBase(u32 addr)
{
  return (((addr >> 24) & 0xf) << 16) | 0x10000000;
}

/* Words 12..21 (+ 22..26 for cull mode 3) of a bezier/spline patch draw; returns the list end. */
static u32 *MeshObjWritePatch(u32 *dl, const GfxMeshObj *obj, int spline)
{
  u32 iaddr;
  u32 vaddr;

  dl[12] = obj->vertexType;
  iaddr = (u32)(uintptr_t)obj->indices;
  dl[13] = GeBase(iaddr);
  iaddr = (u32)(uintptr_t)obj->indices;
  dl[14] = (iaddr & 0xffffff) | 0x02000000;
  vaddr = (u32)(uintptr_t)obj->vertices;
  dl[15] = GeBase(vaddr);
  vaddr = (u32)(uintptr_t)obj->vertices;
  dl[16] = (vaddr & 0xffffff) | 0x01000000;
  dl[17] = ((u32)obj->patchDivT << 8) | 0x36000000 | (u32)obj->patchDivS;
  dl[18] = 0x04030001;
  dl[19] = dl[13];
  dl[20] = dl[14];
  if (spline) {
    dl[21] = (u32)obj->patchCountU | ((u32)obj->splineEdgeU << 16) | ((u32)obj->patchCountV << 8) |
             (((u32)obj->splineEdgeV << 18) | 0x06000000);
  } else {
    dl[21] = ((u32)obj->patchCountV << 8) | 0x05000000 | (u32)obj->patchCountU;
  }
  if (obj->cullMode == 3) {
    dl[22] = 0x9b000000;
    dl[23] = 0x04030001;
    dl[24] = dl[13];
    dl[25] = dl[14];
    dl[26] = dl[21];
    return dl + 27;
  }
  return dl + 22;
}

void GfxMeshObjDrawList(void *packet, GfxMeshObj *first, u32 fog)
{
  GfxMeshObj *obj = first;
  BtlArenaFog *fogParams;
  GfxCamera *camera;
  u32 *dl;
  u32 lightList;
  u32 wtop;
  u32 rgba;
  u32 rgb;
  u32 alpha;
  void *curTex;
  int curClut;
  int curBlend;
  int row;
  int col;

  fog &= 0xff;
  if (obj == NULL) {
    return;
  }
  fogParams = g_gfxFogParams;
  dl = GfxPacketBeginChunk(packet);
  dl = GfxDlWriteLightState(dl, g_gfxActiveCamera, 0);
  lightList = (u32)(uintptr_t)g_gfxMeshObjLightList;
  dl[0] = GeBase(lightList);
  dl[1] = (lightList & 0xffffff) | 0x0a000000;
  dl[2] = 0x1e000000;
  camera = g_gfxActiveCamera;
  dl[3] = (FloatBits(camera->dir[0]) >> 8) | 0x63000000;
  dl[4] = (FloatBits(camera->dir[1]) >> 8) | 0x64000000;
  dl[5] = (FloatBits(camera->dir[2]) >> 8) | 0x65000000;
  dl[6] = g_gfxMeshObjDepthTest | 0x23000000;
  dl[7] = fog | 0x1f000000;
  dl += 8;

  curClut = 0;
  curTex = NULL;
  curBlend = -1;
  do {
    if (obj->visible == 0) {
      obj = (GfxMeshObj *)obj->base.next;
      continue;
    }

    if (curTex != obj->texture || curClut != obj->clut) {
      curClut = obj->clut;
      curTex = obj->texture;
      if (curTex != NULL) {
        dl = GfxTextureWriteCall(curTex, dl, curClut);
        *dl = 0x1e000001;
      } else {
        *dl = 0x1e000000;
      }
      dl++;
    }

    if (curBlend != obj->blendMode) {
      curBlend = obj->blendMode;
      switch (curBlend) {
      case 0: /* src * 1 + dst * 0 */
        dl[0] = 0xdf0000aa;
        dl[1] = 0xe0ffffff;
        dl[2] = 0xe1000000;
        dl += 3;
        break;
      case 1: /* src alpha, 1 - src alpha */
        dl[0] = 0xdf000032;
        dl[1] = 0xe0000000;
        dl[2] = 0xe1000000;
        dl += 3;
        break;
      case 2: /* src * alpha + dst */
        dl[0] = 0xdf0000a2;
        dl[1] = 0xe0000000;
        dl[2] = 0xe1ffffff;
        dl += 3;
        break;
      case 3: /* dst - src * alpha */
        dl[0] = 0xdf0002a2;
        dl[1] = 0xe0000000;
        dl[2] = 0xe1ffffff;
        dl += 3;
        break;
      default:
        break;
      }
      if (curBlend < 2) {
        *dl = (fogParams->color & 0xffffff) | 0xcf000000;
      } else {
        *dl = 0xcf000000;
      }
      dl++;
    }

    /* World matrix: rows 0..2 of `basis` (xyz) and `pos`. */
    dl[0] = 0x3a000000;
    wtop = g_geWorldDataCmdTemplate & 0xff000000;
    for (row = 0; row < 3; row++) {
      for (col = 0; col < 3; col++) {
        dl[1 + row * 3 + col] = wtop | (FloatBits(obj->basis[row * 4 + col]) >> 8);
      }
    }
    for (col = 0; col < 3; col++) {
      dl[10 + col] = wtop | (FloatBits(obj->pos[col]) >> 8);
    }
    dl += 13;
    dl[0] = obj->stencilOp;
    dl[1] = obj->stencilTest;
    dl += 2;
    if (obj->state == 7) {
      *dl = 0x23000000;
    } else {
      *dl = g_gfxMeshObjDepthTest | 0x23000000;
    }
    dl++;

    /* Material colour packed to RGBA8: clamp to [0, 1], scale by 255 (bank constant S701). */
    rgba = (u32)VfI2uc(VfF2iz(VfSat0(obj->color[0]) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(obj->color[1]) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(obj->color[2]) * 255.0f, 23)) << 16 |
           (u32)VfI2uc(VfF2iz(VfSat0(obj->color[3]) * 255.0f, 23)) << 24;
    rgb = rgba & 0xffffff;
    alpha = rgba >> 24;
    if (obj->extraCmd != 0) {
      *dl++ = obj->extraCmd;
    }
    if (obj->emissive != 0) {
      dl[0] = rgb | 0x5c000000;
      dl[1] = alpha | 0x5d000000;
    } else {
      dl[0] = rgb | 0x55000000;
      dl[1] = alpha | 0x58000000;
    }
    dl[2] = ((u32)obj->alphaRef << 8) | 0xdbff0007;
    dl[3] = (obj->cullMode != 0) | 0x1d000000;
    dl[4] = (obj->cullMode != 1) | 0x9b000000;
    dl[5] = (FloatBits(obj->texScaleU) >> 8) | 0x48000000;
    dl[6] = (FloatBits(obj->texScaleV) >> 8) | 0x49000000;
    dl[7] = (FloatBits(obj->texOffsetU) >> 8) | 0x4a000000;
    dl[8] = (FloatBits(obj->texOffsetV) >> 8) | 0x4b000000;
    dl[9] = obj->emissive | 0x17000000;
    dl[10] = ((u32)(curBlend == 0) << 1) | 0x53000000 | obj->emissive;
    dl[11] = obj->geE7Arg | 0xe7000000;

    switch (obj->drawKind) {
    case 0:
      dl = MeshObjWritePatch(dl, obj, 1);
      break;
    case 2:
      dl = MeshObjWritePatch(dl, obj, 0);
      break;
    case 1: {
      u32 prim = obj->primCmd;
      u32 vtype = obj->vertexType;
      u32 iaddr = (u32)(uintptr_t)obj->indices;
      u32 vaddr = (u32)(uintptr_t)obj->vertices;

      dl += 11; /* the 0xe7 word is overwritten */
      if (vtype != 0) {
        *dl++ = (vtype & 0xffffff) | 0x12000000;
      }
      if (iaddr != 0) {
        dl[0] = GeBase(iaddr);
        dl[1] = (iaddr & 0xffffff) | 0x02000000;
        dl += 2;
      }
      if (vaddr != 0) {
        dl[0] = GeBase(vaddr);
        dl[1] = (vaddr & 0xffffff) | 0x01000000;
        dl += 2;
      }
      *dl++ = prim | 0x04040000;
      break;
    }
    default: /* the 12 words above are left unclaimed */
      break;
    }
    if (obj->extraCmd != 0) {
      *dl++ = 0xc0000100;
    }
    obj = (GfxMeshObj *)obj->base.next;
  } while (obj != NULL);

  dl[0] = 0x483f8000; /* texture scale U 1.0 */
  dl[1] = 0x493f8000; /* texture scale V 1.0 */
  dl[2] = 0x4a000000; /* texture offset U 0.0 */
  dl[3] = 0x4b000000; /* texture offset V 0.0 */
  GfxPacketEndChunk(packet, dl + 4);
}
