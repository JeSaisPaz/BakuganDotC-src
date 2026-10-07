// bdc 0x08aa3f28 g_scriptOpTable40
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_scriptOpTable40 = {
    { .pfn = (void *)ScriptOpPlayMovie }, { .pfn = (void *)ScriptOpMessageBox },
    { .pfn = (void *)ScriptOpFade }, { .pfn = (void *)ScriptOpPackage },
    { .pfn = (void *)ScriptOpSound }, { .pfn = (void *)ScriptOpBgmPlayer },
    { .pfn = (void *)ScriptOpSprite }, { .pfn = (void *)ScriptOpTextureCmd },
    { .pfn = (void *)ScriptOpFrameSkip },
};
