// bdc 0x08905620 BtlDemoScenePlayerCtor
#include "bdc.h"

/* Constructor of the battle demo scene player task `BtlDemoScenePlayer` (vtable
   `g_btlDemoScenePlayerVtbl`): `CoreTaskInit`, installs the vtable,
   `BtlDemoScenePlayerReset`, then stores demoId, unit, actor and follower; when actor and
   follower are both non-NULL it caches the actor model's sphere01 node (`GfxModelFindNode`) in sphereNode.
   For demo ids 0x1f, 0x23, 0x27, 0x2f, 0x33, 0x37, 0x3f, 0x43, 0x47, 0x4b, 0x4f, 0x57, 0x5f and
   0x63 it sets specialId to 1. Returns task. */
BtlDemoScenePlayer *BtlDemoScenePlayerCtor(BtlDemoScenePlayer *task, s32 demoId, void *unit, Actor *actor,
                                           void *follower)
{
    CoreTaskInit(&task->base);
    task->base.vtable = g_btlDemoScenePlayerVtbl;
    BtlDemoScenePlayerReset(task);
    task->demoId = demoId;
    task->unit = unit;
    task->actor = actor;
    task->follower = follower;
    if (actor != NULL && task->follower != NULL) {
        task->sphereNode = GfxModelFindNode(&actor->base, "sphere01");
    }
    switch (task->demoId) {
    case 0x1f:
    case 0x23:
    case 0x27:
    case 0x2f:
    case 0x33:
    case 0x37:
    case 0x3f:
    case 0x43:
    case 0x47:
    case 0x4b:
    case 0x4f:
    case 0x57:
    case 0x5f:
    case 0x63:
        task->specialId = 1;
        break;
    }
    return task;
}
