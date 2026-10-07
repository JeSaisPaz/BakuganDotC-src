// bdc 0x088635bc BtlBakuganUpdateCombo
#include "bdc.h"

/* Per-frame combo bookkeeping of a battle Bakugan, called by `BtlBakuganUpdate`: when hits are
   pending (`pendingComboHits`) it commits them (`BtlBakuganCommitPendingComboHits`); otherwise
   it counts `comboTimer` down and, when it was already <= 0, latches the finished combo length
   into `lastCombo`, charges the special-art gauges with it when non-zero
   (`BtlCombatChargeArtsFromCombo` on `combat`), adds `comboDamage + hitTally` to profile word
   `14 + playerSlot` (`SaveProfileAddWord`) when the battle rule mode (script global var 8) is 2,
   a camera task exists and profile word 2 is non-zero, resets the combo
   (`BtlBakuganResetCombo`), clears both totals and resets `comboDamageScale` to 1.0. */
void BtlBakuganUpdateCombo(BtlBakugan *self)
{
    s32 timer;
    s32 combo;
    s32 word;

    if (self->pendingComboHits != 0) {
        BtlBakuganCommitPendingComboHits(self);
        return;
    }
    timer = self->comboTimer;
    self->comboTimer = timer - 1;
    if (timer > 0) {
        return;
    }
    combo = self->combo;
    self->lastCombo = combo;
    if (combo != 0) {
        BtlCombatChargeArtsFromCombo(&self->combat, combo);
    }
    if (g_scriptGlobalVars[8] == 2) {
        word = self->playerSlot + 14;
        if (BtlCameraTaskExists()) {
            BtlGetCameraTask();
            if (SaveProfileGetWord(SaveGetProfile(), 2) != 0) {
                SaveProfileAddWord(SaveGetProfile(), word, self->comboDamage + self->hitTally);
            }
        }
    }
    BtlBakuganResetCombo(self);
    self->comboDamage = 0;
    self->hitTally = 0;
    self->comboDamageScale = 1.0f;
}
