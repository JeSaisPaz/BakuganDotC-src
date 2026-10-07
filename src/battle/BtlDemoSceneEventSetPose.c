// bdc 0x08906210 BtlDemoSceneEventSetPose
#include "bdc.h"

/* Pose event handler of the battle demo scene player task (`BtlDemoScenePlayer`): when the
   event's frame equals the player's current frame, then unless the event's `flags` is nonzero it
   copies the pose values `valueA`/`valueB` (zero-extended u16) into `poseA`/`poseB`, and for demo
   ids 0x19..0x68 overrides `poseB` from a switch: 1 for ids 0x23, 0x32 and 0x5b, 2 for 0x3f, 0 for
   0x19, 0x1d, 0x21, 0x25, 0x29, 0x2d, 0x31, 0x35, 0x39, 0x3d, 0x41, 0x45, 0x49, 0x4d, 0x51,
   0x55, 0x59, 0x5d, 0x61, 0x65, 0x67 and 0x68. It then deletes the event through its deleting
   destructor (vtable slot 1, flag 3) when the event is not NULL. */
void BtlDemoSceneEventSetPose(void *task, void *ev)
{
    BtlDemoScenePlayer *player = (BtlDemoScenePlayer *)task;
    BtlDemoScbPoseEvent *pose = (BtlDemoScbPoseEvent *)ev;

    if (pose->base.frame != player->frame) {
        return;
    }
    if (pose->base.flags == 0) {
        player->poseA = (u16)pose->valueA;
        player->poseB = (u16)pose->valueB;
        switch (player->demoId) {
        case 0x19: case 0x1d: case 0x21: case 0x25: case 0x29: case 0x2d: case 0x31:
        case 0x35: case 0x39: case 0x3d: case 0x41: case 0x45: case 0x49: case 0x4d:
        case 0x51: case 0x55: case 0x59: case 0x5d: case 0x61: case 0x65: case 0x67:
        case 0x68:
            player->poseB = 0;
            break;
        case 0x23:
        case 0x32:
        case 0x5b:
            player->poseB = 1;
            break;
        case 0x3f:
            player->poseB = 2;
            break;
        default:
            break;
        }
    }
    if (pose != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)pose->base.base.vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)pose + dtor->delta, 3);
    }
}
