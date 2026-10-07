// bdc 0x08906300 BtlDemoSceneMotionEventCtor
#include "bdc.h"

/* Constructs a demo scene motion event node: initialises the base object in `list`, installs the
   motion event vtable and stores `scene`, `target`, `trackId` and `demoId`. Returns `node`. */
BtlDemoSceneMotionEvent *BtlDemoSceneMotionEventCtor(BtlDemoSceneMotionEvent *node,
                                                     BtlDemoScene *scene, void *target,
                                                     s32 trackId, s32 demoId, void *list)
{
  CoreObjectInitInList(&node->base, list);
  node->base.vtable = g_btlDemoSceneMotionEventVtbl;
  node->scene = scene;
  node->target = target;
  node->trackId = trackId;
  node->demoId = demoId;
  return node;
}
