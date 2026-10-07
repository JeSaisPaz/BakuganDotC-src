// bdc 0x08a18124 GmoViewMaterialCallback
#include "bdc.h"

/* One vertex of a clipped polygon: clip position (mvp scaled by the eye vector) and the barycentric
   weights of the original triangle's three vertices (lane 3 unused). */
typedef struct GmoClipVert {
  ScePspFVector4 pos;
  ScePspFVector4 wt;
} GmoClipVert;

/* vhtfm4.q with the matrix in M100 (fields as columns): m.x * x + m.y * y + m.z * z + m.w. */
static ScePspFVector4 ClipTransform(const ScePspFMatrix4 *m, float x, float y, float z)
{
  ScePspFVector4 r;

  r.x = m->x.x * x + m->y.x * y + m->z.x * z + m->w.x;
  r.y = m->x.y * x + m->y.y * y + m->z.y * z + m->w.y;
  r.z = m->x.z * x + m->y.z * y + m->z.z * z + m->w.z;
  r.w = m->x.w * x + m->y.w * y + m->z.w * z + m->w.w;
  return r;
}

/* Outcode bits of a point: 0-2 `clip[i] >= clip.w`, 3-5 `clip[i] < -clip.w`, 6-8 and 9-11 the same
   for the clip position scaled lane by lane by `eye` (vsge/vslt with a [W]/[-W] t-prefix). */
static u32 ClipOutcode(const ScePspFMatrix4 *mvp, const ScePspFVector4 *eye, float x, float y,
                       float z)
{
  ScePspFVector4 c;
  ScePspFVector4 s;
  u32 code;

  c = ClipTransform(mvp, x, y, z);
  s.x = eye->x * c.x;
  s.y = eye->y * c.y;
  s.z = eye->z * c.z;
  s.w = eye->w * c.w;
  code = 0;
  code |= (c.x >= c.w) ? 0x001 : 0;
  code |= (c.y >= c.w) ? 0x002 : 0;
  code |= (c.z >= c.w) ? 0x004 : 0;
  code |= (c.x < -c.w) ? 0x008 : 0;
  code |= (c.y < -c.w) ? 0x010 : 0;
  code |= (c.z < -c.w) ? 0x020 : 0;
  code |= (s.x >= s.w) ? 0x040 : 0;
  code |= (s.y >= s.w) ? 0x080 : 0;
  code |= (s.z >= s.w) ? 0x100 : 0;
  code |= (s.x < -s.w) ? 0x200 : 0;
  code |= (s.y < -s.w) ? 0x400 : 0;
  code |= (s.z < -s.w) ? 0x800 : 0;
  return code;
}

/* Three components of one vertex attribute: `kind` 0 s8 (vc2i + vi2f 31: /128), 1 s16 (vi2f 15:
   /32768), 2 float. */
static void LoadVec3(const u8 *p, s32 kind, float *out)
{
  s16 h;
  s32 j;

  for (j = 0; j < 3; j++) {
    if (kind == 0) {
      out[j] = (float)(s8)p[j] / 128.0f;
    } else if (kind == 1) {
      __builtin_memcpy(&h, p + j * 2, sizeof h);
      out[j] = (float)h / 32768.0f;
    } else {
      __builtin_memcpy(&out[j], p + j * 4, sizeof(float));
    }
  }
}

/* Appends `n` floats at `o` and returns the byte after them. */
static u8 *PutF32s(u8 *o, const float *v, s32 n)
{
  __builtin_memcpy(o, v, (u32)n * sizeof(float));
  return o + n * sizeof(float);
}

/* GMO material callback installed by `GmoSetViewMatrix` through `GmoSetMaterialCallback`
   (`g_gmoMaterialCallback`); `GmoDlEmitMesh` calls it with its display-list context `ctx` after
   emitting the CALL to the mesh's prebuilt primitive list. It re-emits, clipped in software, the
   triangles of the first instance of `ctx->mesh` that cross the guard band, and always ends by
   storing its write pointer back to `ctx->cur`.
   - Returns at once when `ctx->worldMatrix`, `ctx->mesh` or its first instance is NULL.
   - `mvp` = `vmmul_q_transp3`(view `g_gmoViewMatrix`, world). With a bounding box
     (`inst->state`, 8 float corners) it returns when every corner lies beyond one clip plane
     (`x >= w` or `x < -w`, per axis) or when no corner, scaled per component by
     `g_gmoEyePos`, lies beyond the scaled volume (the mesh is fully inside).
   - Walks the vertices (position format `vtype & 0x180`: s8, s16 or float) as a strip and lists
     in a 512-entry stack array every index `i >= 2` whose triangle `(i-2, i-1, i)` is not entirely
     beyond one plane and has a vertex outside the scaled volume. Returns when the list is empty.
   - Scales the columns of `mvp` by `g_gmoEyePos` and stores the GE viewport-Z command
     `0x47 | float24(``g_gmoMeshDepthCenter`` - ``g_gmoClipDepthBias``)` in
     `g_gmoClipDepthCmd`.
   - For each listed index it advances through the instance display list (`displayList`,
     `displayListWords` words; returns when it runs out) to the PRIM command (`0x0403` triangles or
     `0x0404` strip) holding the index; triangles need `rel % 3 == 2`, strips `rel >= 2` (odd `rel`
     flips the winding). The triangle (clip position + barycentric weights per vertex) is clipped
     against the six `g_gmoClipPlanes` (up to `g_gmoClipPlanesEnd`); an empty result skips it.
   - Writes the depth command, then (unless the byte count `cur - end` compares unsigned below the
     size needed) VTYPE, BASE/VADDR, a triangle-fan PRIM, BASE/JUMP past the data and the vertices:
     float UV, 8888 color, float normal and float position interpolated from the original
     triangle's attributes with the barycentric weights. */
void GmoViewMaterialCallback(GmoDlContext *ctx)
{
  ScePspFMatrix4 mvp;
  ScePspFMatrix4 mvpS;
  ScePspFVector4 eye;
  ScePspFVector4 pl;
  float mvpZw;
  u16 tris[512];
  GmoClipVert polyA[16];
  GmoClipVert polyB[16];
  GmoClipVert prevV;
  GmoClipVert curV;
  float tri[3][3];
  float uv[3][2];
  float col[3][4];
  float nrm[3][3];
  float pos[3][3];
  float v[3];
  float dPrev;
  float dCur;
  float t;
  float w0;
  float w1;
  float w2;
  union { float f; u32 u; } depth;
  u32 *cur;
  u32 *end;
  u32 *next;
  u32 *data;
  u32 *dl;
  GmoMesh *mesh;
  GmoInstance *inst;
  const float *bbox;
  const float *corner;
  const u8 *verts;
  const u8 *pos0;
  const u8 *posLast;
  const u8 *p;
  const u8 *a;
  const ScePspFVector4 *plane;
  const ScePspFVector4 *planesEnd;
  GmoClipVert *in;
  GmoClipVert *out;
  GmoClipVert *outBase;
  GmoClipVert *src;
  GmoClipVert *last;
  GmoClipVert *swap;
  GmoClipVert *dst;
  u8 *o;
  u32 vtype;
  u32 code;
  u32 prev1;
  u32 prev2;
  u32 andBits;
  u32 orBits;
  u32 tfmt;
  u32 cfmt;
  u32 nfmt;
  u32 wfmt;
  u32 pfmt;
  u32 posFmt;
  u32 colorBits;
  u32 normalBits;
  u32 room;
  u32 need;
  u32 word;
  u32 cmd;
  u32 packed;
  s32 stride;
  s32 off;
  s32 posOff;
  s32 sh;
  s32 count;
  s32 i;
  s32 j;
  s32 n;
  s32 nTris;
  s32 k;
  s32 dlWords;
  s32 base;
  s32 primCount;
  s32 rel;
  s32 triOff;
  s32 step;
  s32 nv;
  s32 kind;
  s32 texSize;
  s32 colSize;
  s32 nrmSize;

  cur = ctx->cur;
  end = ctx->end;
  if (ctx->worldMatrix == NULL) {
    goto done;
  }
  mesh = ctx->mesh;
  if (mesh == NULL) {
    goto done;
  }
  inst = mesh->instances;
  if (inst == NULL) {
    goto done;
  }
  vmmul_q_transp3(&mvp, &g_gmoViewMatrix, ctx->worldMatrix, &mvpZw);

  /* bounding-box test: all corners beyond one plane -> culled; none outside the scaled volume ->
     fully inside; either way the prebuilt list is enough */
  bbox = (const float *)inst->state;
  if (bbox != NULL) {
    eye = g_gmoEyePos;
    andBits = 0x3f; /* vone.t C200/C210 */
    orBits = 0;     /* vzero.t C220/C230 */
    for (corner = bbox; ; corner += 3) {
      code = ClipOutcode(&mvp, &eye, corner[0], corner[1], corner[2]);
      andBits &= code;
      orBits |= code;
      if (corner == bbox + 21) { /* 8 corners, 12 bytes apart */
        break;
      }
    }
    if (!((andBits & 0x3f) == 0 && (orBits & 0xfc0) != 0)) {
      goto done;
    }
  }

  /* byte offset of the position inside a vertex, computed as -2 * offset */
  vtype = inst->extra | (u32)inst->vertexSize << 24;
  wfmt = (vtype >> 9) & 3;
  tfmt = vtype & 3;
  cfmt = (vtype >> 2) & 7;
  nfmt = (vtype >> 5) & 3;
  pfmt = (vtype >> 7) & 3;
  off = 0;
  posOff = 0;
  if (wfmt != 0) {
    off = (s32)(~((vtype >> 14) & 7) << wfmt);
  }
  if (tfmt != 0) {
    posOff = off & -(1 << tfmt);
    off = posOff - (2 << tfmt);
  }
  if (cfmt != 0) {
    sh = (s32)(cfmt + 0x11) >> 3;
    posOff = off & -(1 << sh);
    off = posOff - (1 << sh);
  }
  if (nfmt != 0) {
    posOff = off & -(1 << nfmt);
    off = posOff - (3 << nfmt);
  }
  if (pfmt != 0) {
    posOff = off & -(1 << pfmt);
  }

  verts = (const u8 *)inst->vertices;
  count = inst->vertexCount;
  stride = (s32)vtype >> 24;
  pos0 = verts + ((-posOff) >> 1);
  posLast = pos0 + stride * (count - 1);
  posFmt = vtype & 0x180;

  /* list the strip triangles that need clipping: outcodes of this vertex combined with the
     previous two (M300, M400, M500; the two before the first vertex count as 0) */
  eye = g_gmoEyePos;
  prev1 = 0;
  prev2 = 0;
  n = 0;
  p = pos0;
  for (i = 0; ; i++) {
    if (posFmt == 0x100) {
      LoadVec3(p, 1, v);
    } else if (posFmt > 0x100) {
      LoadVec3(p, 2, v);
    } else {
      LoadVec3(p, 0, v);
    }
    code = ClipOutcode(&mvp, &eye, v[0], v[1], v[2]);
    andBits = code & prev1 & prev2;
    orBits = code | prev1 | prev2;
    prev2 = prev1;
    prev1 = code;
    if ((andBits & 0x3f) == 0 && (orBits & 0xfc0) != 0 && i - 2 >= 0) {
      tris[n++] = (u16)i;
      if (n == 512) {
        break;
      }
    }
    if (p == posLast) {
      break;
    }
    p += stride;
  }
  nTris = n;
  if (nTris == 0) {
    goto done;
  }

  room = (u32)((u8 *)cur - (u8 *)end);
  dl = inst->displayList;
  dlWords = inst->displayListWords;
  eye = g_gmoEyePos;
  mvpS.x.x = mvp.x.x * eye.x;
  mvpS.x.y = mvp.x.y * eye.y;
  mvpS.x.z = mvp.x.z * eye.z;
  mvpS.x.w = mvp.x.w * eye.w;
  mvpS.y.x = mvp.y.x * eye.x;
  mvpS.y.y = mvp.y.y * eye.y;
  mvpS.y.z = mvp.y.z * eye.z;
  mvpS.y.w = mvp.y.w * eye.w;
  mvpS.z.x = mvp.z.x * eye.x;
  mvpS.z.y = mvp.z.y * eye.y;
  mvpS.z.z = mvp.z.z * eye.z;
  mvpS.z.w = mvp.z.w * eye.w;
  mvpS.w.x = mvp.w.x * eye.x;
  mvpS.w.y = mvp.w.y * eye.y;
  mvpS.w.z = mvp.w.z * eye.z;
  mvpS.w.w = mvp.w.w * eye.w;
  depth.f = g_gmoMeshDepthCenter - g_gmoClipDepthBias;
  g_gmoClipDepthCmd = depth.u >> 8 | 0x47000000;

  normalBits = vtype & 0x60;
  colorBits = vtype & 0x1c;
  cmd = 0;
  primCount = 0;
  base = 0;
  for (k = 0; ; ) {
    rel = tris[k] - base;
    /* advance to the PRIM command that holds this vertex */
    while (rel >= primCount) {
      do {
        dlWords--;
        if (dlWords < 0) {
          goto done;
        }
        word = *dl++;
        cmd = word & 0xffff0000;
      } while (cmd != 0x04030000 && cmd != 0x04040000);
      rel -= primCount;
      base += primCount;
      primCount = word & 0xffff;
    }
    if (cmd == 0x04030000) {
      rel = rel % 3;
    }
    if (rel < 2) {
      goto next_tri;
    }

    /* positions of the triangle (tris[k]-2 .. tris[k]) */
    triOff = stride * (tris[k] - 2);
    kind = (posFmt == 0x80) ? 0 : (posFmt == 0x100) ? 1 : 2;
    p = pos0 + triOff;
    for (j = 0; j < 3; j++) {
      LoadVec3(p, kind, tri[j]);
      p += stride;
    }

    /* polygon A = 3 x (scaled clip position, barycentric weight); odd strip triangles reversed */
    dst = polyA;
    step = 1;
    if ((rel & 1) != 0) {
      dst = polyA + 2;
      step = -1;
    }
    for (j = 0; j < 3; j++) {
      dst->pos = ClipTransform(&mvpS, tri[j][0], tri[j][1], tri[j][2]);
      dst->wt.x = (j == 0) ? 1.0f : 0.0f; /* vmidt.t E000 */
      dst->wt.y = (j == 1) ? 1.0f : 0.0f;
      dst->wt.z = (j == 2) ? 1.0f : 0.0f;
      dst->wt.w = 0.0f; /* stale VFPU lane in the listing, never read */
      dst += step;
    }

    /* Sutherland-Hodgman against each plane; inside is dot(plane, pos) <= 0 */
    plane = g_gmoClipPlanes;
    planesEnd = g_gmoClipPlanesEnd;
    in = polyA;
    outBase = polyB;
    nv = 3;
    for (;;) {
      last = in + (nv - 1);
      out = outBase;
      pl = *plane;
      prevV = *last;
      dPrev = prevV.pos.x * pl.x + prevV.pos.y * pl.y + prevV.pos.z * pl.z + prevV.pos.w * pl.w;
      for (src = in; ; src++) {
        curV = *src;
        dCur = curV.pos.x * pl.x + curV.pos.y * pl.y + curV.pos.z * pl.z + curV.pos.w * pl.w;
        if (dPrev <= 0.0f) {
          *out++ = prevV;
        }
        if (dCur * dPrev < 0.0f) {
          t = VfRcp(dCur - dPrev) * dCur;
          out->pos.x = curV.pos.x - (curV.pos.x - prevV.pos.x) * t;
          out->pos.y = curV.pos.y - (curV.pos.y - prevV.pos.y) * t;
          out->pos.z = curV.pos.z - (curV.pos.z - prevV.pos.z) * t;
          out->pos.w = curV.pos.w - (curV.pos.w - prevV.pos.w) * t;
          out->wt.x = curV.wt.x - (curV.wt.x - prevV.wt.x) * t;
          out->wt.y = curV.wt.y - (curV.wt.y - prevV.wt.y) * t;
          out->wt.z = curV.wt.z - (curV.wt.z - prevV.wt.z) * t;
          out->wt.w = 0.0f;
          out++;
        }
        prevV = curV;
        dPrev = dCur;
        if (src == last) {
          break;
        }
      }
      nv = (s32)(out - outBase);
      plane++;
      if (nv == 0) {
        goto next_tri;
      }
      if (plane == planesEnd) {
        break;
      }
      swap = in;
      in = outBase;
      outBase = swap;
    }

    /* attributes of the original triangle (one set per vertex) */
    if (nv <= 0) {
      goto next_tri;
    }
    p = verts + triOff;
    if (tfmt != 0) {
      if (tfmt == 1) {
        a = p;
        p = a + 2 * sizeof(u8);     /* u8 u, v: vi2f 7 */
        for (j = 0; j < 3; j++) {
          uv[j][0] = (float)a[j * stride] / 128.0f;
          uv[j][1] = (float)a[j * stride + 1] / 128.0f;
        }
      } else if (tfmt == 2) {
        u16 hv[2];

        a = (const u8 *)(((uintptr_t)p + 1) & ~(uintptr_t)1);
        p = a + 2 * sizeof(u16);    /* u16 u, v: vi2f 15 */
        for (j = 0; j < 3; j++) {
          __builtin_memcpy(hv, a + j * stride, sizeof hv);
          uv[j][0] = (float)hv[0] / 32768.0f;
          uv[j][1] = (float)hv[1] / 32768.0f;
        }
      } else {
        a = (const u8 *)(((uintptr_t)p + 3) & ~(uintptr_t)3);
        p = a + 2 * sizeof(float);  /* float u, v */
        for (j = 0; j < 3; j++) {
          __builtin_memcpy(uv[j], a + j * stride, 2 * sizeof(float));
        }
      }
    }
    if (colorBits != 0) {
      if (colorBits == 0x1c) {
        a = (const u8 *)(((uintptr_t)p + 3) & ~(uintptr_t)3);
        p = a + sizeof(u32);        /* 8888 color: vuc2i + vi2f 23 */
        for (j = 0; j < 3; j++) {
          for (i = 0; i < 4; i++) {
            col[j][i] = (float)(s32)((u32)a[j * stride + i] * 0x01010101u >> 1) / 8388608.0f;
          }
        }
      } else {
        /* 16-bit colors are not decoded: 1.0 per channel (vone.q) */
        a = (const u8 *)(((uintptr_t)p + 1) & ~(uintptr_t)1);
        p = a + sizeof(u16);        /* 16-bit color */
        for (j = 0; j < 3; j++) {
          for (i = 0; i < 4; i++) {
            col[j][i] = 1.0f;
          }
        }
      }
    }
    if (normalBits != 0) {
      if (normalBits == 0x20) {
        a = p;
        p = a + 3 * sizeof(s8);     /* s8 normal */
        kind = 0;
      } else if (normalBits == 0x40) {
        a = (const u8 *)(((uintptr_t)p + 1) & ~(uintptr_t)1);
        p = a + 3 * sizeof(s16);    /* s16 normal */
        kind = 1;
      } else {
        a = (const u8 *)(((uintptr_t)p + 3) & ~(uintptr_t)3);
        p = a + 3 * sizeof(float); /* float normal */
        kind = 2;
      }
      for (j = 0; j < 3; j++) {
        LoadVec3(a + j * stride, kind, nrm[j]);
      }
    }
    if (posFmt != 0) {
      if (posFmt == 0x80) {
        a = p;
        kind = 0;
      } else if (posFmt == 0x100) {
        a = (const u8 *)(((uintptr_t)p + 1) & ~(uintptr_t)1);
        kind = 1;
      } else {
        a = (const u8 *)(((uintptr_t)p + 3) & ~(uintptr_t)3);
        kind = 2;
      }
      for (j = 0; j < 3; j++) {
        LoadVec3(a + j * stride, kind, pos[j]);
      }
    } else {
      /* no position in the vertex: the listing emits stale M500; not reproduced */
      for (j = 0; j < 3; j++) {
        pos[j][0] = 0.0f;
        pos[j][1] = 0.0f;
        pos[j][2] = 0.0f;
      }
    }

    /* emitted vertex: float UV (8), color 8888 (4), float normal (12), float position (12) */
    texSize = (tfmt != 0) ? 8 : 0;
    colSize = (colorBits != 0) ? 4 : 0;
    nrmSize = (normalBits != 0) ? 0xc : 0;
    need = (u32)((nrmSize + texSize + colSize + 0xc) * nv + 0x18);
    cur[0] = g_gmoClipDepthCmd;
    if (room < need) {
      next = cur + 1;
      room -= (u32)((u8 *)next - (u8 *)cur);
      cur = next;
      goto next_tri;
    }
    next = (u32 *)((u8 *)(cur + 1) + need);
    data = cur + 7;
    cur[1] = ((nrmSize != 0) ? 0x60 : 0) | ((colSize != 0) ? 0x1c : 0) |
             ((texSize != 0) ? 3 : 0) | 0x12000180;                          /* VTYPE */
    cur[2] = (((u32)(uintptr_t)data >> 24) & 0xf) << 16 | 0x10000000;       /* BASE */
    cur[3] = ((u32)(uintptr_t)data & 0xffffff) | 0x01000000;                /* VADDR */
    cur[4] = (u32)nv | 0x04050000;                                           /* PRIM fan */
    cur[5] = (((u32)(uintptr_t)next >> 24) & 0xf) << 16 | 0x10000000;       /* BASE */
    cur[6] = ((u32)(uintptr_t)next & 0xffffff) | 0x08000000;                /* JUMP */
    o = (u8 *)data;
    src = outBase; /* the last plane's output */
    do {
      w0 = src->wt.x;
      w1 = src->wt.y;
      w2 = src->wt.z;
      if (texSize != 0) {
        for (i = 0; i < 2; i++) {
          v[i] = uv[0][i] * w0 + uv[1][i] * w1 + uv[2][i] * w2;
        }
        o = PutF32s(o, v, 2);
      }
      if (colSize != 0) {
        packed = 0;
        for (i = 0; i < 4; i++) {
          t = col[0][i] * w0 + col[1][i] * w1 + col[2][i] * w2;
          packed |= (u32)VfI2uc(VfF2iz(t, 23)) << (i * 8);
        }
        __builtin_memcpy(o, &packed, sizeof packed);
        o += sizeof packed;
      }
      if (nrmSize != 0) {
        for (i = 0; i < 3; i++) {
          v[i] = nrm[0][i] * w0 + nrm[1][i] * w1 + nrm[2][i] * w2;
        }
        o = PutF32s(o, v, 3);
      }
      for (i = 0; i < 3; i++) {
        v[i] = pos[0][i] * w0 + pos[1][i] * w1 + pos[2][i] * w2;
      }
      o = PutF32s(o, v, 3);
      src++;
    } while (o != (u8 *)next);
    room -= (u32)((u8 *)next - (u8 *)cur);
    cur = next;

  next_tri:
    k++;
    if (k == nTris) {
      break;
    }
  }

done:
  ctx->cur = cur;
}
