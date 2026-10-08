// bdc 0x08a19dac GmoModelMeasure
#include "bdc.h"

/* Measure pass of `GmoModelLoad`: walks the children of a GMO model chunk (type 3) and reserves
   in the plan everything `GmoModelBuild` will carve.
   - bone (4): 0x20 bytes per 0x14 tag, `n * 4` (align 0x10) per 0x43 tag, `n * 2` and `n * 0x40`
     (align 0x40) per 0x44 tag, 0x10 per 0x46, 0x40 per 0x47, then `4 * (number of 0x4e tags)`;
   - part (5, remembered in `partChunk`): per mesh (6) child with draw tags (0x66/0x68/0x6a) one
     0x20 record (`GmoPlanReserveRec20`) and three pool-1 blocks sized from the draw counts, the
     tag-0x16 flag and the vertex format of the type-7 array the last draw tag indexes in the
     part; 2 bytes per 0x62 tag word, 0x10 per 0x63 tag; then one 0x30 record per mesh;
   - material (8): parsed by `GmoMaterialParseChunk`, 0x10 per 0x98 and 0x40 per 0x9a layer tag,
     then `layerCount` attribute records (`GmoPlanReserveAttrs`);
   - motion (0xb, remembered in `motionChunk`): per 0xb3 tag the data size of the fcurve (type 0xc)
     it indexes, then one 0x10 record per 0xb3 tag (`GmoPlanReserveRec10A`) and 2 bytes each;
   - 0x14: 0x20 bytes (align 0x10); 0x16: 0x60 bytes in pool 1.
   Finally the per-type arrays: nodes (one per bone), parts, materials, textures, motions plus one
   when any motion exists (`GmoPlanReserveRec30A`) and 2 bytes per bone. */

void GmoModelMeasure(void *planCtx, const void *chunk)
{
  GmoLoadCtx *ctx = (GmoLoadCtx *)planCtx;
  const GmoChunk *model = (const GmoChunk *)chunk;
  const GmoChunk *c;
  const GmoChunk *sub;
  const GmoChunk *t;
  const GmoChunk *q;
  const GmoChunk *found;
  const u8 *end;
  const u8 *subEnd;
  const u8 *tEnd;
  const u8 *qEnd;
  const s32 *data;
  u16 type;
  s32 nodeCount;
  s32 partCount;
  s32 materialCount;
  s32 textureCount;
  s32 motionCount;
  s32 meshCount;
  s32 count;
  s32 drawCount;
  s32 vertexTotal;
  s32 extra16;
  s32 index;
  s32 format;
  s32 vertexSize;
  s32 words;
  s32 size;

  end = (const u8 *)model + model->size;
  c = (const GmoChunk *)end;
  if ((s16)model->type >= 0) {
    c = (const GmoChunk *)((const u8 *)model + model->childOffset);
  }
  motionCount = 0;
  nodeCount = 0;
  partCount = 0;
  materialCount = 0;
  textureCount = 0;
  for (; (const u8 *)c < end; c = (const GmoChunk *)((const u8 *)c + c->size)) {
    type = c->type;
    switch (type & 0x7fff) {
    case 4: /* bone */
      subEnd = (const u8 *)c + c->size;
      sub = (const GmoChunk *)subEnd;
      if ((s16)type >= 0) {
        sub = (const GmoChunk *)((const u8 *)c + c->childOffset);
      }
      count = 0;
      for (; (const u8 *)sub < subEnd; sub = (const GmoChunk *)((const u8 *)sub + sub->size)) {
        type = sub->type;
        if ((s16)type < 0) {
          data = (const s32 *)&sub->childOffset; /* short header: data follows type/size */
        } else {
          data = (const s32 *)((const u8 *)sub + sub->headerSize);
        }
        switch (type & 0x7fff) {
        case 0x14:
          GmoPlanReserve(ctx->plan, 0, 0x10, 0x20);
          break;
        case 0x43:
          GmoPlanReserve(ctx->plan, 0, 0x10, data[0] * 4);
          break;
        case 0x44:
          GmoPlanReserve(ctx->plan, 0, 4, data[0] * 2);
          GmoPlanReserve(ctx->plan, 0, 0x40, data[0] * 0x40);
          break;
        case 0x46:
          GmoPlanReserve(ctx->plan, 0, 0x10, 0x10);
          break;
        case 0x47:
          GmoPlanReserve(ctx->plan, 0, 0x40, 0x40);
          break;
        case 0x4e:
          count++;
          break;
        default:
          break;
        }
      }
      GmoPlanReserve(ctx->plan, 0, (u32)__alignof__(void *), count * (int)sizeof(void *)); /* node part pointers */
      nodeCount++;
      break;

    case 5: /* part */
      ctx->partChunk = c;
      subEnd = (const u8 *)c + c->size;
      sub = (const GmoChunk *)subEnd;
      if ((s16)type >= 0) {
        sub = (const GmoChunk *)((const u8 *)c + c->childOffset);
      }
      meshCount = 0;
      for (; (const u8 *)sub < subEnd; sub = (const GmoChunk *)((const u8 *)sub + sub->size)) {
        if ((sub->type & 0x7fff) != 6) {
          continue;
        }
        /* mesh */
        tEnd = (const u8 *)sub + sub->size;
        t = (const GmoChunk *)tEnd;
        if ((s16)sub->type >= 0) {
          t = (const GmoChunk *)((const u8 *)sub + sub->childOffset);
        }
        if ((const u8 *)t < tEnd) {
          index = -1;
          drawCount = 0;
          vertexTotal = 0;
          extra16 = 0;
          for (; (const u8 *)t < tEnd; t = (const GmoChunk *)((const u8 *)t + t->size)) {
            type = t->type;
            switch (type & 0x7fff) {
            case 0x63:
              GmoPlanReserve(ctx->plan, 0, 0x10, 0x10);
              break;
            case 0x16:
              extra16 = 8;
              break;
            case 0x62:
              if ((s16)type < 0) {
                data = (const s32 *)&t->childOffset;
              } else {
                data = (const s32 *)((const u8 *)t + t->headerSize);
              }
              GmoPlanReserve(ctx->plan, 0, 4, data[0] * 2);
              break;
            case 0x66:
            case 0x68:
            case 0x6a:
              /* draw: data[0] array index (low 12 bits), data[2] * data[3] vertices, data[3]
                 primitives for 0x66, 1 otherwise */
              if ((s16)type < 0) {
                data = (const s32 *)&t->childOffset;
              } else {
                data = (const s32 *)((const u8 *)t + t->headerSize);
              }
              index = data[0] & 0xfff;
              if ((type & 0x7fff) == 0x66) {
                drawCount += data[3];
              } else {
                drawCount += 1;
              }
              vertexTotal += data[2] * data[3];
              break;
            default:
              break;
            }
          }
          if (drawCount > 0) {
            /* array chunk (type 7) number `index` of the current part; dereferenced even when
               missing, as the original does */
            q = ctx->partChunk;
            found = NULL;
            if (q != NULL) {
              qEnd = (const u8 *)q + q->size;
              q = ((s16)q->type >= 0) ? (const GmoChunk *)((const u8 *)q + q->childOffset)
                                      : (const GmoChunk *)qEnd;
              for (; (const u8 *)q < qEnd; q = (const GmoChunk *)((const u8 *)q + q->size)) {
                if ((q->type & 0x7fff) == 7) {
                  index--;
                  if (index == -1) {
                    found = q;
                    break;
                  }
                }
              }
            }
            if ((s16)found->type < 0) {
              data = (const s32 *)&found->childOffset;
            } else {
              data = (const s32 *)((const u8 *)found + found->headerSize);
            }
            format = data[0];
            /* vertex size (bits 24..31) times morph count (bits 18..20, plus one) */
            vertexSize = (format >> 24) * (((format >> 18) & 7) + 1);
            words = 4;
            if (extra16 != 0) {
              words = 10;
            }
            GmoPlanReserveRec20(1, ctx->plan);
            GmoPlanReserve(ctx->plan, 1, 4, (words + drawCount) * 4);
            GmoPlanReserve(ctx->plan, 1, 4, vertexSize * vertexTotal);
            GmoPlanReserve(ctx->plan, 1, 4, extra16 * 12);
          }
        }
        meshCount++;
      }
      partCount++;
      GmoPlanReserveRec30B(meshCount, ctx->plan);
      break;

    case 8: /* material */
      GmoMaterialParseChunk(ctx, c);
      subEnd = (const u8 *)c + c->size;
      sub = (const GmoChunk *)subEnd;
      if ((s16)c->type >= 0) {
        sub = (const GmoChunk *)((const u8 *)c + c->childOffset);
      }
      for (; (const u8 *)sub < subEnd; sub = (const GmoChunk *)((const u8 *)sub + sub->size)) {
        if ((sub->type & 0x7fff) != 9) {
          continue;
        }
        /* layer */
        tEnd = (const u8 *)sub + sub->size;
        t = (const GmoChunk *)tEnd;
        if ((s16)sub->type >= 0) {
          t = (const GmoChunk *)((const u8 *)sub + sub->childOffset);
        }
        for (; (const u8 *)t < tEnd; t = (const GmoChunk *)((const u8 *)t + t->size)) {
          if ((t->type & 0x7fff) == 0x98) {
            GmoPlanReserve(ctx->plan, 0, 0x10, 0x10);
          } else if ((t->type & 0x7fff) == 0x9a) {
            GmoPlanReserve(ctx->plan, 0, 0x40, 0x40);
          }
        }
      }
      materialCount++;
      GmoPlanReserveAttrs(ctx->layerCount, ctx->plan);
      break;

    case 10: /* texture */
      textureCount++;
      break;

    case 0xb: /* motion */
      ctx->motionChunk = c;
      subEnd = (const u8 *)c + c->size;
      sub = (const GmoChunk *)subEnd;
      if ((s16)type >= 0) {
        sub = (const GmoChunk *)((const u8 *)c + c->childOffset);
      }
      count = 0;
      for (; (const u8 *)sub < subEnd; sub = (const GmoChunk *)((const u8 *)sub + sub->size)) {
        if ((sub->type & 0x7fff) != 0xb3) {
          continue;
        }
        if ((s16)sub->type < 0) {
          data = (const s32 *)&sub->childOffset;
        } else {
          data = (const s32 *)((const u8 *)sub + sub->headerSize);
        }
        index = data[3] & 0xfff;
        /* fcurve chunk (type 0xc) number `index` of the current motion; dereferenced even when
           missing, as the original does */
        q = ctx->motionChunk;
        found = NULL;
        if (q != NULL) {
          qEnd = (const u8 *)q + q->size;
          q = ((s16)q->type >= 0) ? (const GmoChunk *)((const u8 *)q + q->childOffset)
                                  : (const GmoChunk *)qEnd;
          for (; (const u8 *)q < qEnd; q = (const GmoChunk *)((const u8 *)q + q->size)) {
            if ((q->type & 0x7fff) == 0xc) {
              index--;
              if (index == -1) {
                found = q;
                break;
              }
            }
          }
        }
        size = 0;
        if ((s16)found->type >= 0) {
          size = (s32)(found->childOffset - found->dataOffset);
        }
        GmoPlanReserve(ctx->plan, 0, 4, size);
        count++;
      }
      motionCount++;
      GmoPlanReserveRec10A(count, ctx->plan);
      GmoPlanReserve(ctx->plan, 0, 4, count * 2);
      break;

    case 0x14:
      GmoPlanReserve(ctx->plan, 0, 0x10, 0x20);
      break;

    case 0x16:
      GmoPlanReserve(ctx->plan, 1, 4, 0x60);
      break;

    default:
      break;
    }
  }

  GmoPlanReserveNodes(nodeCount, ctx->plan);
  GmoPlanReserveRec10D(partCount, ctx->plan);
  GmoPlanReserveRec10C(materialCount, ctx->plan);
  GmoPlanReserveRec10B(textureCount, ctx->plan);
  count = 0;
  if (motionCount != 0) {
    count = motionCount + 1;
  }
  GmoPlanReserveRec30A(count, ctx->plan);
  GmoPlanReserve(ctx->plan, 0, 4, nodeCount * 2);
}
