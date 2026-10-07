// bdc 0x08a1c840 GmoModelLoad
#include "bdc.h"

/* Loads model `index` of a GMO file into `model` in two passes: checks the `OMG.00.1PSP` signature
   (words `0x2e474d4f`, `0x312e3030`, `0x00505350`), finds the model chunk (type 3), measures it
   into a plan (`GmoPlanInit`, `GmoModelMeasure`), commits the plan (`GmoPlanCommit`) and
   builds (`GmoModelLoadBuild`), then frees the plan (`GmoPlanFree`). Returns the build result
   on success, 0 for a NULL model or file, `size` 1..0x1f, a bad signature, a missing model chunk
   or a failed commit. */

s32 GmoModelLoad(GmoModel *self, const void *gmo, u32 size, s32 index)

{
  GmoImagePlan *planCtx[18];
  GmoImagePlan plan;
  const GmoFileHeader *hdr;
  const GmoChunk *root;
  const GmoChunk *end;
  const GmoChunk *chunk;
  s32 left;
  s32 result;

  GmoPlanInit(&plan);
  hdr = (const GmoFileHeader *)gmo;
  if (self == NULL || hdr == NULL || size - 1 < 0x1f) {
    return 0;
  }
  if (hdr->magic != 0x2e474d4f || hdr->version != 0x312e3030 || hdr->platform != 0x505350) {
    return 0;
  }
  root = &hdr->root;
  if (root == NULL) {
    return 0;
  }
  end = (const GmoChunk *)((const u8 *)root + root->size);
  chunk = end;
  if ((s16)root->type >= 0) {
    chunk = (const GmoChunk *)((const u8 *)root + root->childOffset);
  }
  left = index;
  for (; chunk < end; chunk = (const GmoChunk *)((const u8 *)chunk + chunk->size)) {
    if ((chunk->type & 0x7fff) == 3) {
      left--;
      if (left == -1) {
        planCtx[0] = &plan;
        GmoModelMeasure(planCtx, chunk);
        if (GmoPlanCommit((int *)&plan) == 0) {
          return 0;
        }
        result = GmoModelLoadBuild(self, hdr, size, index, &plan);
        GmoPlanFree(&plan);
        return result;
      }
    }
  }
  return 0;
}
