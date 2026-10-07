// bdc 0x0888dcb4 BtlAiSetupKind
#include "bdc.h"

/* Kind-dependent setup of `BtlAi` (from `BtlAiCreate` with the owner's kind):
   references and fetches the rule script of `kind` from the script cache
   (`BtlAiComScriptCacheAddRef`, `BtlAiComScriptCacheGet` → `script`), attaches its four rule
   groups to the four command channels (`BtlAiChannelSetRules`), resets (`BtlAiResetAll`),
   builds the per-species parameter objects at `params` (`BtlAiCreateKindParamSet`), applies the
   level (`BtlAiApplyLevel`), the rule mode and the target pickers (`BtlAiUpdateRuleMode`,
   `BtlAiSetTargetPickers`), then sets `firstFrame` = 1, clears `guardRoll`/`dodgePressed`, and
   sets `startLimit` = 60.0, `startElapsed` = 0, `startExpired` = 0. */

void BtlAiSetupKind(BtlAi *self, s32 kind)
{
    BtlAiComScript *script;
    s32 i;

    BtlAiComScriptCacheAddRef(self->scriptCache, kind);
    self->script = BtlAiComScriptCacheGet(self->scriptCache, kind);
    for (i = 0; i < 4; i++) {
        script = (BtlAiComScript *)self->script;
        BtlAiChannelSetRules(&self->channels[i], &script->groups[i]);
    }
    BtlAiResetAll(self);
    BtlAiCreateKindParamSet(self->scriptCache, (void **)&self->params, kind);
    BtlAiApplyLevel(self);
    BtlAiUpdateRuleMode(self);
    BtlAiSetTargetPickers(self);
    self->firstFrame = 1;
    self->guardRoll = 0;
    self->dodgePressed = 0;
    /* The binary stores +0x944 as one word built from an uninitialised stack word with its low
       byte cleared, so the padding bytes +0x945..+0x947 receive indeterminate stack contents;
       only startExpired is meaningful. */
    self->startLimit = 60.0f;
    self->startElapsed = 0.0f;
    self->startExpired = 0;
}
