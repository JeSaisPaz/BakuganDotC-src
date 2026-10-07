// bdc 0x089e1a18 GfxModelIndexChunks
#include "bdc.h"

/* Indexes the children of the first model chunk (`GmoGetFirstModel`) of the GMO file at
   `self->gmo`: counts the node (4), part (5), material (8) and motion (0xb) chunks with
   `GmoChunkCount` into nodeCount/partCount/materialCount/motionCount; when there are materials,
   allocates (low heap) a zeroed 16-byte-per-material state table into materialStates (left
   untouched otherwise); then allocates the pointer table `chunks` and fills it with the payload
   pointers (`chunk + 0x10`) of all chunks of those four types in file order, storing for each type
   the address of its first slot in nodeChunks/partChunks/materialChunks/motionChunks (NULL when the
   type is absent, except motionChunks, which then points past the last filled slot). */

void GfxModelIndexChunks(GfxModel *self)
{
  const GmoChunk *model;
  const GmoChunk *end;
  const GmoChunk *p;
  bool wasLow;
  void *states;
  void **table;
  bool haveNode = false;
  bool havePart = false;
  bool haveMaterial = false;
  bool haveMotion = false;
  s32 n = 0;

  model = (const GmoChunk *)GmoGetFirstModel((const GmoFile *)self->gmo);
  self->nodeCount = GmoChunkCount(model, 4);
  self->partCount = GmoChunkCount(model, 5);
  self->materialCount = GmoChunkCount(model, 8);
  self->motionCount = GmoChunkCount(model, 0xb);

  if (self->materialCount > 0) {
    s32 size = self->materialCount * sizeof(GfxMaterialState);
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    states = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    self->materialStates = states;
    memset(states, 0, self->materialCount * sizeof(GfxMaterialState));
  }

  {
    s32 size = (self->nodeCount + self->partCount + self->materialCount + self->motionCount) * sizeof(void *);
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    table = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
  }
  self->chunks = table;
  self->nodeChunks = NULL;
  self->partChunks = NULL;
  self->materialChunks = NULL;
  self->motionChunks = NULL;

  end = (const GmoChunk *)((const u8 *)model + model->size);
  if (model->type & 0x8000) {
    p = end;
  } else {
    p = (const GmoChunk *)((const u8 *)model + model->childOffset);
  }
  while (p < end) {
    void *payload = (void *)(p + 1);

    switch (p->type & 0x7fff) {
    case 4:
      self->chunks[n] = payload;
      if (!haveNode) {
        self->nodeChunks = &self->chunks[n];
        haveNode = true;
      }
      n++;
      break;
    case 5:
      self->chunks[n] = payload;
      if (!havePart) {
        self->partChunks = &self->chunks[n];
        havePart = true;
      }
      n++;
      break;
    case 8:
      self->chunks[n] = payload;
      if (!haveMaterial) {
        self->materialChunks = &self->chunks[n];
        haveMaterial = true;
      }
      n++;
      break;
    case 0xb:
      self->chunks[n] = payload;
      if (!haveMotion) {
        self->motionChunks = &self->chunks[n];
        haveMotion = true;
      }
      n++;
      break;
    default:
      break;
    }
    p = (const GmoChunk *)((const u8 *)p + p->size);
  }
  if (!haveMotion) {
    self->motionChunks = self->chunks + n;
  }
}
