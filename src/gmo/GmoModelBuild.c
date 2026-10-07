// bdc 0x08a1a744 GmoModelBuild
#include "bdc.h"

/* Build pass of `GmoModelLoad` (`planCtx` is the `GmoLoadCtx` whose plan `GmoModelMeasure`
   sized). Counts the node (4), part (5), material (8), texture (10) and motion (0xb) children of
   the model chunk (all 0 for a NULL chunk), carves the arrays (`GmoPlanTakeNodes`,
   `GmoPlanTakeRec10D`, `GmoPlanTakeRec10C`, `GmoPlanTakeRec10B`, `GmoPlanTakeMotions`;
   one extra motion record when any motion exists) into `model`, stores `model` in the context,
   then fills them child by child:
   - node (4): `parts` gets one slot per 0x4e tag (part index, low 12 bits), `visible` the mask
     of that many bits; 0x41 parent index, 0x42 visible, 0x43 morph weights, 0x44 bone node
     indices (`block10`), 0x45 bind matrices, 0x46 `block38`, 0x47 explicit matrix, 0x48
     translate, 0x49/0x4a Euler rotation (`GmoQuatFromEulerA`/`GmoQuatFromEulerB`), 0x4b
     quaternion, 0x4c/0x4d/0xe1 scale, 0x4f morph position, 0xe2 draw group, 0x14 bounding box
     (`block34`). A skinned node gets `boneCount` inverse bind matrices (`block14`; identity past
     the 0x45 data) and flag 0x20000, others flag 0x10000;
   - part (5, remembered in `partChunk`): one `GmoMesh` per type-6 child. A mesh with draw tags
     (0x66/0x68/0x6a) gets one `GmoInstance` whose GE display list holds an optional bounding
     box test (0x16 tag: 8 float vertices, BBOX + BJUMP to the final RET), VTYPE/BASE/VADDR of
     its private vertex buffer, one PRIM/BEZIER/SPLINE command per draw and RET; the vertices
     each draw uses are copied from the part's type-7 array chunk the draw indexes (by index list,
     or as one run when bit 0x100 of word 1 is set). 0x61 material index, 0x62 skin data, 0x63
     patch scale, 0x6b/0x6c draw mask, 0x81 enable bits;
   - material (8): `GmoMaterialParseChunk`, then `layerCount` `GmoAttr` records: an implicit
     base layer (0x1000), one per type-9 layer (tags 0x91..0x9a; layer types 0x82..0x86 take the
     parsed colours), implicit 0x84 emission (0x2000) and 0x86 reflection (0x4000) layers; the
     diffuse (0x82) or implicit base layer gets ambient, specular and specular power;
   - texture (10): an embedded file (0x13) is loaded into a new texture (`GmoTextureCreate`,
     `GmoTextureLoad`, released on failure) and its name (0x12) passed to
     `GmoHookReleaseTexture`; with no loaded texture the name is looked up
     (`GmoHookFindTextureByName`) and shared (`GmoTextureShare`, flags 2);
   - motion (0xb, remembered in `motionChunk`): one `GmoMotionTrack` per 0xb3 tag with a copy of
     the 0xc f-curve chunk it indexes, a zeroed cursor table, 0xb1 frame range, 0xb2 rate, 0xb4;
   - 0x14 model bounding box, 0x15 uv transform (3) / scale (0x180), 0x16 8-vertex bounding box.
   Finally takes `nodeCount` draw-order entries and calls `GmoModelFinalize` and
   `GmoModelInitBaseMotion`. */

/* chunk walking: bit 15 of `type` marks a short (8-byte) header without children */
#define CHUNK_END(c) ((const GmoChunk *)((const u8 *)(c) + (c)->size))
#define CHUNK_FIRST(c) \
  ((s16)(c)->type < 0 ? CHUNK_END(c) : (const GmoChunk *)((const u8 *)(c) + (c)->childOffset))
#define CHUNK_ARGS(c) \
  ((s16)(c)->type < 0 ? (const s32 *)&(c)->childOffset \
                      : (const s32 *)((const u8 *)(c) + (c)->headerSize))
#define CHUNK_PAYLOAD(c) \
  ((s16)(c)->type < 0 ? (const u8 *)&(c)->childOffset : (const u8 *)(c) + (c)->dataOffset)

void GmoModelBuild(void **planCtx, const void *chunk, GmoModel *model)
{
  GmoLoadCtx *ctx = (GmoLoadCtx *)planCtx;
  const GmoChunk *root = (const GmoChunk *)chunk;
  const GmoChunk *c;
  const GmoChunk *sub;
  const GmoChunk *t;
  const GmoChunk *q;
  const GmoChunk *arr;
  const GmoChunk *found;
  const s32 *data;
  const s32 *args;
  const s32 *bind;
  u16 type;
  s32 nodeCount;
  s32 partCount;
  s32 materialCount;
  s32 textureCount;
  s32 motionCount;
  s32 motionRecs;
  s32 n;
  s32 i;
  s32 index;
  GmoNode *node;
  GmoPart *part;
  GmoMesh *mesh;
  GmoMaterial *mat;
  GmoLayer *tex;
  GmoMotionInfo *mot;
  void **partRefs;
  float *buf;
  float *m;
  const float *src;
  u16 *list;
  float one;
  float euler[4] __attribute__((aligned(16)));
  union { float f; u32 u; } bits;

  nodeCount = 0;
  partCount = 0;
  materialCount = 0;
  textureCount = 0;
  motionCount = 0;
  if (root != NULL) {
    for (c = CHUNK_FIRST(root); c < CHUNK_END(root); c = CHUNK_END(c)) {
      switch (c->type & 0x7fff) {
      case 4:  nodeCount++; break;
      case 5:  partCount++; break;
      case 8:  materialCount++; break;
      case 10: textureCount++; break;
      case 11: motionCount++; break;
      default: break;
      }
    }
  }
  motionRecs = 0;
  if (motionCount != 0) {
    motionRecs = motionCount + 1;
  }

  node = GmoPlanTakeNodes(nodeCount, ctx->plan);
  part = GmoPlanTakeRec10D(partCount, ctx->plan);
  mat = GmoPlanTakeRec10C(materialCount, ctx->plan);
  tex = GmoPlanTakeRec10B(textureCount, ctx->plan);
  mot = GmoPlanTakeMotions(motionRecs, ctx->plan);
  model->nodeCount = nodeCount;
  model->partCount = partCount;
  model->materialCount = materialCount;
  model->textureCount = textureCount;
  model->motionCount = motionCount;
  model->nodes = node;
  model->parts = part;
  model->materials = mat;
  model->textures = tex;
  model->motions = mot;
  ctx->model = model;

  for (c = CHUNK_FIRST(root); c < CHUNK_END(root); c = CHUNK_END(c)) {
    data = CHUNK_ARGS(c);
    switch (c->type & 0x7fff) {

    case 4: { /* node */
      n = 0;
      for (t = CHUNK_FIRST(c); t < CHUNK_END(c); t = CHUNK_END(t)) {
        if ((t->type & 0x7fff) == 0x4e) {
          n++;
        }
      }
      partRefs = GmoPlanTake(ctx->plan, 0, 4, n * 4);
      node->visible = (1 << n) - 1;
      node->partCount = n;
      node->parts = partRefs;
      bind = NULL;
      for (t = CHUNK_FIRST(c); t < CHUNK_END(c); t = CHUNK_END(t)) {
        args = CHUNK_ARGS(t);
        switch (t->type & 0x7fff) {
        case 0x14: /* bounding box: min, max */
          buf = GmoPlanTake(ctx->plan, 0, 0x10, 0x20);
          GmoVec3Copy(buf, (const float *)args);
          GmoVec3Copy(buf + 4, (const float *)(args + 3));
          node->block34 = buf;
          break;
        case 0x41:
          node->parentIndex = args[0] & 0xfff;
          break;
        case 0x42:
          node->visible = args[0];
          break;
        case 0x43: /* morph weights */
          buf = GmoPlanTake(ctx->plan, 0, 0x10, args[0] * 4);
          memcpy(buf, args + 1, args[0] * 4);
          node->morphCount = args[0];
          node->flags |= 0x40000;
          node->morphWeights = buf;
          break;
        case 0x44: /* bone node indices */
          list = GmoPlanTake(ctx->plan, 0, 4, args[0] * 2);
          node->block10 = list;
          node->boneCount = args[0];
          for (i = 0; i < args[0]; i++) {
            list[i] = args[1 + i] & 0xfff;
          }
          break;
        case 0x45: /* bind matrices: count, then 4x4 matrices */
          bind = args;
          break;
        case 0x46:
          buf = GmoPlanTake(ctx->plan, 0, 0x10, 0x10);
          GmoVec3Copy(buf, (const float *)args);
          node->block38 = buf;
          node->flags42 |= 0x80;
          break;
        case 0x47: /* explicit matrix */
          buf = GmoPlanTake(ctx->plan, 0, 0x40, 0x40);
          GmoMatrixCopy(buf, (const float *)args);
          node->matrix = buf;
          node->flags42 |= 0x10;
          break;
        case 0x48:
          GmoVec3Copy(node->translate, (const float *)args);
          break;
        case 0x49:
          GmoVec3Copy(euler, (const float *)args);
          GmoQuatFromEulerA(node->rotate, euler);
          break;
        case 0x4a:
          GmoVec3Copy(euler, (const float *)args);
          GmoQuatFromEulerB(node->rotate, euler);
          break;
        case 0x4b:
          GmoVec4CopyB(node->rotate, (const float *)args);
          break;
        case 0x4c:
          GmoVec3Copy(node->scale, (const float *)args);
          break;
        case 0x4d:
          GmoVec3Copy(node->scale, (const float *)args);
          node->flags42 |= 0x20;
          break;
        case 0x4e: /* part index (left as an index, see GmoDlDrawNode) */
          *partRefs = (void *)(uintptr_t)(args[0] & 0xfff);
          partRefs++;
          break;
        case 0x4f:
          node->morphPos = ((const float *)args)[0];
          node->flags |= 0x40000;
          break;
        case 0xe1:
          GmoVec3Copy(node->scale, (const float *)args);
          node->flags42 |= 0x40;
          break;
        case 0xe2:
          if (args[0] == 0) {
            node->drawGroup = (args[1] != 0);
          }
          break;
        default:
          break;
        }
      }
      if (node->boneCount == 0) {
        node->flags |= 0x10000;
      } else {
        m = GmoPlanTake(ctx->plan, 0, 0x40, node->boneCount * 0x40);
        node->block14 = m;
        if (bind == NULL) {
          one = g_gmoBuildUnitFloat;
          for (i = 0; i < node->boneCount; i++) {
            GmoMatrixScaleTranslate(0.0f, 0.0f, 0.0f, one, one, one, m);
            m += 16;
          }
        } else {
          one = g_gmoBuildUnitFloat;
          src = (const float *)(bind + 1);
          for (i = 0; i < node->boneCount; i++) {
            if (i < bind[0]) {
              GmoMatrixCopy(m, src);
            } else {
              GmoMatrixScaleTranslate(0.0f, 0.0f, 0.0f, one, one, one, m);
            }
            src += 16;
            m += 16;
          }
        }
        node->flags |= 0x20000;
      }
      node++;
      break;
    }

    case 5: { /* part */
      GmoPart *nextPart = part + 1;

      n = 0;
      for (t = CHUNK_FIRST(c); t < CHUNK_END(c); t = CHUNK_END(t)) {
        if ((t->type & 0x7fff) == 6) {
          n++;
        }
      }
      mesh = GmoPlanTakeRec30B(n, ctx->plan);
      part->meshCount = n;
      part->meshes = mesh;
      ctx->partChunk = c;
      for (sub = CHUNK_FIRST(c); sub < CHUNK_END(c); sub = CHUNK_END(sub)) {
        s32 drawCount;
        s32 vertexTotal;
        s32 bboxCount;
        s32 words;
        s32 format;
        s32 vertexSize;
        const u8 *arrayData;
        GmoInstance *inst;
        u32 *dl;
        u32 *bbox;
        u32 *cmd;
        u32 *out;
        u32 *ret;
        u8 *verts;

        if ((sub->type & 0x7fff) != 6) {
          continue;
        }
        /* mesh: first pass sizes the draws */
        index = -1;
        drawCount = 0;
        vertexTotal = 0;
        bboxCount = 0;
        for (t = CHUNK_FIRST(sub); t < CHUNK_END(sub); t = CHUNK_END(t)) {
          args = CHUNK_ARGS(t);
          type = t->type & 0x7fff;
          switch (type) {
          case 0x16:
            bboxCount = 8;
            break;
          case 0x61:
            mesh->materialIndex = args[0] & 0xfff;
            break;
          case 0x62: /* skin data */
            list = GmoPlanTake(ctx->plan, 0, 4, args[0] * 2);
            mesh->skinData = list;
            mesh->skinCount = args[0];
            for (i = 0; i < args[0]; i++) {
              list[i] = args[1 + i];
            }
            mesh->flags |= 0x20000;
            break;
          case 0x63: /* patch scale */
            buf = GmoPlanTake(ctx->plan, 0, 0x10, 0x10);
            GmoVec2Set(((const float *)args)[0], ((const float *)args)[1], buf);
            mesh->patchScale = buf;
            mesh->flags |= 0x80000;
            break;
          case 0x66:
          case 0x68:
          case 0x6a: /* draw: array index, primitive, vertices per primitive, primitives */
            index = args[0] & 0xfff;
            if (type == 0x66) {
              vertexTotal += args[2] * args[3];
              drawCount += args[3];
            } else {
              vertexTotal += args[2] * args[3];
              drawCount += 1;
            }
            break;
          case 0x6b:
            mesh->drawMask = (mesh->drawMask & 0xffff0000) | (u16)args[0];
            break;
          case 0x6c: {
            u32 hi = 0x10000u << args[1];

            mesh->drawMask = (mesh->drawMask & 0xffff) |
                             ((hi | (hi - 1)) & -(0x10000u << args[0]) & 0xffff0000);
            break;
          }
          case 0x81: {
            u16 bit = (u16)(1 << args[0]);

            mesh->enableMask |= bit;
            mesh->enableBits = (mesh->enableBits & ~bit) | (args[1] != 0 ? bit : 0);
            mesh->flags |= 0xffff;
            break;
          }
          default:
            break;
          }
        }
        if (drawCount > 0) {
          ctx->arrayIndex = index;
          ctx->vertexTotal = vertexTotal;
          ctx->bboxVertexCount = bboxCount;
          ctx->drawCount = drawCount;
          inst = GmoPlanTakeRec20(1, ctx->plan);
          mesh->instances = inst;
          index = ctx->arrayIndex;
          drawCount = ctx->drawCount;
          vertexTotal = ctx->vertexTotal;
          bboxCount = ctx->bboxVertexCount;
          /* type-7 array chunk number `index` of the part; dereferenced even when missing, as
             the original does */
          found = NULL;
          q = ctx->partChunk;
          if (q != NULL) {
            for (arr = CHUNK_FIRST(q); arr < CHUNK_END(q); arr = CHUNK_END(arr)) {
              if ((arr->type & 0x7fff) == 7) {
                index--;
                if (index == -1) {
                  found = arr;
                  break;
                }
              }
            }
          }
          arrayData = CHUNK_PAYLOAD(found);
          format = CHUNK_ARGS(found)[0];
          /* vertex size (bits 24..31) times morph count (bits 18..20, plus one) */
          vertexSize = (format >> 24) * (((format >> 18) & 7) + 1);
          words = 4;
          if (bboxCount != 0) {
            words = 10;
          }
          words += drawCount;
          dl = GmoPlanTake(ctx->plan, 1, 4, words * 4);
          bbox = GmoPlanTake(ctx->plan, 1, 4, bboxCount * 12);
          verts = GmoPlanTake(ctx->plan, 1, 4, vertexSize * vertexTotal);
          inst->displayListWords = words;
          inst->vertexCount = vertexTotal;
          inst->displayList = dl;
          inst->vertices = verts;
          inst->state = bbox;
          inst->extra = format & 0xffefff;
          inst->vertexSize = vertexSize;
          cmd = dl;
          if (bboxCount > 0) {
            ret = &dl[words - 1];
            cmd[0] = 0x12000180; /* VTYPE: float positions */
            cmd[1] = 0x10000000 | (u32)(((uintptr_t)bbox >> 24) & 0xf) << 16; /* BASE */
            cmd[2] = 0x01000000 | (u32)((uintptr_t)bbox & 0xffffff);          /* VADDR */
            cmd[3] = 0x07000008;                                              /* BBOX 8 */
            cmd[4] = 0x10000000 | (u32)(((uintptr_t)ret >> 24) & 0xf) << 16;  /* BASE */
            cmd[5] = 0x09000000 | (u32)((uintptr_t)ret & 0xffffff);           /* BJUMP */
            cmd += 6;
          }
          cmd[0] = (format & 0xffefff) | 0x12000000;                         /* VTYPE */
          cmd[1] = 0x10000000 | (u32)(((uintptr_t)verts >> 24) & 0xf) << 16; /* BASE */
          cmd[2] = 0x01000000 | (u32)((uintptr_t)verts & 0xffffff);          /* VADDR */
          cmd += 3;
          /* second pass: draw commands and their vertices */
          for (t = CHUNK_FIRST(sub); t < CHUNK_END(sub); t = CHUNK_END(t)) {
            args = CHUNK_ARGS(t);
            out = cmd;
            switch (t->type & 0x7fff) {
            case 0x66: /* PRIM, one per primitive */
              for (i = 0; i < args[3]; i++) {
                *out++ = args[2] | (args[1] & 0xf) << 16 | 0x04000000;
              }
              break;
            case 0x68: /* SPLINE */
              *out++ = ((args[1] >> 14) & 3) << 18 | ((args[1] >> 12) & 3) << 16 |
                       args[3] << 8 | args[2] | 0x06000000;
              break;
            case 0x6a: /* BEZIER */
              *out++ = args[3] << 8 | args[2] | 0x05000000;
              break;
            case 0x16:
              if (args[0] == 8) {
                for (i = 0; i < 24; i++) {
                  bbox[i] = args[1 + i];
                }
              }
              break;
            default:
              break;
            }
            if (cmd < out) {
              s32 count;
              size_t size;
              const u16 *indices;

              index = args[0] & 0xfff;
              if (index != ctx->arrayIndex) {
                found = NULL;
                q = ctx->partChunk;
                if (q != NULL) {
                  s32 left = index;

                  for (arr = CHUNK_FIRST(q); arr < CHUNK_END(q); arr = CHUNK_END(arr)) {
                    if ((arr->type & 0x7fff) == 7) {
                      left--;
                      if (left == -1) {
                        found = arr;
                        break;
                      }
                    }
                  }
                }
                arrayData = CHUNK_PAYLOAD(found);
                ctx->arrayIndex = index;
              }
              count = args[2] * args[3];
              if ((args[1] & 0x100) != 0) {
                /* one run of `count` vertices from the first index */
                size = vertexSize * count;
                count = 1;
              } else {
                size = vertexSize;
              }
              indices = (const u16 *)(args + 4);
              for (i = 0; i < count; i++) {
                memcpy(verts, arrayData + vertexSize * indices[i], size);
                verts += size;
              }
            }
            cmd = out;
          }
          *cmd = 0x0b000000; /* RET */
          mesh->data = inst->displayList;
          GmoHeapAddRef(1, inst->displayList);
        }
        mesh++;
      }
      part = nextPart;
      break;
    }

    case 8: { /* material */
      GmoMaterial *nextMat;
      GmoAttr *attr;
      GmoAttr *baseAttr;
      const GmoChunk *layer;

      GmoMaterialParseChunk(ctx, c);
      mat->flags02 |= (u16)ctx->layerFlags;
      attr = GmoPlanTakeAttrs(ctx->layerCount, ctx->plan);
      mat->attrs = attr;
      nextMat = mat + 1;
      mat->attrCount = ctx->layerCount;
      baseAttr = NULL;
      if ((ctx->layerFlags & 0x1000) != 0) {
        /* implicit base layer */
        attr->diffuse = ctx->diffuse;
        attr->layerFlags16 = ctx->stateValues;
        attr->layerFlags14 = ctx->stateMask;
        if ((u16)ctx->stateMask != 0) {
          attr->flags |= 0xffff;
        }
        baseAttr = attr;
        attr++;
      }
      for (layer = CHUNK_FIRST(c); layer < CHUNK_END(c); layer = CHUNK_END(layer)) {
        if ((layer->type & 0x7fff) != 9) {
          continue;
        }
        attr->layerFlags16 = ctx->stateValues;
        attr->layerFlags14 = ctx->stateMask;
        for (t = CHUNK_FIRST(layer); t < CHUNK_END(layer); t = CHUNK_END(t)) {
          args = CHUNK_ARGS(t);
          switch (t->type & 0x7fff) {
          case 0x91:
            attr->layerRef = args[0] & 0xfff;
            break;
          case 0x92:
            attr->type = args[0];
            break;
          case 0x93:
            break;
          case 0x94: /* blend op, source, destination */
            attr->blendOp = args[0];
            attr->blendSrc = args[1];
            attr->blendDst = args[2];
            if (!(args[0] == 0 && args[1] == 6 && args[2] == 7)) {
              attr->flags |= 0x200000;
            }
            break;
          case 0x95:
            attr->layerFlags14 |= 0xc00;
            if (args[0] == 0) {
              attr->layerFlags16 |= 0x400;
            }
            if (args[1] == 1) {
              attr->layerFlags16 |= 0x800;
            }
            break;
          case 0x96:
            attr->layerFlags14 |= 0x3000;
            if (args[0] > 0) {
              attr->layerFlags16 |= 0x1000;
            }
            if (args[1] >= 4) {
              attr->layerFlags16 |= 0x2000;
            }
            break;
          case 0x97:
            attr->layerFlags14 |= 0xc000;
            if (args[0] == 0) {
              attr->layerFlags16 |= 0x4000;
            }
            if (args[1] == 0) {
              attr->layerFlags16 |= 0x8000;
            }
            break;
          case 0x98: { /* uv transform; flagged unless (0, 0, 1, 1) */
            float *uv = GmoPlanTake(ctx->plan, 0, 0x10, 0x10);

            GmoVec4CopyC(uv, (const float *)args);
            attr->data = uv;
            if (!(uv[0] == 0.0f && uv[1] == 0.0f && uv[2] == g_gmoBuildUnitFloat &&
                  uv[3] == g_gmoBuildUnitFloat)) {
              attr->flags |= 0x800000;
            }
            break;
          }
          case 0x99:
            attr->flags |= 0x1000000;
            attr->texMapMode = args[0];
            if ((args[0] & 0xf00) != 0) {
              attr->flags |= 0x3000000;
            }
            break;
          case 0x9a:
            buf = GmoPlanTake(ctx->plan, 0, 0x40, 0x40);
            GmoMatrixCopy(buf, (const float *)args);
            attr->block = buf;
            attr->flags |= 0x3000000;
            break;
          default:
            break;
          }
        }
        type = attr->type;
        switch (type) {
        case 0x82: /* diffuse */
          attr->diffuse = ctx->diffuse;
          ctx->diffuse = 0;
          break;
        case 0x83: /* specular */
          attr->blendDst = 1;
          attr->specular = ctx->specular;
          ctx->specular = 0;
          bits.f = ctx->specularPower;
          attr->word3c = bits.u;
          attr->flags |= 0x400000;
          break;
        case 0x84: /* emission */
          attr->layerFlags14 |= 1;
          attr->emission = ctx->emission;
          attr->blendDst = 1;
          ctx->emission = 0;
          attr->layerFlags16 &= ~1;
          break;
        case 0x85: /* ambient */
          attr->blendDst = 1;
          attr->emission = ctx->ambient;
          ctx->ambient = 0;
          break;
        case 0x86: /* reflection */
          GmoColorToRgba8(g_gmoBuildUnitFloat, g_gmoBuildUnitFloat, g_gmoBuildUnitFloat,
                          ctx->reflection, &attr->emission);
          attr->flags |= 0x1000000;
          attr->layerFlags14 |= 1;
          attr->layerFlags16 &= ~1;
          ctx->reflection = 0.0f;
          break;
        default:
          break;
        }
        if (attr->layerFlags14 != 0) {
          attr->flags |= 0xffff;
        }
        attr->flags |= 0x100000;
        if (type == 0x82) {
          baseAttr = attr;
        }
        attr++;
      }
      if ((ctx->layerFlags & 0x2000) != 0) {
        /* implicit emission layer */
        attr->blendSrc = 1;
        attr->emission = ctx->emission;
        attr->flags |= 0x20ffff;
        attr->type = 0x84;
        attr->layerFlags14 = ctx->stateMask | 1;
        attr->layerFlags16 = ctx->stateValues & ~1;
        attr++;
      }
      if ((ctx->layerFlags & 0x4000) != 0) {
        /* implicit reflection layer */
        attr->flags |= 0xffff;
        attr->type = 0x86;
        attr->layerFlags14 = ctx->stateMask | 1;
        attr->layerFlags16 = ctx->stateValues & ~1;
      }
      mat = nextMat;
      if (baseAttr != NULL) {
        baseAttr->emission = ctx->ambient;
        bits.f = ctx->specularPower;
        baseAttr->word3c = bits.u;
        baseAttr->specular = ctx->specular;
      }
      break;
    }

    case 10: { /* texture */
      GmoLayer *nextTex = tex + 1;
      const char *name = NULL;
      const s32 *file;
      u32 size;
      void *img;

      t = CHUNK_FIRST(c);
      if (t < CHUNK_END(c)) {
        file = NULL;
        size = 0;
        for (; t < CHUNK_END(c); t = CHUNK_END(t)) {
          args = CHUNK_ARGS(t);
          if ((t->type & 0x7fff) == 0x12) {
            name = (const char *)args;
          } else if ((t->type & 0x7fff) == 0x13) {
            size = args[0];
            file = args + 1;
          }
        }
        if (file != NULL) {
          img = GmoTextureCreate();
          if (GmoTextureLoad(img, file, size, 0) != 0) {
            tex->texture = img;
            GmoHookReleaseTexture(name);
          } else {
            GmoTextureRelease(img);
          }
        }
      }
      if (tex->texture == NULL) {
        img = GmoHookFindTextureByName(name);
        img = GmoTextureShare(img, 2);
        tex->texture = img;
      }
      tex = nextTex;
      break;
    }

    case 11: { /* motion */
      GmoMotionInfo *nextMot = mot + 1;
      GmoMotionTrack *track;
      u16 *cursors;

      n = 0;
      for (t = CHUNK_FIRST(c); t < CHUNK_END(c); t = CHUNK_END(t)) {
        if ((t->type & 0x7fff) == 0xb3) {
          n++;
        }
      }
      track = GmoPlanTakeRec10A(n, ctx->plan);
      cursors = GmoPlanTake(ctx->plan, 0, 4, n * 2);
      memset(cursors, 0, n * 2);
      mot->trackCount = n;
      mot->table = cursors;
      mot->tracks = track;
      ctx->motionChunk = c;
      for (t = CHUNK_FIRST(c); t < CHUNK_END(c); t = CHUNK_END(t)) {
        args = CHUNK_ARGS(t);
        switch (t->type & 0x7fff) {
        case 0xb1:
          mot->startFrame = ((const float *)args)[0];
          mot->endFrame = ((const float *)args)[1];
          break;
        case 0xb2:
          mot->frameRate = ((const float *)args)[0];
          break;
        case 0xb3: { /* track: target ref, channel, -, f-curve index */
          const s32 *params;
          size_t size;
          void *keys;
          s32 channel;
          u32 fmt;
          u16 kind;

          /* type-0xc f-curve chunk number args[3] of the motion; dereferenced even when
             missing, as the original does */
          found = NULL;
          q = ctx->motionChunk;
          index = args[3] & 0xfff;
          if (q != NULL) {
            for (arr = CHUNK_FIRST(q); arr < CHUNK_END(q); arr = CHUNK_END(arr)) {
              if ((arr->type & 0x7fff) == 0xc) {
                index--;
                if (index == -1) {
                  found = arr;
                  break;
                }
              }
            }
          }
          params = CHUNK_ARGS(found);
          size = 0;
          if ((s16)found->type >= 0) {
            size = found->childOffset - found->dataOffset;
          }
          keys = GmoPlanTake(ctx->plan, 0, 4, size);
          memcpy(keys, CHUNK_PAYLOAD(found), size);
          channel = args[1];
          fmt = params[0];
          if ((u32)(channel - 0x48) < 6 || channel == 0xe1) {
            /* translate/rotate/scale channels */
            kind = ((fmt & 0xf) != 3) | 0x100;
            if ((fmt & 0xff00) != 0) {
              kind = 0x100;
            }
          } else if ((u32)(channel - 0x42) < 6 || channel == 0x4f) {
            kind = 0x100;
          } else if ((u32)(channel - 0x82) < 7 || channel == 0x98) {
            kind = 0x200;
          } else {
            kind = 0;
          }
          track->paramC = params[1];
          track->kind |= kind;
          track->paramD = args[1];
          track->data = keys;
          track->ref = args[0] & 0xfff;
          track->param8 = fmt;
          track->paramA = params[2];
          track++;
          break;
        }
        case 0xb4:
          mot->value0e = args[0];
          break;
        default:
          break;
        }
      }
      mot = nextMot;
      break;
    }

    case 0x14: /* model bounding box: min, max */
      buf = GmoPlanTake(ctx->plan, 0, 0x10, 0x20);
      GmoVec3Copy(buf, (const float *)data);
      GmoVec3Copy(buf + 4, (const float *)(data + 3));
      model->bbox = buf;
      break;

    case 0x15:
      if (data[0] == 3) {
        GmoVec4CopyC(model->uvTransform, (const float *)(data + 1));
      } else if (data[0] == 0x180) {
        GmoVec4Copy(model->scaleVec, (const float *)(data + 1));
      }
      break;

    case 0x16: /* model bounding box: 8 float vertices */
      if (data[0] == 8) {
        u32 *box = GmoPlanTake(ctx->plan, 1, 4, 0x60);

        for (i = 0; i < 24; i++) {
          box[i] = data[1 + i];
        }
        model->chunk16 = box;
      }
      break;

    default:
      break;
    }
  }

  model->drawNodes = GmoPlanTake(ctx->plan, 0, 4, nodeCount * 2);
  GmoModelFinalize(model);
  GmoModelInitBaseMotion(model);
}
