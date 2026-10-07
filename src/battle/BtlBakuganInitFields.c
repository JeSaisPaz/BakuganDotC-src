// bdc 0x0885decc BtlBakuganInitFields
#include "bdc.h"

/* Default-initialises the fields of a battle unit (`BtlBakuganCtor` runs it right after the
   embedded `BtlCombatState` constructor): zeroes about a hundred scalars (state words, flags,
   counters, pointers), copies `g_vecUp` into `groundNormal` and `orient`, sets the collision mask
   `collisionMask = 0x3fbf2700`, `adviceTimer = 0x1e0`, `comboDamageScale = 1.0f`, and zeroes the vec4 fields `tiltPush`, `groundPoint`, `base.velocity`,
   `spawnPoint`, `effectAnchor`, `statePos`, `guardAnchor` and `flashColor`.
   Returns nothing. */

/* sv.q of the bank's zero vector C720 */
#define ZERO_VEC4(dst) \
    do {               \
        (dst)[0] = 0.0f; \
        (dst)[1] = 0.0f; \
        (dst)[2] = 0.0f; \
        (dst)[3] = 0.0f; \
    } while (0)

#define COPY_VEC_UP(dst)      \
    do {                      \
        (dst)[0] = g_vecUp.x; \
        (dst)[1] = g_vecUp.y; \
        (dst)[2] = g_vecUp.z; \
        (dst)[3] = g_vecUp.w; \
    } while (0)

void BtlBakuganInitFields(BtlBakugan *self)
{
    int i;

    self->stencilRef = 0;
    self->targetId = 0;
    self->targetAux = 0;
    self->playerSlot = 0;
    self->unitTag = 0;
    self->isPlayer = 0;
    self->dashSpeed = 0.0f;
    COPY_VEC_UP(self->groundNormal);
    self->radius = 0.0f;
    ZERO_VEC4(self->tiltPush);
    self->collisionMask = 0x3fbf2700;
    self->height = 0.0f;
    ZERO_VEC4(self->groundPoint);
    self->groundProbed = 0;
    self->floorMaterial = 0;
    ZERO_VEC4(self->base.velocity);
    self->gravity = 0.0f;
    self->targetDistance = 0.0f;
    self->reserved42c = 0;
    self->footstep = 0;
    self->stateFlags = 0;
    self->flags = 0;
    COPY_VEC_UP(self->orient);
    self->airborneFrames = 0;
    self->charge = 0.0f;
    self->hitCount = 0;
    self->knockdownGauge = 0;
    self->noTurn = 0;
    self->targetNear = 0;
    self->attackMotionFlags = 0;
    self->combo = 0;
    self->bestCombo = 0;
    self->pendingComboHits = 0;
    self->comboTimer = 0;
    self->comboDamage = 0;
    self->hitTally = 0;
    self->lastCombo = 0;
    self->selectedArtSlot = 0;
    self->statusVisualBits = 0;
    self->auxNode = NULL;
    self->gravityHold = 0;
    self->forceFinalHit = 0;
    self->guardEffect = 0;
    self->guardAge = 0;
    self->blockTimer = 0;
    self->counterWindow = 0;
    self->attacker = NULL;
    self->counterPress = 0;
    self->spineNode = NULL;
    self->stateFrames = 0;
    self->ballEntryPending = 0;
    self->knockoutCount = 0;
    self->knockoutTally = 0;
    self->crystalBreakCount = 0;
    self->itemPickupCount = 0;
    self->meleeHitAttacker = NULL;
    self->slowMotionActive = 0;
    self->targetCloseness = 0.0f;
    self->linkedUnit = NULL;
    self->lockedAttacker = NULL;
    self->commands = 0;
    self->untargetable = 0;
    self->hpThreshold = 0.0f;
    self->voiceBank = 0;
    self->groundHeight = 0.0f;
    self->adviceFlag = 0;
    self->retargeted = 0;
    self->adviceTimer = 0x1e0;
    self->flinchGauge = 0;
    self->hitStunTimer = 0;
    self->hitQueueLocked = 0;
    self->hitQueueCount = 0;
    self->artCancelTimer = 0;
    self->status12Source = NULL;
    self->slopeDescent = 0.0f;
    self->koCameraDone = 0;
    for (i = 0; i < 4; i++) {
        self->legs[i] = NULL;
    }
    self->attackMotions = NULL;
    self->combos = NULL;
    self->motionDone = 0;
    self->hitWindowActive = 0;
    ZERO_VEC4(self->spawnPoint);
    self->respawnCountdown = 0;
    self->respawnProtect = 0;
    self->respawnStage = 0;
    self->statusTimer = 0;
    self->collider1 = NULL;
    self->collider0 = NULL;
    self->base.sound = NULL;
    self->shadow = NULL;
    self->input = NULL;
    self->motionTable = NULL;
    self->hpGauge = NULL;
    self->auxNode = NULL;
    self->weapon = NULL;
    self->stencilRef = 0;
    self->stats = NULL;
    self->attackCount = 0;
    self->subTimer = 0;
    self->subWait = 0;
    self->dashHeading = 0.0f;
    self->dashLift = 0.0f;
    ZERO_VEC4(self->effectAnchor);
    ZERO_VEC4(self->statePos);
    self->wallHitFrames = 0;
    self->hitShake = 0.0f;
    self->hitShakeAmplitude = 0.0f;
    self->inWater = 0;
    self->slowWalk = 0;
    self->inWater12To1B = 0;
    ZERO_VEC4(self->guardAnchor);
    self->attackCount = 0;
    self->contactObj = NULL;
    self->wingModel = NULL;
    ZERO_VEC4(self->flashColor);
    self->attackResult = 0;
    self->knockOutMode = 0;
    self->stage5HintFlag = 0;
    self->stage36HintFlag = 0;
    self->attackLevel = 0;
    self->defenseLevel = 0;
    self->airRecoverDelay = 0;
    self->comboDamageScale = 1.0f;
    self->counterOpening = 0;
}
