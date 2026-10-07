// bdc 0x089d9c54 GmoMotionIndexOfName
#include "bdc.h"

/* Returns the index of the motion called `name` in the registry, loading `name + ".bin"` with
   `GmoMotionLoadFile` when it is not registered yet (the new entry's index is then the old
   count). Returns -1 when the load failed. */

typedef struct GmoMotionVtblView {
  u8 pad00[0x28];
  s16 nameCmpAdj;
  u8 pad2a[2];
  s32 (*nameCmp)(void *self, const char *name);
} GmoMotionVtblView;

s32 GmoMotionIndexOfName(void *mgr, const char *name)
{
  CoreNode *node;
  const GmoMotionVtblView *vt;
  s32 index = 0;
  char path[76];

  for (node = g_gmoMotionRegistry->next; node != NULL; node = node->next) {
    vt = (const GmoMotionVtblView *)node->vtable;
    if (vt->nameCmp((u8 *)node + vt->nameCmpAdj, name) != 0) {
      return index;
    }
    index++;
  }
  strcpy(path, name);
  strcat(path, ".bin");
  if (GmoMotionLoadFile(mgr, path) != NULL) {
    return index;
  }
  return -1;
}
