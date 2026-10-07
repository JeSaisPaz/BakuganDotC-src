// bdc 0x089d9d28 GmoMotionFindByName
#include "bdc.h"

/* Returns the registered motion entry whose virtual name-compare method (vtable entry 5) accepts
   `name`, or NULL. `mgr` is unused. */

typedef struct GmoMotionVtblView {
  u8 pad00[0x28];
  s16 nameCmpAdj;
  u8 pad2a[2];
  s32 (*nameCmp)(void *self, const char *name);
} GmoMotionVtblView;

CoreNode *GmoMotionFindByName(void *mgr, const char *name)
{
  CoreNode *node;
  const GmoMotionVtblView *vt;

  for (node = g_gmoMotionRegistry->next; node != NULL; node = node->next) {
    vt = (const GmoMotionVtblView *)node->vtable;
    if (vt->nameCmp((u8 *)node + vt->nameCmpAdj, name) != 0) {
      return node;
    }
  }
  return NULL;
}
