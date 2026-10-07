// bdc 0x08a14b90 GmoMeshDestroyContents
#include "bdc.h"

/* Releases what a GMO mesh record references: its material array (`materialIndex`, when it holds
   a pointer rather than a small index, `GmoMaterialArrayRelease`), instance array (`instances`,
   `GmoInstanceArrayRelease`), data blocks `patchScale`/`skinData` (pool 0) and `data` (pool 1),
   and the vertex array (`GmoVertexArrayRelease`). A NULL `mesh` releases nothing. Returns `mesh`. */

void *GmoMeshDestroyContents(GmoMesh *mesh)
{
  u32 material;

  if (mesh != NULL) {
    /* `materialIndex` is a word that is either a material index (0..0xfffe, or 0xffffffff = none)
       or, once resolved, a GmoMaterial pointer: values with (v + 1) < 0x10000 are not pointers. */
    material = mesh->materialIndex;
    if (((material + 1) & 0xffff0000) == 0) {
      material = 0;
    }
    GmoMaterialArrayRelease((short *)(uintptr_t)material, 1);
    GmoInstanceArrayRelease((short *)mesh->instances, 1);
    GmoHeapReleaseThunk(0, mesh->patchScale);
    GmoHeapReleaseThunk(0, mesh->skinData);
    GmoHeapReleaseThunk(1, mesh->data);
    GmoVertexArrayRelease(mesh->vertexArray);
  }
  return mesh;
}
