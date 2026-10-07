// bdc 0x0885b150 BtlLoadRequestStep
#include "bdc.h"

/* Loading state machine of one load request, advanced as far as it gets per call (steps above 4,
   or negative, do nothing):
   0 sets `loading` and loads the unit's model/motion packages (`BtlLoadBakuganAssetsStep`);
   1 registers the kind's motion file (`g_charMotionFiles`, `GmoMotionLoadFile`) unless its
   first motion name (`g_charMotionNameLists`) is already known, stopping when the kind has no
   name list;
   2 with rule mode 1 (script variable 8) loads sphere package `0x57 + kind` for kinds 1..20
   (`BtlLoadSpherePackage` into `packages[2]`), on stage 0x18 only for the kind stored in
   profile word 3;
   3 under the same mode/stage condition loads the extra sphere package 0x67 (kind 2) or 0x60
   (kind 10) into `packages[3]`;
   4 clears `loading`, sets `done` and deletes the motion package `packages[1]` (virtual deleting
   destructor, flags 3).
   A load that is not finished yet returns and retries the same step on the next call. */
void BtlLoadRequestStep(BtlLoadRequest *req)
{
    char **motionNames;
    IoLzsPackage *pkg;

    switch (req->step) {
    case 0:
        req->loading = 1;
        if (!BtlLoadBakuganAssetsStep(req->kind, &req->packages[0], &req->packages[1])) {
            return;
        }
        req->step++;
        /* fallthrough */
    case 1:
        motionNames = g_charMotionNameLists[req->kind];
        if (motionNames == NULL) {
            return;
        }
        if (GmoMotionIndexOfName(GmoMotionMgrGet(), motionNames[0]) == -1) {
            GmoMotionLoadFile(GmoMotionMgrGet(), g_charMotionFiles[req->kind]);
        }
        req->step++;
        /* fallthrough */
    case 2:
        if (g_scriptGlobalVars[8] == 1 && req->kind > 0 && req->kind < 0x15) {
            if (g_scriptGlobalVars[1] != 0x18) {
                if (!BtlLoadSpherePackage(&req->packages[2], req->kind + 0x57)) {
                    return;
                }
            } else if ((u32)req->kind == SaveProfileGetWord(SaveGetProfile(), 3)) {
                if (!BtlLoadSpherePackage(&req->packages[2], req->kind + 0x57)) {
                    return;
                }
            }
        }
        req->step = 3;
        /* fallthrough */
    case 3:
        if (g_scriptGlobalVars[8] == 1 &&
            (g_scriptGlobalVars[1] != 0x18 ||
             (u32)req->kind == SaveProfileGetWord(SaveGetProfile(), 3))) {
            if (req->kind == 2) {
                if (!BtlLoadSpherePackage(&req->packages[3], 0x67)) {
                    return;
                }
            } else if (req->kind == 10) {
                if (!BtlLoadSpherePackage(&req->packages[3], 0x60)) {
                    return;
                }
            }
        }
        req->step = 4;
        /* fallthrough */
    case 4:
        break;
    default:
        return;
    }

    req->loading = 0;
    req->done = 1;
    pkg = req->packages[1];
    if (pkg != NULL) {
        const VtblEntry *vtbl = pkg->base.vtable;
        const VtblEntry *dtor = &vtbl[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)pkg + dtor->delta, 3);
        req->packages[1] = NULL;
    }
    req->step++;
}
