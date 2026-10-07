// bdc 0x08a15710 GmoModelPrepareInstances
#include "bdc.h"

/* Prepares the per-model mesh instances of a GMO model data block: initialises a private plan
   (`GmoPlanInit`); when `flags & 4`, runs the measure pass (`GmoInstanceArrayMeasureCopy`,
   one record, flags 0x80000001) on the instance list of every mesh entry that has instances but
   no id-1 instance (`GmoMeshFindInstance`); then commits the plan (`GmoPlanCommit`), creates
   the instances (`GmoModelCreateInstances`) and frees the plan (`GmoPlanFree`). Returns the
   result of `GmoModelCreateInstances`, or 0 for a NULL model or when the plan could not be
   committed (the plan is not freed on that path). */

int GmoModelPrepareInstances(GmoModel *self, u32 flags)
{
  int plan[28];
  GmoPart *part;
  GmoMesh *mesh;
  int i;
  int j;
  int result;

  GmoPlanInit(plan);
  if (self == NULL) {
    return 0;
  }
  if ((flags & 4) != 0) {
    for (i = 0; i < (int)self->partCount; i++) {
      part = &((GmoPart *)self->parts)[i];
      for (j = 0; j < (int)part->meshCount; j++) {
        mesh = &part->meshes[j];
        if (GmoMeshFindInstance(mesh, 1, 0) == NULL && mesh->instances != NULL) {
          GmoInstanceArrayMeasureCopy(mesh->instances, 1, 0x80000001, plan);
        }
      }
    }
  }
  if (GmoPlanCommit(plan) == 0) {
    return 0;
  }
  result = GmoModelCreateInstances(self, flags, plan);
  GmoPlanFree(plan);
  return result;
}
