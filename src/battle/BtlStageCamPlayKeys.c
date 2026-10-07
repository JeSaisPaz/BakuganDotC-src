// bdc 0x08903abc BtlStageCamPlayKeys
#include "bdc.h"

/* Plays one frame of the stage camera task's script keys into its demo camera `cam`, then stores
   `frame` in `cam->frame` and advances `frame`. `keyPhase` 0 resets the eye and lens cursors (not
   the second track's) and moves to 1 (playing); 2 sets `skipped`; any other value only does the
   frame bookkeeping. While playing, three tracks are walked: the eye track
   `scene->head[eyeTrack]`, the second track `scene->head[keyBTrack]` (both `BtlStageCamKey`) and
   the lens track `scene->blocks[1][0]` (`BtlStageCamLensKey`). For each, once the current key's
   frame is <= `frame`, the next key (or the last one when the cursor is on it; a zeroed key, with an
   extra cursor step, when the count is <= 0) is pushed and the cursor advanced: `BtlDemoCamPushEyeKey` with
   `pos` x 0.1 plus the followed unit's position (its y forced to 0) and `target`;
   `BtlDemoCamPushKeyB` with `pos` x 0.1 and `target`; `BtlDemoCamPushLensKey` with `pos` x 0.1
   (replaced by the unit position vector when its x/y/z bits are not all ±0), `paramA` and the
   current key's `paramB` (0 for camera indices 0x10..0x12 and 0x18). A cursor past the end is
   clamped to the last key. `keyPhase` advances to 2 when the eye track has no key array, or when
   the eye track has ended and the lens track has no key array or its cursor is past the end. Called by
   `BtlStageCamStateStart` and `BtlStageCamStatePlay`. */
void BtlStageCamPlayKeys(BtlStageCam *task)
{
    float unitPos[4] __attribute__((aligned(16)));
    bool eyeEnded;
    bool playing;

    unitPos[0] = 0.0f;
    unitPos[1] = 0.0f;
    unitPos[2] = 0.0f;
    unitPos[3] = 0.0f;
    if (task->unit != NULL) {
        unitPos[0] = task->unit->pos[0];
        unitPos[1] = task->unit->pos[1];
        unitPos[2] = task->unit->pos[2];
        unitPos[3] = task->unit->pos[3];
        unitPos[1] = 0.0f;
    }

    if (task->keyPhase < 0) {
        goto done;
    }
    if (task->keyPhase == 0) {
        task->eyeCursor = 0;
        task->lensCursor = 0;
        task->keyPhase++;
    } else if (task->keyPhase == 2) {
        task->skipped = 1;
        goto done;
    } else if (task->keyPhase != 1) {
        goto done;
    }

    eyeEnded = false;
    if (task->scene->head[task->eyeTrack].keys == NULL) {
        playing = false;
    } else {
        /* eye track */
        {
            const BtlStageCamKeySlot12 *slot = &task->scene->head[task->eyeTrack];
            const BtlStageCamKey *keys = (const BtlStageCamKey *)slot->keys;

            if (task->eyeCursor < slot->count) {
                BtlStageCamKey cur = keys[task->eyeCursor];
                BtlStageCamKey next;

                if (task->eyeCursor + 1 < slot->count) {
                    next = keys[task->eyeCursor + 1];
                } else if (slot->count - 1 < 0) {
                    memset(&next, 0, sizeof(next));
                    task->eyeCursor++;
                } else {
                    next = keys[slot->count - 1];
                    eyeEnded = true;
                }
                if (cur.frame <= task->frame) {
                    float eye[4] __attribute__((aligned(16)));
                    float look[4] __attribute__((aligned(16)));
                    float eyeArg[4] __attribute__((aligned(16)));
                    float lookArg[4] __attribute__((aligned(16)));

                    eye[0] = next.pos[0] * 0.100000001f;
                    eye[1] = next.pos[1] * 0.100000001f;
                    eye[2] = next.pos[2] * 0.100000001f;
                    eye[3] = 0.0f;
                    look[0] = next.target[0];
                    look[1] = next.target[1];
                    look[2] = next.target[2];
                    look[3] = 0.0f;
                    eye[0] += unitPos[0];
                    eye[1] += unitPos[1];
                    eye[2] += unitPos[2];
                    eyeArg[0] = eye[0];
                    eyeArg[1] = eye[1];
                    eyeArg[2] = eye[2];
                    eyeArg[3] = eye[3];
                    lookArg[0] = look[0];
                    lookArg[1] = look[1];
                    lookArg[2] = look[2];
                    lookArg[3] = look[3];
                    BtlDemoCamPushEyeKey(task->cam, eyeArg, lookArg, next.frame);
                    task->eyeCursor++;
                }
            } else {
                task->eyeCursor = slot->count - 1;
                eyeEnded = true;
            }
        }

        /* second track (no key-array check) */
        {
            const BtlStageCamKeySlot12 *slot = &task->scene->head[task->keyBTrack];
            const BtlStageCamKey *keys = (const BtlStageCamKey *)slot->keys;

            if (task->keyBCursor < slot->count) {
                BtlStageCamKey cur = keys[task->keyBCursor];
                BtlStageCamKey next;

                if (task->keyBCursor + 1 < slot->count) {
                    next = keys[task->keyBCursor + 1];
                } else if (slot->count - 1 < 0) {
                    memset(&next, 0, sizeof(next));
                    task->keyBCursor++;
                } else {
                    next = keys[slot->count - 1];
                }
                if (cur.frame <= task->frame) {
                    float a[4] __attribute__((aligned(16)));
                    float b[4] __attribute__((aligned(16)));
                    float aArg[4] __attribute__((aligned(16)));
                    float bArg[4] __attribute__((aligned(16)));

                    a[0] = next.pos[0] * 0.100000001f;
                    a[1] = next.pos[1] * 0.100000001f;
                    a[2] = next.pos[2] * 0.100000001f;
                    a[3] = 0.0f;
                    b[0] = next.target[0];
                    b[1] = next.target[1];
                    b[2] = next.target[2];
                    b[3] = 0.0f;
                    aArg[0] = a[0];
                    aArg[1] = a[1];
                    aArg[2] = a[2];
                    aArg[3] = a[3];
                    bArg[0] = b[0];
                    bArg[1] = b[1];
                    bArg[2] = b[2];
                    bArg[3] = b[3];
                    BtlDemoCamPushKeyB(task->cam, aArg, bArg, next.frame);
                    task->keyBCursor++;
                }
            } else {
                task->keyBCursor = slot->count - 1;
            }
        }

        /* lens track */
        {
            const BtlStageCamKeySlot8 *slot = &task->scene->blocks[1][0];
            const BtlStageCamLensKey *keys = (const BtlStageCamLensKey *)slot->keys;

            if (keys == NULL) {
                playing = !eyeEnded;
            } else if (task->lensCursor < slot->count) {
                BtlStageCamLensKey cur = keys[task->lensCursor];
                BtlStageCamLensKey next;

                playing = true;
                if (task->lensCursor + 1 < slot->count) {
                    next = keys[task->lensCursor + 1];
                } else if (slot->count - 1 < 0) {
                    memset(&next, 0, sizeof(next));
                    task->lensCursor++;
                } else {
                    next = keys[slot->count - 1];
                }
                if (cur.frame <= task->frame) {
                    float vec[4] __attribute__((aligned(16)));
                    float vecArg[4] __attribute__((aligned(16)));
                    u32 bx;
                    u32 by;
                    u32 bz;
                    float paramB;

                    vec[0] = next.pos[0] * 0.100000001f;
                    vec[1] = next.pos[1] * 0.100000001f;
                    vec[2] = next.pos[2] * 0.100000001f;
                    vec[3] = 0.0f;
                    memcpy(&bx, &unitPos[0], sizeof(bx));
                    memcpy(&by, &unitPos[1], sizeof(by));
                    memcpy(&bz, &unitPos[2], sizeof(bz));
                    if (((bx | by | bz) & 0x7fffffffu) != 0) {
                        vec[0] = unitPos[0];
                        vec[1] = unitPos[1];
                        vec[2] = unitPos[2];
                        vec[3] = unitPos[3];
                    }
                    if ((task->index >= 0x10 && task->index < 0x13) || task->index == 0x18) {
                        paramB = 0.0f;
                    } else {
                        paramB = cur.paramB;
                    }
                    vecArg[0] = vec[0];
                    vecArg[1] = vec[1];
                    vecArg[2] = vec[2];
                    vecArg[3] = vec[3];
                    BtlDemoCamPushLensKey(next.paramA, paramB, task->cam, vecArg, next.frame);
                    task->lensCursor++;
                }
            } else {
                task->lensCursor = slot->count - 1;
                playing = !eyeEnded;
            }
        }
    }
    if (!playing) {
        task->keyPhase++;
    }

done:
    task->cam->frame = task->frame;
    task->frame++;
}
