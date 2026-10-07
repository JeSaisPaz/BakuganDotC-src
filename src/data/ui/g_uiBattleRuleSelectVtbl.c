// bdc 0x08af4e5c g_uiBattleRuleSelectVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiBattleRuleSelectVtbl = {
    {0}, { .fn = (void *)UiBattleRuleSelectDtor }, { .fn = (void *)UiBattleRuleSelectUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiBattleRuleSelectDraw },
    { .fn = (void *)UiBattleRuleSelectSetField }, { .fn = (void *)UiBattleRuleSelectGetField },
};
