// bdc 0x089d9ddc GmoMotionGetDataByName
#include "bdc.h"

/* Returns the data block of the motion called `name`: looks it up with `GmoMotionFindByName` and,
   when it is not registered, loads `name + ".bin"` with `GmoMotionLoadFile` and takes the new
   entry's data (virtual get-data, vtable entry 2). NULL when the load failed. */

typedef struct GmoMotionVtblView {
  u8 pad00[0x10];
  s16 getDataAdj;
  u8 pad12[2];
  void *(*getData)(void *self);
} GmoMotionVtblView;

void *GmoMotionGetDataByName(void *mgr, const char *name)
{
  CoreNode *node;
  const GmoMotionVtblView *vt;
  char path[76];

  node = GmoMotionFindByName(mgr, name);
  if (node == NULL) {
    strcpy(path, name);
    strcat(path, ".bin");
    node = GmoMotionLoadFile(mgr, path);
    if (node == NULL) {
      return NULL;
    }
  }
  vt = (const GmoMotionVtblView *)node->vtable;
  return vt->getData((u8 *)node + vt->getDataAdj);
}
