// bdc 0x0884a7c4 BtlFinishTaskUpdate
#include "bdc.h"

/* Calls unit virtual `slot` (a no-argument predicate) through the GCC 2.x vtable entry. */
static int UnitVirtual(BtlBakugan *unit, int slot)
{
    const VtblEntry *entry = &((const VtblEntry *)unit->base.base.vtable)[slot];

    return ((int (*)(void *))entry->fn)((u8 *)unit + entry->delta);
}

/* Turns `camDir` about the Y axis by 2.0923 radians (x' = x cos - z sin, z' = x sin + z cos;
   y and w kept). */
static void RotateCamDir(BtlFinishTask *task)
{
    float angle = 0x1.0bd084p+1f;
    float c = __builtin_cosf(angle);
    float s = __builtin_sinf(angle);
    float x = task->camDir[0];
    float y = task->camDir[1];
    float z = task->camDir[2];

    task->camDir[0] = x * c + y * 0.0f + z * -s;
    task->camDir[2] = x * s + y * 0.0f + z * c;
}

/* Runs the world effects the cinematic shows (ids 0x82, 0x1e4, 0x1f, 0x20, 0xa9) and the mesh
   objects for this frame, and points the fog at the arena's record. */
static void UpdateWorldEffects(void)
{
    GfxEffectCaptureCamera();
    GfxEffectUpdateAttachedNow(g_worldEffectMgr, 0x82, NULL);
    GfxEffectUpdateAttachedNow(g_worldEffectMgr, 0x1e4, NULL);
    GfxEffectUpdateAttachedNow(g_worldEffectMgr, 0x1f, NULL);
    GfxEffectUpdateAttachedNow(g_worldEffectMgr, 0x20, NULL);
    GfxEffectUpdateAttachedNow(g_worldEffectMgr, 0xa9, NULL);
    GfxMeshObjUpdateAll();
    g_gfxFogParams = BtlStageGetFogParams();
}

/* Update of the end-of-battle cinematic task (`BtlFinishTask`, vtable slot 2). When profile
   flag 0 is set and the profile has any of the flags 0x4880, it unpauses the battle main task
   (task 100) and removes itself at once. Otherwise, with `farCamera` (the ctor mode) 0, it looks
   the followed unit up in the battle list (`BtlBakuganListFind`) and takes the unit's last melee
   attacker unless that attacker's virtual slot 11 is non-zero. Steps (`step`, `timer` counts frames):
   0 — camera ramp (`BtlFinishTaskUpdateCamera`); after 21 frames turns `camDir` about Y,
   resets distance 1000 / blend 0.5 / flash 0, replays the unit's motion 0xee, holds the attacker
   at `attackerFrame` and plays sound 0x200133; 1, 2 — the same every 16 frames until step 3;
   3 — first frame clears the battle's white flash and slows motion to 0.07, after 50 frames opens
   UI window 3, stops effects 0x82/0x1e4, unpauses task 100, hides the HUD, plays the unit's
   motion 0xf2 and ends its hit reaction, restores the time scale and the field effects (timer
   starts at -20 while task 0x1e1 exists); 4 — after 36 frames raises the battle's dim alpha by 0.1
   per frame while the match is undecided, moves on after 46; 5 — waits for the intro demo to
   finish (running the world effects and returning meanwhile); 6 — sets `g_btlDemoReady`, opens
   UI window 2 and removes the task. Then it advances both models' motions, picks the camera focus
   by `farCamera` (0 the unit's "Bip01" node, 1 its position + 100 in Y, 2 its node 0 + 200 in Y,
   other values the origin), in mode 0 resets the distance to
   300 and the zoom (`frustumScale`) from `camBlend` and the unit height, places the eye at
   `focus + camDir * camDistance` (steps below 4) and the target at the focus, updates the
   camera and, in mode 0 before step 4, the world effects. */
void BtlFinishTaskUpdate(BtlFinishTask *task)
{
    BtlBakugan *unit = NULL;
    BtlBakugan *attacker = NULL;
    BtlMain *battle;
    float focus[4] __attribute__((aligned(16)));
    ScePspFVector4 nodePos __attribute__((aligned(16)));
    ScePspFVector4 bipPos __attribute__((aligned(16)));
    float c;
    float zoom;
    s32 old;

    if (SaveGetProfileFlag0() != 0 && SaveHasProfile() &&
        SaveProfileHasFlags(SaveGetProfile(), 0x4880)) {
        CoreTaskClearFlags(CoreTaskFind(100), 1);
        CoreTaskRemove(&task->base, true);
        return;
    }
    if (task->farCamera == 0) {
        unit = BtlBakuganListFind(task->unit);
        if (unit != NULL) {
            attacker = unit->meleeHitAttacker;
        }
        if (attacker != NULL && UnitVirtual(attacker, 11) != 0) {
            attacker = NULL;
        }
    }

    switch (task->step) {
    case 0:
        BtlFinishTaskUpdateCamera(task);
        old = task->timer++;
        if (old > 20) {
            task->timer = 0;
            task->step++;
            RotateCamDir(task);
            task->camDistance = 1000.0f;
            task->camBlend = 0.5f;
            task->flashT = 0.0f;
            if (unit != NULL) {
                BtlBakuganPlayMotion(0.0f, unit, 0xee, 0, 1);
            }
            if (attacker != NULL) {
                GfxModelSwapMotionFrame(&attacker->base, task->attackerFrame);
            }
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x200133, 0, 0);
            }
        }
        break;
    case 1:
    case 2:
        BtlFinishTaskUpdateCamera(task);
        old = task->timer++;
        if (old > 15) {
            task->timer = 0;
            task->step++;
            if (task->step < 3) {
                RotateCamDir(task);
                task->camDistance = 1000.0f;
                if (unit != NULL) {
                    BtlBakuganPlayMotion(0.0f, unit, 0xee, 0, 1);
                }
                if (attacker != NULL) {
                    GfxModelSwapMotionFrame(&attacker->base, task->attackerFrame);
                }
                task->flashT = 0.0f;
                task->camBlend = 0.5f;
                if (SndHasManager()) {
                    SndManagerPlay(SndGetManager(), 0x200133, 0, 0);
                }
            }
        }
        break;
    case 3:
        task->timer++;
        if (task->timer == 1) {
            battle = BtlGetCameraTask();
            battle->flashTarget = 0.0f;
            GfxSetMotionTimeScale(0.0700000003f);
        } else if (task->timer > 50) {
            UiSetWindowActive(3, 1);
            GfxEffectStopAttached(g_worldEffectMgr, 0x82, NULL);
            GfxEffectStopAttached(g_worldEffectMgr, 0x1e4, NULL);
            CoreTaskClearFlags(CoreTaskFind(100), 1);
            g_btlHudHidden = 1;
            if (unit != NULL) {
                BtlBakuganPlayMotion(0.200000003f, unit, 0xf2, 1, 0);
                BtlBakuganStopHitReaction(unit);
            }
            task->timer = 0;
            if (CoreTaskExists(0x1e1) != 0) {
                task->timer = -20;
            }
            task->step++;
            GfxSetMotionTimeScale(1.0f);
            BtlSetFieldEffectsActive(true);
        }
        break;
    case 4:
        old = task->timer++;
        if (old >= 46) {
            task->step++;
        } else if (task->timer >= 36 && BtlCameraTaskExists() != 0 &&
                   BtlMainIsMatchUndecided(BtlGetCameraTask())) {
            battle = BtlGetCameraTask();
            battle->dimColor[3] = battle->dimColor[3] + 0.100000001f;
        }
        break;
    case 5:
        if (!BtlDemoIsFinished()) {
            UpdateWorldEffects();
            return;
        }
        task->step++;
        /* fall through */
    case 6:
        g_btlDemoReady = 1;
        UiSetWindowActive(2, 1);
        CoreTaskRemove(&task->base, true);
        return;
    default:
        break;
    }

    if (unit != NULL) {
        GfxModelUpdateMotion(&unit->base);
        GfxModelApplyMotion(&unit->base);
    }
    if (attacker != NULL) {
        GfxModelUpdateMotion(&attacker->base);
        GfxModelApplyMotion(&attacker->base);
    }

    focus[0] = 0.0f;
    focus[1] = 0.0f;
    focus[2] = 0.0f;
    focus[3] = 0.0f;
    if (task->farCamera == 1) {
        GfxModel *model = (GfxModel *)task->unit;

        focus[0] = model->pos[0];
        focus[1] = model->pos[1];
        focus[2] = model->pos[2];
        focus[3] = model->pos[3];
        focus[1] = focus[1] + 100.0f;
    } else if (task->farCamera == 2) {
        GfxModelGetNodeWorldPosByIndex((GfxModel *)task->unit, &nodePos, 0);
        focus[0] = nodePos.x;
        focus[1] = nodePos.y;
        focus[2] = nodePos.z;
        focus[3] = nodePos.w;
        focus[1] = focus[1] + 200.0f;
    } else if (task->farCamera == 0 && unit != NULL) {
        GfxModelGetNodeWorldPos(&unit->base, &bipPos, "Bip01");
        focus[0] = bipPos.x;
        focus[1] = bipPos.y;
        focus[2] = bipPos.z;
        focus[3] = bipPos.w;
    }

    if (task->farCamera == 0) {
        task->camDistance = 300.0f;
        c = __builtin_cosf(task->camBlend * task->camBlend * 3.14159274f);
        zoom = 1.14999998f - (1.0f - c) * 0.5f * 0.400000006f;
        if (unit != NULL) {
            zoom = unit->height * 0x1.111112p-7f * zoom;
        }
        task->camera.frustumScale = zoom;
    }

    if (task->step < 4) {
        /* eye = focus + camDir * camDistance (xyz; w from focus) */
        float dist = task->camDistance;

        task->camera.eye[0] = focus[0] + task->camDir[0] * dist;
        task->camera.eye[1] = focus[1] + task->camDir[1] * dist;
        task->camera.eye[2] = focus[2] + task->camDir[2] * dist;
        task->camera.eye[3] = focus[3];
    }
    task->camera.target[0] = focus[0];
    task->camera.target[1] = focus[1];
    task->camera.target[2] = focus[2];
    task->camera.target[3] = focus[3];
    GfxCameraUpdate(&task->camera, 3);
    if (task->farCamera == 0 && task->step < 4) {
        UpdateWorldEffects();
    }
}
