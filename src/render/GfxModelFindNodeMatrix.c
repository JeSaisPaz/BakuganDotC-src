// bdc 0x089defa8 GfxModelFindNodeMatrix
#include "bdc.h"

/* Returns the local matrix (`node+0x80`) of the node `name` (`GfxModelFindNode`), or the identity
   matrix at `0x08b001c0` when it does not exist. */

ScePspFMatrix4 *GfxModelFindNodeMatrix(GfxModel *self, const char *name) {
    GmoNode *node = (GmoNode *)GfxModelFindNode(self, name);
    if (node != NULL) {
        return (ScePspFMatrix4 *)node->localMatrix;
    }
    return &g_gfxIdentityMatrix;
}
