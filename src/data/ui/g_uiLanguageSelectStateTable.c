// bdc 0x08a34024 g_uiLanguageSelectStateTable
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_uiLanguageSelectStateTable = {
    { .pfn = (void *)UiLanguageSelectStateLoad }, { .pfn = (void *)UiLanguageSelectStateBuild },
    { .pfn = (void *)UiLanguageSelectStateInput }, { .pfn = (void *)UiLanguageSelectStateNop },
    { .pfn = (void *)UiLanguageSelectStateFinish },
};
