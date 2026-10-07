// bdc 0x08a15d48 GmoMeshSetData
#include "bdc.h"

/* Replaces the data block referenced at `data` of a mesh entry, moving the heap reference (pool
   1). */

void GmoMeshSetData(GmoMesh *mesh, void *data)
{
  if (mesh != NULL && mesh->data != data) {
    GmoHeapRelease(1, mesh->data);
    GmoHeapAddRef(1, data);
    mesh->data = data;
  }
}
