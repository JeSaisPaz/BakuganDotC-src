// bdc 0x089c8de8 SndReleaseAll
#include "bdc.h"

/* Quiesces the positional-sound layer before a scene change: disables the group loader and marks
   all of its outstanding requests as released (`SndGroupLoaderReleaseAll`), then tears down the
   global list of exclusive emitter groups `g_soundEmitterGroupList`. For every emitter of every
   group it sets `released = 1`, clears `src` and sets `autoFree = 1` (so `SndEmitterUpdateAll`
   frees them once their voices end), removes the group from the list and destroys it
   (`SndEmitterListDestroy`, flags 3); finally it destroys the group list itself and clears the
   global. */

void SndReleaseAll(void)
{
  CorePrioNode *groupNode;
  CorePrioNode *node;
  CorePrioList *group;
  SndEmitter *emitter;

  if (SndHasGroupLoader()) {
    SndGetGroupLoader()->enabled = 0;
  }
  if (SndHasGroupLoader()) {
    SndGroupLoaderReleaseAll(SndGetGroupLoader());
  }
  if (g_soundEmitterGroupList == NULL) {
    return;
  }
  SndEmitterGroupListFlush(g_soundEmitterGroupList);
  groupNode = SndEmitterGroupNodeGetNext(SndEmitterGroupListHead(g_soundEmitterGroupList));
  while (groupNode != NULL) {
    group = SndEmitterGroupNodeGetData(groupNode);
    groupNode = SndEmitterGroupNodeGetNext(groupNode);
    if (group == NULL) {
      continue;
    }
    SndEmitterListFlush(group);
    node = SndEmitterNodeGetNext(SndEmitterListHead(group));
    while (node != NULL) {
      emitter = SndEmitterNodeGetData(node);
      node = SndEmitterNodeGetNext(node);
      if (emitter == NULL) {
        break; /* a NULL payload ends the walk of this group */
      }
      emitter->released = 1;
      emitter->src = NULL;
      emitter->autoFree = 1;
    }
    if (g_soundEmitterGroupList != NULL) {
      SndEmitterGroupListRemove(g_soundEmitterGroupList, group);
    }
    if (group != NULL) {
      SndEmitterListDestroy(group, 3);
    }
  }
  if (g_soundEmitterGroupList != NULL) {
    SndEmitterGroupListDestroy(g_soundEmitterGroupList, 3);
    g_soundEmitterGroupList = NULL;
  }
}
