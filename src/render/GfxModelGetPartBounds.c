// bdc 0x089e1ccc GfxModelGetPartBounds
#include "bdc.h"

/* Finds the `partIndex`-th part chunk (type 5) among the children of the first model chunk
   (`GmoGetFirstModel`) of the model's GMO file and, for every type-`0x14` (bounding box) child of
   that part, copies two 3-float corners from the part's own payload (8 bytes past its data) into
   `out` as two vec4s (`out[0..2]`, `out[4..6]`, `out[3]`/`out[7]` cleared). Returns true when the
   part exists, false otherwise. */

bool GfxModelGetPartBounds(GfxModel *self, s32 partIndex, float *out)
{
  const GmoChunk *model;
  const GmoChunk *part;
  const GmoChunk *child;
  const u8 *end;
  const u8 *partEnd;
  const float *data;
  s32 index;

  model = (const GmoChunk *)GmoGetFirstModel(self->gmo);
  end = (const u8 *)model + model->size;
  if ((model->type & 0x8000) != 0) {
    part = (const GmoChunk *)end;
  } else {
    part = (const GmoChunk *)((const u8 *)model + model->childOffset);
  }
  index = 0;
  for (; (const u8 *)part < end; part = (const GmoChunk *)partEnd) {
    partEnd = (const u8 *)part + part->size;
    if ((part->type & 0x7fff) != 5) {
      continue;
    }
    if (index++ != partIndex) {
      continue;
    }
    if ((part->type & 0x8000) != 0) {
      child = (const GmoChunk *)partEnd;
    } else {
      child = (const GmoChunk *)((const u8 *)part + part->childOffset);
    }
    for (; (const u8 *)child < partEnd;
         child = (const GmoChunk *)((const u8 *)child + child->size)) {
      if ((child->type & 0x7fff) != 0x14) {
        continue;
      }
      /* data of the part itself (short header: right after type/size); the corners start at
         its third word */
      if ((part->type & 0x8000) != 0) {
        data = (const float *)&part->childOffset;
      } else {
        data = (const float *)((const u8 *)part + part->headerSize);
      }
      memcpy(out, &data[2], 0xc);
      memcpy(out + 4, &data[5], 0xc);
      out[7] = 0.0f;
      out[3] = 0.0f;
    }
    return true;
  }
  return false;
}
