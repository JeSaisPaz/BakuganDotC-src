// bdc 0x08a15548 GmoModelReleaseInstances
#include "bdc.h"

/* Counterpart of `GmoModelCreateInstances`. When `flags & 4`, for every mesh entry of every part
   of a GMO model data block: finds its id-1 instance (`GmoMeshFindInstance`), unlinks and
   releases it (`GmoMeshRemoveInstance`, `GmoInstanceArrayRelease`) when found, and rebinds the
   mesh data (`GmoMeshSetData`) to the display list of the instance list head read before the
   removal. When `flags & 0xa`, calls `GmoTextureReleaseWritable` on every texture with
   `(flags & 2 ? 1 : 0) | (flags & 8 ? 2 : 0)`. */

void GmoModelReleaseInstances(GmoModel *self, u32 flags)
{
  int partIdx;
  int meshIdx;
  int texIdx;
  u32 writable;

  if (self == NULL) {
    return;
  }
  if ((flags & 4) != 0) {
    for (partIdx = 0; partIdx < (int)self->partCount; partIdx++) {
      GmoPart *part = &((GmoPart *)self->parts)[partIdx];
      for (meshIdx = 0; meshIdx < (int)part->meshCount; meshIdx++) {
        GmoMesh *mesh = &part->meshes[meshIdx];
        GmoInstance *inst = (GmoInstance *)GmoMeshFindInstance(mesh, 1, 0);
        GmoInstance *head = mesh->instances;
        if (inst != NULL && head != NULL) {
          GmoMeshRemoveInstance(mesh, inst);
          GmoInstanceArrayRelease((short *)inst, 1);
        }
        GmoMeshSetData(mesh, head->displayList);
      }
    }
  }
  if ((flags & 0xa) != 0 && (int)self->textureCount > 0) {
    writable = (flags & 2) != 0;
    if ((flags & 8) != 0) {
      for (texIdx = 0; texIdx < (int)self->textureCount; texIdx++) {
        GmoTextureReleaseWritable(((GmoLayer *)self->textures)[texIdx].texture, writable | 2);
      }
    } else {
      for (texIdx = 0; texIdx < (int)self->textureCount; texIdx++) {
        GmoTextureReleaseWritable(((GmoLayer *)self->textures)[texIdx].texture, writable);
      }
    }
  }
}
