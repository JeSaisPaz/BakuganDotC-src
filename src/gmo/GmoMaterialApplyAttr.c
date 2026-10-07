// bdc 0x08a16b70 GmoMaterialApplyAttr
#include "bdc.h"

/* Applies an animated material attribute value to material `mat`. Types 0x82..0x88: finds the
   attribute record (`GmoFindAttr`); if missing, falls back to the diffuse (0x82) record or
   appends a new one (`GmoMaterialCloneAttrs`) as g_gmoAttrColorSlots says, returning if none;
   then blends the record's packed RGBA8 colour (at the table's byte offset) towards `value` (RGBA
   floats; `blend == 1` uses (1, 1, 1, value[0])) by `weight` (VFPU in the original: `vuc2i.s` +
   `vi2f.q …, 31` unpack, c + (v - c) * weight, `vsat0.q`, x255, `vf2iz.q …, 23` + `vi2uc.q`). Type 0x82 then re-applies the value as type 0x85 and clears
   bit 15 of `flags02` unless that bit was set; 0x85 sets it; 0x83 also lerps the float at `+0x3c`
   towards `value[4]` (`GmoLerpFloats`). Type 0x98 sets attribute `index`'s data block to `value`
   (`GmoMaterialSetAttrData`). Other types, or a NULL `mat`/`value`, do nothing. No VFPU value crosses
   its calls. */

void GmoMaterialApplyAttr(float weight, void *mat, s32 type, s32 index, const float *value, s32 blend)
{
  GmoMaterial *m = (GmoMaterial *)mat;
  const u8 *slot;
  GmoAttr *a;
  u32 *rgba;
  float v[4];
  float c;
  u32 word;
  u32 packed;
  s32 lane;

  if (m == NULL || value == NULL) {
    return;
  }
  if ((u32)(type - 0x82) >= 7) {
    if (type == 0x98) {
      GmoMaterialSetAttrData(m, index, (const u32 *)value);
    }
    return;
  }
  slot = g_gmoAttrColorSlots[type - 0x82];
  a = (GmoAttr *)GmoFindAttr(m, type, 0);
  if (a == NULL) {
    if (slot[1] != 0) {
      a = (GmoAttr *)GmoFindAttr(m, 0x82, 0);
    } else {
      a = (GmoAttr *)GmoMaterialCloneAttrs(m, type);
    }
    if (a == NULL) {
      return;
    }
  }
  rgba = (u32 *)((u8 *)a + slot[0]);
  if (blend == 1) {
    v[0] = 1.0f;
    v[1] = 1.0f;
    v[2] = 1.0f;
    v[3] = value[0];
  } else {
    v[0] = value[0];
    v[1] = value[1];
    v[2] = value[2];
    v[3] = value[3];
  }
  word = *rgba;
  packed = 0;
  for (lane = 0; lane < 4; lane++) {
    c = (float)(s32)(((word >> (lane * 8)) & 0xffu) * 0x01010101u >> 1) / 2147483648.0f;
    c = c + (v[lane] - c) * weight;
    packed |= (u32)VfI2uc(VfF2iz(VfSat0(c) * 255.0f, 23)) << (lane * 8);
  }
  *rgba = packed;
  if (type == 0x82) {
    if ((m->flags02 & 0x8000) == 0) {
      GmoMaterialApplyAttr(weight, m, 0x85, index, value, blend);
      m->flags02 &= 0x7fff;
    }
  } else if (type == 0x85) {
    m->flags02 |= 0x8000;
  } else if (type == 0x83) {
    GmoLerpFloats(weight, (float *)&a->word3c, (const float *)&a->word3c, value + 4, 1);
  }
}
