// bdc 0x08899acc BtlBakuganRunBallEntry
#include "bdc.h"

/* Per-frame entry sequence of a spawned CPU battle unit (`BtlCpuUnit`): the Bakugan arrives as
   its closed ball model (`ballModel`, an `ActorBall` created by `BtlBakuganBeginBallEntry`) and
   pops open into the battle unit. Returns 1 when no entry is pending (`ballEntryPending == 0`),
   otherwise 0 (also while a cut-in runs, `BtlIsCutInRunning`), so `BtlCpuUnitUpdate` and
   `BtlUnitMode4Update` run the AI only afterwards. While the step `ballEntryStep` is below 0x17
   the gravity hold is refreshed to 4 every frame; `ballEntryTimer` counts the frames of a step.
   Step 0 (unless the input is disabled) gives the body collider timer -1 and active-hit flag
   (hit queries skip it), snaps the unit to the ground (`CollisionRaycastPoint` from 1000
   above), stores the ground point (`ctorVecs[1]`) and the ball point 150 above it (`ctorVecs[0]`)
   and enters state 0xf. With script bit 5 and a ball model (species 10 and 2 first show the ball's
   motion at frame 1), generic enemies 0x15..0x1f hatch from an attribute-coloured egg crystal
   unless `flag6bc` or script bit 3 is set (step 200: `ActorStageObjEggCrystalElementCtor` in
   `word6c8`, wait until `ActorStageObjValidate` drops it, then step 1); the others go to step 1.
   Steps 1-4 place the ball at the ball point turned by pi/2 - yaw, frame it with
   `BtlCameraFocusUnit` (script bit 5 and `flag6b0`), play sound 0xb and the ball's open motion
   (`ActorBallPlayMotion`), wait `g_btlBallEntryOpenWait` frames, then frame the unit (camera
   distance from `g_btlBallEntryCamParams`) and play sound 0x200012 once per value of
   `g_btlFrameCount` (`g_btlBallEntrySoundFrame`). Without a ball model step 0 goes straight to
   step 0x13 with alpha 0 and the velocity zeroed (the bank zero C720). Steps 0x13-0x15 spawn the
   attribute effect `0x6a + attribute` attached to the ball point and lift the unit by its hover
   height, hide the ball (alpha 0) and grow model node 1 from 0.01 to full scale ((1 - cos(pi t)) / 2),
   fading the attribute tint out and the alpha in while easing the position to the ground point.
   Without script bit 5 the entry then ends (step 100); otherwise steps 0x16-0x1b play the species
   roar (`g_btlUnitRoarSound`) and motion (0x10f; 0x10a for species 3; 0x31 for 8, at speed 0.6)
   for species 1..0x14 with the camera task's `flashTarget` at 0.8 until the motion reaches 0.99.
   The last step (any other value, after its timer runs out) ends the entry: collider hit flag and
   timer cleared, state 0, ball deleted (`CoreObjectDeferDelete`), `ballEntryPending = 0`, and
   `cpuEntryDone` set on the ally crystal when the camera task exists, script bit 5 is set and no
   egg crystal stands (`ActorStageObjCountStandingEggCrystals`).
   The VFPU code reads only the bank constants C720 (zero) and S703 (2/pi, so the quarter-turn
   cos/sin become cosf/sinf of the angle); no VFPU value crosses a call. */

int BtlBakuganRunBallEntry(BtlBakugan *self)
{
    BtlCpuUnit *unit = (BtlCpuUnit *)self;
    float ground[4];
    float tint[4];
    float spawnPos[4];
    ScePspFVector4 nodePos;
    ScePspFVector4 nodePos2;
    const VtblEntry *entry;
    GfxModel *ball;
    GmoModel *ballData;
    GmoNode *node;
    GfxEffectMgr *effectMgr;
    ActorStageObjEggCrystal *crystal;
    ActorStageObjEggCrystal *mem;
    ActorCrystal *ally;
    float *color;
    float angle;
    float distance;
    float c;
    float s;
    float *m;
    float grow;
    int attribute;
    int i;
    int motion;
    bool fromLow;
    bool entered;

    if (self->ballEntryPending == 0) {
        return 1;
    }
    if (BtlIsCutInRunning()) {
        return 0;
    }
    if (unit->ballEntryStep < 0x17) {
        self->gravityHold = 4;
    }
    switch (unit->ballEntryStep) {
    case 0:
        entered = false;
        if (self->input->disabled != 0) {
            return 0;
        }
        if (self->collider0 != NULL) {
            self->collider0->hitTimer = -1;
            self->collider0->flags |= 1;
        }
        ground[0] = self->base.pos[0];
        ground[1] = self->base.pos[1];
        ground[2] = self->base.pos[2];
        ground[3] = self->base.pos[3];
        ground[1] += 1000.0f;
        CollisionRaycastPoint(ground, ground);
        self->base.pos[0] = ground[0];
        self->base.pos[1] = ground[1];
        self->base.pos[2] = ground[2];
        self->base.pos[3] = ground[3];
        unit->ctorVecs[1][0] = self->base.pos[0];
        unit->ctorVecs[1][1] = self->base.pos[1];
        unit->ctorVecs[1][2] = self->base.pos[2];
        unit->ctorVecs[1][3] = self->base.pos[3];
        ground[1] += 150.0f;
        unit->ctorVecs[0][0] = ground[0];
        unit->ctorVecs[0][1] = ground[1];
        unit->ctorVecs[0][2] = ground[2];
        unit->ctorVecs[0][3] = ground[3];
        BtlBakuganSetState(self, 0xf, 0);
        if (CoreBitsetTest(5, g_scriptGlobalBits) && unit->ballModel != NULL) {
            if (self->base.base.unk08 == 10 || self->base.base.unk08 == 2) {
                ActorBallPlayMotion(0.0f, &unit->ballModel->base, 0, 0, true);
                GfxModelSwapMotionFrame(unit->ballModel, 1.0f);
                GfxModelUpdateMotion(unit->ballModel);
                GfxModelApplyMotion(unit->ballModel);
                ball = unit->ballModel;
                entry = &((const VtblEntry *)ball->base.vtable)[6];
                ((void (*)(void *, float))entry->fn)((u8 *)ball + entry->delta, 0.0f);
            }
            entered = true;
        }
        if (entered) {
            if (unit->flag6bc == 0 && self->base.base.unk08 >= 0x15 &&
                self->base.base.unk08 < 0x21 && self->base.base.unk08 != 0x20 &&
                !CoreBitsetTest(3, g_scriptGlobalBits)) {
                unit->ballEntryStep = 200;
            } else {
                unit->ballEntryStep++;
            }
        } else {
            unit->ballEntryStep = 0x13;
            unit->ballEntryTimer = 3;
            self->base.ambient[3] = 0.0f;
            self->base.velocity[0] = 0.0f;
            self->base.velocity[1] = 0.0f;
            self->base.velocity[2] = 0.0f;
            self->base.velocity[3] = 0.0f;
        }
        break;

    case 1:
        unit->ballModel->ambient[3] = 1.0f;
        ballData = unit->ballModel->data;
        angle = 1.57079637f - self->base.rot[1];
        if (!(angle <= 3.14159274f)) {
            angle -= 6.28318548f;
        } else if (angle <= -3.14159274f) {
            angle += 6.28318548f;
        }
        /* rootMatrix = scale (1, 1, 1) times the yaw rotation by angle (the listing builds the
           unit-scale diagonal and multiplies it in; the scale is 1, so the product is the
           rotation itself) */
        c = __builtin_cosf(angle);
        s = __builtin_sinf(angle);
        m = ballData->rootMatrix;
        m[0] = c;
        m[1] = 0.0f;
        m[2] = -s;
        m[3] = 0.0f;
        m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f;
        m[7] = 0.0f;
        m[8] = s;
        m[9] = 0.0f;
        m[10] = c;
        m[11] = 0.0f;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
        ball = unit->ballModel;
        m = &ball->data->rootMatrix[12];
        m[0] = unit->ctorVecs[0][0];
        m[1] = unit->ctorVecs[0][1];
        m[2] = unit->ctorVecs[0][2];
        m[3] = unit->ctorVecs[0][3];
        ball->pos[0] = m[0];
        ball->pos[1] = m[1];
        ball->pos[2] = m[2];
        ball->pos[3] = m[3];
        ActorBallSetHeading(self->base.rot[1], unit->ballModel);
        if (self->base.base.unk08 == 0x17) {
            unit->ballModel->rot[0] = 3.14159274f;
            unit->ballModel->rot[2] = 3.14159274f;
        }
        if (BtlCameraTaskExists() != 0 && CoreBitsetTest(5, g_scriptGlobalBits) &&
            unit->flag6b0 != 0) {
            BtlCameraFocusUnit(400.0f, 0.0f, 30.0f, (BtlMain *)BtlGetCameraTask(),
                               unit->ballModel, 1, 0, NULL);
            if (self->base.base.unk08 == 10 || self->base.base.unk08 == 2) {
                BtlCameraFocusUnit(200.0f, 60.0f, 13.0f, (BtlMain *)BtlGetCameraTask(),
                                   unit->ballModel, 0, 0, NULL);
            } else {
                BtlCameraFocusUnit(70.0f, 0.0f, 13.0f, (BtlMain *)BtlGetCameraTask(),
                                   unit->ballModel, 0, 0, NULL);
            }
        }
        unit->ballEntryTimer = 20;
        unit->ballEntryStep++;
        /* fallthrough */
    case 2:
        if (--unit->ballEntryTimer > 0) {
            return 0;
        }
        if (CoreBitsetTest(5, g_scriptGlobalBits) && unit->flag6b0 != 0) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0xb, 0, 0);
            }
        } else if (SndHasListener()) {
            SndEmitterCreateAtPos(SndGetListener(), 0xb, self->base.pos, 0, 1);
        }
        ActorBallPlayMotion(0.2f, &unit->ballModel->base, 0, 0, true);
        ball = unit->ballModel;
        entry = &((const VtblEntry *)ball->base.vtable)[6];
        ((void (*)(void *, float))entry->fn)((u8 *)ball + entry->delta, 1.0f);
        switch (self->base.base.unk08) {
        case 1:
        case 2:
        case 5:
        case 6:
        case 0x12:
            ball = unit->ballModel;
            entry = &((const VtblEntry *)ball->base.vtable)[6];
            ((void (*)(void *, float))entry->fn)((u8 *)ball + entry->delta, 0.5f);
            break;
        case 0x15:
            ball = unit->ballModel;
            entry = &((const VtblEntry *)ball->base.vtable)[6];
            ((void (*)(void *, float))entry->fn)((u8 *)ball + entry->delta, 0.8f);
            break;
        default:
            break;
        }
        unit->ballEntryTimer = 30;
        if (self->base.base.unk08 != 0 && self->base.base.unk08 < 0x21) {
            unit->ballEntryTimer = g_btlBallEntryOpenWait[self->base.base.unk08];
        }
        unit->ballEntryStep++;
        break;

    case 3:
        if (--unit->ballEntryTimer > 0) {
            return 0;
        }
        unit->ballEntryTimer = 3;
        unit->ballEntryStep++;
        break;

    case 4:
        if (--unit->ballEntryTimer > 0) {
            return 0;
        }
        if (BtlCameraTaskExists() != 0 && CoreBitsetTest(5, g_scriptGlobalBits) &&
            unit->flag6b0 != 0) {
            if (self->base.base.unk08 == 0xf) {
                distance = 450.0f;
            } else if (self->base.base.unk08 == 0x20) {
                distance = 500.0f;
            } else {
                distance = g_btlBallEntryCamParams[self->base.base.unk08][0] *
                           self->combat.stats->modelScale;
            }
            BtlCameraFocusUnit(distance, 100.0f, 40.0f, (BtlMain *)BtlGetCameraTask(), self, 0, 0,
                               NULL);
        }
        if (g_btlFrameCount != g_btlBallEntrySoundFrame) {
            if (CoreBitsetTest(5, g_scriptGlobalBits) && unit->flag6b0 != 0) {
                if (SndHasManager()) {
                    SndManagerPlay(SndGetManager(), 0x200012, 0, 0);
                }
            } else if (SndHasListener()) {
                SndEmitterCreateAtPos(SndGetListener(), 0x200012, self->base.pos, 0, 1);
            }
        }
        g_btlBallEntrySoundFrame = g_btlFrameCount;
        unit->ballEntryStep = 0x13;
        unit->ballEntryTimer = 3;
        self->base.ambient[3] = 0.0f;
        self->base.velocity[0] = 0.0f;
        self->base.velocity[1] = 0.0f;
        self->base.velocity[2] = 0.0f;
        self->base.velocity[3] = 0.0f;
        /* fallthrough */
    case 0x13:
        if (--unit->ballEntryTimer > 0) {
            return 0;
        }
        effectMgr = g_worldEffectMgr;
        entry = &((const VtblEntry *)self->base.base.vtable)[20];
        attribute = ((int (*)(void *))entry->fn)((u8 *)self + entry->delta);
        GfxEffectSpawnAttached(effectMgr, attribute + 0x6a, unit->ctorVecs[0]);
        self->base.pos[1] += self->combat.stats->hoverHeight;
        unit->ballEntryStep = 0x14;
        unit->ballEntryTimer = 5;
        break;

    case 0x14:
        if (--unit->ballEntryTimer > 0) {
            return 0;
        }
        if (unit->ballModel != NULL) {
            unit->ballModel->ambient[3] = 0.0f;
        }
        GfxModelGetNodeWorldPosByIndex(&self->base, &nodePos, 1);
        unit->ctorVecs[1][0] = self->base.pos[0];
        unit->ctorVecs[1][1] = self->base.pos[1];
        unit->ctorVecs[1][2] = self->base.pos[2];
        unit->ctorVecs[1][3] = self->base.pos[3];
        if (self->base.base.unk08 != 0x1f) {
            self->base.pos[1] = self->base.pos[1] - (nodePos.y - self->base.pos[1]);
        }
        node = GfxModelGetNode(&self->base, 1);
        node->scaleW = 0.0f;
        node->scale[0] = 0.01f;
        node->scale[1] = 0.01f;
        node->scale[2] = 0.01f;
        unit->ballEntryGrow = 0.0f;
        unit->ballEntryStep = 0x15;
        break;

    case 0x15:
        grow = unit->ballEntryGrow + 0.08f;
        unit->ballEntryGrow = grow;
        angle = grow * 3.14159274f;
        c = __builtin_cosf(angle);
        grow = (1.0f - c) * 0.5f;
        node = GfxModelGetNode(&self->base, 1);
        node->scale[0] = grow;
        node->scale[1] = grow;
        node->scale[2] = grow;
        node->scaleW = 0.0f;
        color = BtlBakuganGetAttributeColor(self);
        tint[0] = color[0];
        tint[1] = color[1];
        tint[2] = color[2];
        tint[3] = 1.0f - grow;
        entry = &((const VtblEntry *)self->base.base.vtable)[4];
        ((void (*)(void *, float *))entry->fn)((u8 *)self + entry->delta, tint);
        self->base.ambient[3] += (1.0f - self->base.ambient[3]) * 0.3f;
        GfxModelGetNodeWorldPosByIndex(&self->base, &nodePos2, 1);
        /* pos += (ground point - pos) * ballEntryGrow */
        grow = unit->ballEntryGrow;
        for (i = 0; i < 4; i++) {
            self->base.pos[i] =
                self->base.pos[i] + (unit->ctorVecs[1][i] - self->base.pos[i]) * grow;
        }
        if (unit->ballEntryGrow < 1.0f) {
            return 0;
        }
        node = GfxModelGetNode(&self->base, 1);
        node->scale[0] = 1.0f;
        node->scale[1] = 1.0f;
        node->scale[2] = 1.0f;
        node->scaleW = 0.0f;
        self->base.fogEnabled = 0;
        unit->ballEntryStep++;
        self->base.ambient[3] = 1.0f;
        /* fallthrough */
    case 0x16:
        if (!CoreBitsetTest(5, g_scriptGlobalBits)) {
            unit->ballEntryStep = 100;
            return 0;
        }
        unit->ballEntryStep++;
        /* fallthrough */
    case 0x17:
        unit->ballEntryTimer = 5;
        unit->ballEntryStep++;
        /* fallthrough */
    case 0x18:
        if (--unit->ballEntryTimer > 0) {
            return 0;
        }
        unit->ballEntryStep++;
        /* fallthrough */
    case 0x19:
        if (self->base.base.unk08 == 0 || self->base.base.unk08 >= 0x15) {
            unit->ballEntryTimer = 10;
            unit->ballEntryStep = 100;
            return 0;
        }
        if (CoreBitsetTest(5, g_scriptGlobalBits) && unit->flag6b0 != 0) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), g_btlUnitRoarSound[self->base.base.unk08], 0, 0);
            }
        } else if (SndHasListener()) {
            SndEmitterCreateAtPos(SndGetListener(), (s32)g_btlUnitRoarSound[self->base.base.unk08],
                                  self->base.pos, 0, 1);
        }
        ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.8f;
        motion = 0x10f;
        if ((s32)self->base.base.unk08 < 4) {
            if ((s32)self->base.base.unk08 >= 3) {
                motion = 0x10a;
            }
        } else if (self->base.base.unk08 == 8) {
            motion = 0x31;
            entry = &((const VtblEntry *)self->base.base.vtable)[6];
            ((void (*)(void *, float))entry->fn)((u8 *)self + entry->delta, 0.6f);
        }
        BtlBakuganPlayMotion(0.1f, self, motion, 0, 1);
        unit->ballEntryTimer = 0;
        unit->ballEntryStep++;
        break;

    case 0x1a:
        if (self->base.motionEnded == 0 && !GfxModelMotionReached(&self->base, 0.99f)) {
            return 0;
        }
        entry = &((const VtblEntry *)self->base.base.vtable)[6];
        ((void (*)(void *, float))entry->fn)((u8 *)self + entry->delta, 1.0f);
        ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
        unit->ballEntryTimer = 5;
        unit->ballEntryStep++;
        break;

    case 0x1b:
        if (--unit->ballEntryTimer > 0) {
            return 0;
        }
        unit->ballEntryStep = 100;
        break;

    case 200:
        crystal = NULL;
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = (ActorStageObjEggCrystal *)MemAlloc((s32)sizeof(ActorStageObjEggCrystal), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (mem != NULL) {
            spawnPos[0] = self->base.pos[0];
            spawnPos[1] = self->base.pos[1];
            spawnPos[2] = self->base.pos[2];
            spawnPos[3] = self->base.pos[3];
            entry = &((const VtblEntry *)self->base.base.vtable)[20];
            attribute = ((int (*)(void *))entry->fn)((u8 *)self + entry->delta);
            ActorStageObjEggCrystalElementCtor(mem, spawnPos, attribute);
            crystal = mem;
        }
        unit->word6c8 = crystal;
        unit->ballEntryStep++;
        /* fallthrough */
    case 201:
        if (ActorStageObjValidate(unit->word6c8) == NULL) {
            unit->ballEntryStep = 210;
        }
        break;

    case 210:
        unit->ballEntryStep = 1;
        break;

    default:
        /* step 100 (and any other value): wait out the timer, then end the entry */
        if (--unit->ballEntryTimer >= 0) {
            return 0;
        }
        if (self->collider0 != NULL) {
            self->collider0->hitTimer = 0;
            self->collider0->flags &= ~1u;
        }
        BtlBakuganSetState(self, 0, 0);
        if (unit->ballModel != NULL) {
            CoreObjectDeferDelete(&unit->ballModel->base, 0);
        }
        if (BtlCameraTaskExists() != 0 && CoreBitsetTest(5, g_scriptGlobalBits) &&
            ActorStageObjCountStandingEggCrystals() == 0) {
            ally = (ActorCrystal *)BtlBakuganListFind(unit->ally);
            if (ally != NULL) {
                ally->cpuEntryDone = 1;
            }
        }
        self->ballEntryPending = 0;
        break;
    }
    return 0;
}
