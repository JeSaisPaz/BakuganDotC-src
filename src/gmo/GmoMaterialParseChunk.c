// bdc 0x08a19a14 GmoMaterialParseChunk
#include "bdc.h"

/* Parses a GMO material chunk (type 8) into the material fields of the load context (both passes
   of `GmoModelLoad`): resets them, then walks the child tags — `0x81` render-state bit (int bit
   index, int enable: `stateMask` gets the bit, `stateValues` gets it set or cleared), `0x82`
   diffuse (RGBA8), `0x83` specular (RGB, alpha cleared; power float data[4]), `0x84` emission
   (RGB, alpha cleared), `0x85` ambient (flag 0x8000), `0x86` reflection float, and layer chunks
   (9: one more `layerCount`, bit `mapType - 0x82` of its `0x92` tag, or bit 0 without one, set in
   `layerFlags`). Finally: ambient defaults to diffuse, and flags 0x1000 (no bit 0), 0x2000
   (emission != 0 without bit 2) and 0x4000 (reflection > 0 without bit 4) each count one more
   layer. */

void GmoMaterialParseChunk(void *material, const void *chunk)
{
  GmoLoadCtx *ctx = (GmoLoadCtx *)material;
  const GmoChunk *mc = (const GmoChunk *)chunk;
  const GmoChunk *c;
  const GmoChunk *sub;
  const u8 *end;
  const u8 *subEnd;
  const u8 *data;
  const s32 *idata;
  u32 flags;
  u32 bit;
  u32 values;
  u32 shift;
  u16 type;

  ctx->ambient = 0xffffffff;
  ctx->layerFlags = 0;
  ctx->layerCount = 0;
  ctx->diffuse = 0xffffffff;
  ctx->specular = 0;
  ctx->emission = 0;
  ctx->stateValues = 0;
  ctx->stateMask = 0;
  ctx->specularPower = 0.0f;
  ctx->reflection = 0.0f;

  end = (const u8 *)mc + mc->size;
  c = (const GmoChunk *)end;
  if ((s16)mc->type >= 0) {
    c = (const GmoChunk *)((const u8 *)mc + mc->childOffset);
  }
  while ((const u8 *)c < end) {
    type = c->type;
    if ((s16)type < 0) {
      data = (const u8 *)&c->childOffset; /* short header: data follows type/size */
    } else {
      data = (const u8 *)c + c->headerSize;
    }
    switch (type & 0x7fff) {
    case 0x81:
      idata = (const s32 *)data;
      bit = 1u << (idata[0] & 0x1f);
      values = ctx->stateValues & ~bit;
      ctx->stateValues = values;
      ctx->stateMask = bit | ctx->stateMask;
      if (idata[1] == 0) {
        bit = 0;
      }
      ctx->stateValues = bit | values;
      break;
    case 0x82:
      GmoColorVecToRgba8(&ctx->diffuse, (const float *)data);
      break;
    case 0x83:
      GmoColorVecToRgba8(&ctx->specular, (const float *)data);
      ctx->specular &= 0x00ffffff;
      ctx->specularPower = ((const float *)data)[4];
      break;
    case 0x84:
      GmoColorVecToRgba8(&ctx->emission, (const float *)data);
      ctx->emission &= 0x00ffffff;
      break;
    case 0x85:
      GmoColorVecToRgba8(&ctx->ambient, (const float *)data);
      ctx->layerFlags |= 0x8000;
      break;
    case 0x86:
      ctx->reflection = ((const float *)data)[0];
      break;
    case 9:
      /* Map type of the layer: data word of its 0x92 child tag, minus 0x82. */
      shift = 0;
      if (c != NULL) {
        subEnd = (const u8 *)c + c->size;
        sub = (const GmoChunk *)subEnd;
        if ((s16)type >= 0) {
          sub = (const GmoChunk *)((const u8 *)c + c->childOffset);
        }
        while ((const u8 *)sub < subEnd) {
          if ((sub->type & 0x7fff) == 0x92) {
            break;
          }
          sub = (const GmoChunk *)((const u8 *)sub + sub->size);
        }
        if ((const u8 *)sub < subEnd && sub != NULL) {
          if ((s16)sub->type < 0) {
            data = (const u8 *)&sub->childOffset; /* short header */
          } else {
            data = (const u8 *)sub + sub->headerSize;
          }
          if (data != NULL) {
            shift = *(const s32 *)data - 0x82;
          }
        }
      }
      ctx->layerCount++;
      ctx->layerFlags |= 1u << (shift & 0x1f);
      break;
    default:
      break;
    }
    c = (const GmoChunk *)((const u8 *)c + c->size);
  }

  flags = ctx->layerFlags;
  if ((flags & 0x8000) == 0) {
    ctx->ambient = ctx->diffuse;
  }
  if ((flags & 1) == 0) {
    flags |= 0x1000;
    ctx->layerFlags = flags;
    ctx->layerCount++;
  }
  if ((flags & 4) == 0 && ctx->emission != 0) {
    flags |= 0x2000;
    ctx->layerFlags = flags;
    ctx->layerCount++;
  }
  if ((flags & 0x10) == 0 && 0.0f < ctx->reflection) {
    ctx->layerFlags = flags | 0x4000;
    ctx->layerCount++;
  }
}
