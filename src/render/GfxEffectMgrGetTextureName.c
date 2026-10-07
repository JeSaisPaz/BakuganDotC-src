// bdc 0x088238fc GfxEffectMgrGetTextureName
#include "bdc.h"

/* Returns the `index`-th texture name of an effect manager (`GfxEffectMgrCtor`)'s particle data
   (`.ptb`): the names are consecutive NUL-terminated strings starting at `data(+0x80) + 4` and
   ending before the definition table `+0x88`; NULL when `index ≥ count(+0x8c)` or the list ends.
    */

const char * GfxEffectMgrGetTextureName(GfxEffectMgr *mgr, s32 index)
{
    const char *s = (const char *)(mgr->data + 1);
    s32 i;

    if (index < mgr->textureCount && (const unsigned char *)s < mgr->defs) {
        for (i = 0; ; i++) {
            if (*s == '\0') {
                return NULL;
            }
            if (i == index) {
                return s;
            }
            s = s + strlen(s);
            s++;
            if (!((const unsigned char *)s < mgr->defs)) {
                break;
            }
        }
    }
    return NULL;
}
