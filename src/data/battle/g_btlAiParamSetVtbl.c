// bdc 0x08af62c8 g_btlAiParamSetVtbl
#include "bdc.h"

__typeof__(VtblEntry[12]) g_btlAiParamSetVtbl = {
    {0}, { .fn = (void *)BtlAiParamSetDtor }, { .fn = (void *)BtlAiParamSetLevel1DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel2DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel3DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel4DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel5DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel6DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel7DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel8DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel9DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel10DelayWeight },
};
