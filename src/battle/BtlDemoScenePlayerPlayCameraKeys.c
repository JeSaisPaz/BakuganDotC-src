// bdc 0x089070cc BtlDemoScenePlayerPlayCameraKeys
#include "bdc.h"

/* Plays the camera key tracks of the battle demo scene player for one frame. Phase `camStep` 0
   resets both key cursors and the scene frame and moves to phase 1; in phase 1 the eye track
   (`scene->track0[poseB]`, skipped entirely with the lens track when it has no keys) and the lens
   track (`scene->tracks[1][0]`) each check their current key: when the scene frame equals its
   frame, the following key (or the last key when the cursor is on the last one) is pushed into the
   demo camera (`BtlDemoCamPushEyeKey`: eye `pos × 0.1`, `lookAt`; `BtlDemoCamPushLensKey`:
   `vec × 0.1` with the following key's `lensA` and the current key's `lensB`) and the cursor
   advances. A cursor past its track's end is pulled back to the last key and counts the track as
   done (a lens track without keys counts as done too); when both are done the phase becomes 2.
   Every call ends by copying the scene frame into the camera's `frame`. */
void BtlDemoScenePlayerPlayCameraKeys(BtlDemoScenePlayer *player)
{
    BtlDemoScene *scene;
    const BtlDemoCamEyeKey *eyeKeys;
    const BtlDemoCamLensKey *lensKeys;
    BtlDemoCamEyeKey eyeCur;
    BtlDemoCamEyeKey eyeNext;
    BtlDemoCamLensKey lensCur;
    BtlDemoCamLensKey lensNext;
    float eye[4] __attribute__((aligned(16)));
    float lookAt[4] __attribute__((aligned(16)));
    float vec[4] __attribute__((aligned(16)));
    s32 count;
    s32 done;

    if (player->camStep > 0) {
        if (player->camStep >= 2) {
            player->camera->frame = player->frame;
            return;
        }
    } else {
        if (player->camStep < 0) {
            player->camera->frame = player->frame;
            return;
        }
        player->eyeCursor = 0;
        player->lensCursor = 0;
        player->frame = 0;
        player->camStep = player->camStep + 1;
    }

    done = 0;
    if (player->scene->track0[player->poseB].keys != NULL) {
        scene = player->scene;
        count = (s32)scene->track0[player->poseB].keyCount;
        if (player->eyeCursor >= count) {
            player->eyeCursor = count - 1;
            done = 1;
        } else {
            eyeKeys = scene->track0[player->poseB].keys;
            eyeCur = eyeKeys[player->eyeCursor];
            if (player->eyeCursor + 1 < (s32)scene->track0[player->poseB].keyCount) {
                eyeKeys = player->scene->track0[player->poseB].keys;
                eyeNext = eyeKeys[player->eyeCursor + 1];
            } else if ((s32)player->scene->track0[player->poseB].keyCount - 1 < 0) {
                memset(&eyeNext, 0, sizeof(eyeNext));
                player->eyeCursor = player->eyeCursor + 1;
            } else {
                scene = player->scene;
                eyeKeys = scene->track0[player->poseB].keys;
                eyeNext = eyeKeys[(s32)scene->track0[player->poseB].keyCount - 1];
            }
            if (player->frame == eyeCur.frame) {
                eye[0] = eyeNext.pos[0] * 0.1f;
                eye[1] = eyeNext.pos[1] * 0.1f;
                eye[2] = eyeNext.pos[2] * 0.1f;
                eye[3] = 0.0f;
                lookAt[0] = eyeNext.lookAt[0];
                lookAt[1] = eyeNext.lookAt[1];
                lookAt[2] = eyeNext.lookAt[2];
                lookAt[3] = 0.0f;
                BtlDemoCamPushEyeKey(player->camera, eye, lookAt, eyeNext.frame);
                player->eyeCursor = player->eyeCursor + 1;
            }
        }

        scene = player->scene;
        count = (s32)scene->tracks[1][0].keyCount;
        if (scene->tracks[1][0].keys == NULL) {
            done = done + 1;
        } else if (player->lensCursor >= count) {
            player->lensCursor = count - 1;
            done = done + 1;
        } else {
            lensKeys = player->scene->tracks[1][0].keys;
            lensCur = lensKeys[player->lensCursor];
            if (player->lensCursor + 1 < (s32)player->scene->tracks[1][0].keyCount) {
                lensKeys = player->scene->tracks[1][0].keys;
                lensNext = lensKeys[player->lensCursor + 1];
            } else if ((s32)player->scene->tracks[1][0].keyCount - 1 < 0) {
                memset(&lensNext, 0, sizeof(lensNext));
                player->lensCursor = player->lensCursor + 1;
            } else {
                scene = player->scene;
                lensKeys = scene->tracks[1][0].keys;
                lensNext = lensKeys[(s32)scene->tracks[1][0].keyCount - 1];
            }
            if (player->frame == lensCur.frame) {
                vec[0] = lensNext.vec[0] * 0.1f;
                vec[1] = lensNext.vec[1] * 0.1f;
                vec[2] = lensNext.vec[2] * 0.1f;
                vec[3] = 0.0f;
                BtlDemoCamPushLensKey(lensNext.lensA, lensCur.lensB, player->camera, vec, lensNext.frame);
                player->lensCursor = player->lensCursor + 1;
            }
        }
    }
    if (done >= 2) {
        player->camStep = player->camStep + 1;
    }
    player->camera->frame = player->frame;
}
