// bdc 0x08a14e4c GmoModelDestroyContents
#include "bdc.h"

/* Releases everything a GMO model data block (0xc0-byte header from `GmoModelCreate`)
   references, without freeing the header: drops one reference on each node (a node reaching 0 is
   emptied by `GmoNodeDestroyContents` and freed), on each `GmoPart` (a part reaching 0 drops
   one reference on each of its meshes, emptying (`GmoMeshDestroyContents`) and freeing those
   reaching 0, then frees the part), releases the materials (`GmoMaterialArrayRelease`), drops
   one reference on each texture `GmoLayer` (a layer reaching 0 releases its texture with
   `GmoTextureRelease` and is freed), releases `motionCount + 1` motions
   (`GmoMotionArrayRelease`), frees `bbox`, `chunk16` and `drawNodes`, and releases the
   display-list cache (`GmoDlCacheRelease`) and pointer tables (`GmoModelPtrTablesRelease`).
   All frees go through `GmoHeapReleaseThunk` with pool 0. Returns `self` (NULL is a no-op). */

GmoModel *GmoModelDestroyContents(GmoModel *self)
{
  GmoNode *node;
  GmoPart *part;
  GmoMesh *mesh;
  GmoLayer *layer;
  u32 count;
  u32 meshCount;
  u32 i;
  u32 j;

  if (self == NULL) {
    return self;
  }

  node = self->nodes;
  count = self->nodeCount;
  if (node != NULL && count != 0) {
    for (i = 0; i != count; i++, node++) {
      if (--node->refCount == 0) {
        GmoNodeDestroyContents(node);
        GmoHeapReleaseThunk(0, node);
      }
    }
  }

  part = (GmoPart *)self->parts;
  count = self->partCount;
  if (part != NULL && count != 0) {
    for (i = 0; i != count; i++, part++) {
      if (--part->refCount != 0) {
        continue;
      }
      mesh = part->meshes;
      meshCount = part->meshCount;
      if (mesh != NULL && meshCount != 0) {
        for (j = 0; j != meshCount; j++, mesh++) {
          if (--mesh->refCount == 0) {
            GmoMeshDestroyContents(mesh);
            GmoHeapReleaseThunk(0, mesh);
          }
        }
      }
      GmoHeapReleaseThunk(0, part);
    }
  }

  GmoMaterialArrayRelease(self->materials, self->materialCount);

  layer = (GmoLayer *)self->textures;
  count = self->textureCount;
  if (layer != NULL && count != 0) {
    for (i = 0; i != count; i++, layer++) {
      if (--layer->f00 == 0) {
        GmoTextureRelease((short *)layer->texture);
        GmoHeapReleaseThunk(0, layer);
      }
    }
  }

  GmoMotionArrayRelease(self->motions, self->motionCount + 1);
  GmoHeapReleaseThunk(0, self->bbox);
  GmoHeapReleaseThunk(0, self->chunk16);
  GmoHeapReleaseThunk(0, self->drawNodes);
  GmoDlCacheRelease(self->dlCache);
  GmoModelPtrTablesRelease(self->ptrTables);
  return self;
}
