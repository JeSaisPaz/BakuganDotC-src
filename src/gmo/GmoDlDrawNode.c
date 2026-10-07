// bdc 0x089db418 GmoDlDrawNode
#include "bdc.h"

/* Draws the current node (`ctx->node`) of a GMO model into the display list unless its `visible`
   attribute is 0 (then returns `dirty` unchanged). ORs the context's previous node flags, the
   node's `flags` and `texMapDirty` into `dirty`, remembers the node flags, modulates the node
   colour when bit 0x100000 is dirty (`GmoDlModulateNodeColor`), clears `boneList`, then for
   each of the node's `partCount` part references sets `ctx->part` and draws it with
   `GmoDlDrawPart`. A reference whose value + 1 fits in 16 bits is an index into the model's
   0x10-byte part records (NULL when not below `partCount`), otherwise it is the part pointer
   itself. Returns the accumulated dirty mask. */
u32 GmoDlDrawNode(GmoDlContext *ctx, GmoModel *model, u32 dirty)
{
    GmoNode *node = ctx->node;
    void **parts;
    s32 count;

    if (node->visible == 0) {
        return dirty;
    }
    dirty = dirty | ctx->nodeFlags | node->flags | ctx->texMapDirty;
    ctx->nodeFlags = node->flags;
    if (dirty & 0x100000) {
        GmoDlModulateNodeColor(ctx);
        node = ctx->node;
    }
    ctx->boneList = NULL;
    parts = node->parts;
    for (count = node->partCount; count > 0; count--, parts++) {
        uintptr_t ref = (uintptr_t)*parts;
        void *part = *parts;

        if (((ref + 1) & ~(uintptr_t)0xffff) == 0) {
            /* 16-bit index into the model's 0x10-byte part records. */
            if ((ref & 0xffff) < model->partCount) {
                part = (u8 *)model->parts + ref * 0x10;
            } else {
                part = NULL;
            }
        }
        ctx->part = part;
        dirty = GmoDlDrawPart(ctx, model, dirty);
    }
    return dirty;
}
