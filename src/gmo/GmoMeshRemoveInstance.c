// bdc 0x08a15cf8 GmoMeshRemoveInstance
#include "bdc.h"

/* Unlinks an instance from a mesh entry's instance list (`instances`). */

void GmoMeshRemoveInstance(GmoMesh *mesh, GmoInstance *inst)
{
  GmoInstance **link;
  GmoInstance *cur;

  if (mesh != NULL && inst != NULL) {
    link = &mesh->instances;
    cur = *link;
    while (cur != NULL) {
      if (cur == inst) {
        *link = inst->next;
        inst->next = NULL;
        return;
      }
      link = &cur->next;
      cur = *link;
    }
  }
}
