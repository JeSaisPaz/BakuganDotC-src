// bdc 0x089d9ddc GmoMotionGetDataByName
#include "bdc.h"

/* Returns the data block of the motion called `name`: looks it up with `GmoMotionFindByName` and,
   when it is not registered, loads `name + ".bin"` with `GmoMotionLoadFile` and takes the new
   entry's data (virtual get-data, vtable entry 2). NULL when the load failed. */

void *GmoMotionGetDataByName(void *mgr, const char *name)
{
  CoreNode *node;
  const VtblEntry *getData;
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
  getData = &((const VtblEntry *)node->vtable)[2];
  return ((void *(*)(void *))getData->fn)((u8 *)node + getData->delta);
}
