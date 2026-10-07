// bdc 0x08a15300 GmoModelCreateInstances
#include "bdc.h"

/* Counterpart of `GmoModelReleaseInstances`. When `flags & 4`, for every mesh entry of every part
   of a GMO model data block that has no instance of id 1 (`GmoMeshFindInstance`) but has a shared
   instance list, creates one as a copy of the list head (`GmoInstanceArrayCopy`, flags
   `0x80000001`, from `plan`), tags it id 1 and appends it (`GmoMeshAppendInstance`); then binds the
   mesh data to the id-1 instance's display list (`GmoMeshSetData`). When `flags & 0xa`, calls
   `GmoTextureMakeWritable` on every texture with `(flags & 2 ? 1 : 0) | (flags & 8 ? 2 : 0)`.
   Returns 0 for a NULL model or when any texture call returned 0, 1 otherwise. */

int GmoModelCreateInstances(GmoModel *self, u32 flags, void *plan)
{
  int partIdx;
  int meshIdx;
  int texIdx;
  int ok;
  u32 writable;

  if (self == NULL) {
    return 0;
  }
  if ((flags & 4) != 0) {
    for (partIdx = 0; partIdx < (int)self->partCount; partIdx++) {
      GmoPart *part = &((GmoPart *)self->parts)[partIdx];
      for (meshIdx = 0; meshIdx < (int)part->meshCount; meshIdx++) {
        GmoMesh *mesh = &part->meshes[meshIdx];
        GmoInstance *inst = (GmoInstance *)GmoMeshFindInstance(mesh, 1, 0);
        if (inst == NULL) {
          GmoInstance *head = mesh->instances;
          if (head != NULL) {
            inst = (GmoInstance *)GmoInstanceArrayCopy(head, 1, 0x80000001, plan);
            inst->id = 1;
            GmoMeshAppendInstance(mesh, inst);
          }
        }
        /* NULL inst here (no id-1 instance and an empty list) is dereferenced as in the original. */
        GmoMeshSetData(mesh, inst->displayList);
      }
    }
  }
  if ((flags & 0xa) == 0 || self->textureCount == 0) {
    return 1;
  }
  writable = (flags & 2) != 0;
  ok = 1;
  if ((flags & 8) != 0) {
    for (texIdx = 0; texIdx < (int)self->textureCount; texIdx++) {
      if (GmoTextureMakeWritable(((GmoLayer *)self->textures)[texIdx].texture, writable | 2) == 0) {
        ok = 0;
      }
    }
  } else {
    for (texIdx = 0; texIdx < (int)self->textureCount; texIdx++) {
      if (GmoTextureMakeWritable(((GmoLayer *)self->textures)[texIdx].texture, writable) == 0) {
        ok = 0;
      }
    }
  }
  return ok;
}
