// bdc 0x0888da9c BtlAiUpdateRuleMode
#include "bdc.h"

/* Sets the AI rule mode to 1 when script global variable 8 (`g_scriptGlobalVars`) equals 2,
   else 0; `BtlAiSelectRule` skips rules whose mode mask excludes it. */
void BtlAiUpdateRuleMode(BtlAi *self)
{
    self->ruleMode = (g_scriptGlobalVars[8] == 2);
}
