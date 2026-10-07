// bdc 0x08a14c08 GmoNodeDestroyContents
#include "bdc.h"

/* Releases what a 0xc0-byte node record references, without freeing the record: drops one reference
   on the parent node (a parent reaching 0 is emptied recursively and freed), then on each
   `GmoPart` in `parts` (entries that are NULL or whose `ptr + 1` has zero upper 16 bits, i.e.
   unresolved indices, are skipped; a part reaching 0 drops one reference on each of its meshes,
   emptying (`GmoMeshDestroyContents`) and freeing those reaching 0, then is freed), and finally
   frees `parts`, `morphWeights`, `block10`, `block14`, `matrix`, `block34` and `block38`. All frees
   go through `GmoHeapReleaseThunk` with pool 0. Returns `self` (NULL is a no-op). */

GmoNode *GmoNodeDestroyContents(GmoNode *self)
{
  GmoNode *parent;
  GmoPart *part;
  GmoMesh *mesh;
  u32 meshCount;
  u32 j;
  int i;

  if (self == NULL) {
    return self;
  }

  parent = self->parent;
  if (parent != NULL) {
    if (--parent->refCount == 0) {
      GmoNodeDestroyContents(parent);
      GmoHeapReleaseThunk(0, parent);
    }
  }

  for (i = 0; i < (int)self->partCount; i++) {
    part = (GmoPart *)self->parts[i];
    if ((((uintptr_t)part + 1) & 0xffff0000u) == 0 || part == NULL) {
      continue;
    }
    if (--part->refCount != 0) {
      continue;
    }
    mesh = part->meshes;
    meshCount = part->meshCount;
    if (mesh != NULL) {
      for (j = 0; j < meshCount; j++, mesh++) {
        if (--mesh->refCount == 0) {
          GmoMeshDestroyContents(mesh);
          GmoHeapReleaseThunk(0, mesh);
        }
      }
    }
    GmoHeapReleaseThunk(0, part);
  }

  GmoHeapReleaseThunk(0, self->parts);
  GmoHeapReleaseThunk(0, self->morphWeights);
  GmoHeapReleaseThunk(0, self->block10);
  GmoHeapReleaseThunk(0, self->block14);
  GmoHeapReleaseThunk(0, self->matrix);
  GmoHeapReleaseThunk(0, self->block34);
  GmoHeapReleaseThunk(0, self->block38);
  return self;
}
