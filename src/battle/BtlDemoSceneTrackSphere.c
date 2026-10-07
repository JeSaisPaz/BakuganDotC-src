// bdc 0x08907740 BtlDemoSceneTrackSphere
#include "bdc.h"

/* Keeps the player's follower model on the actor's tracked sphere01 node: when both the node and
   the follower are set and the actor plays motion slot 0x2f (`ActorIsMotionPlaying`), sets the
   follower's root-matrix translation and its position to the per-battle stage vector
   (`BtlGetStageAmbientColor`) plus 0.1 x the node translation (xyz; w is the stage vector's
   w), then forces the root-matrix translation w to 1. 0.1f is 0x3dcccccd. */
void BtlDemoSceneTrackSphere(BtlDemoScenePlayer *task)
{
    float stage[4] __attribute__((aligned(16)));
    const float *node;
    float *translate;
    float *pos;
    float s0, s1, s2;

    if (task->sphereNode == NULL || task->follower == NULL) {
        return;
    }
    if (ActorIsMotionPlaying(task->actor, 0x2f) == 0) {
        return;
    }
    pos = task->follower->pos;
    translate = &task->follower->data->rootMatrix[12];
    BtlGetStageAmbientColor(stage);
    /* sum.xyz = stage.xyz + node.translate * 0.1, sum.w = stage.w; written to the root-matrix
       translation row, then copied to the follower's position. */
    node = task->sphereNode->translate;
    s0 = stage[0] + node[0] * 0.1f;
    s1 = stage[1] + node[1] * 0.1f;
    s2 = stage[2] + node[2] * 0.1f;
    translate[0] = s0;
    translate[1] = s1;
    translate[2] = s2;
    translate[3] = stage[3];
    pos[0] = translate[0];
    pos[1] = translate[1];
    pos[2] = translate[2];
    pos[3] = translate[3];
    task->follower->data->rootMatrix[15] = 1.0f;
}
