// bdc 0x089d8c30 CoreNodeLink
#include "bdc.h"

/* Links `node` into the sibling chain of `anchor` (`+0x00` = prev, `+0x04` = next). With a NULL
   `anchor` both links are cleared. With `mode == 0` the node is appended after the last node
   reachable through `anchor->next`; with `mode != 0` it is inserted directly behind `anchor`, and a
   node that followed the anchor is chained behind it (`next->prev = node`). */
void CoreNodeLink(CoreNode *node, CoreNode *anchor, u8 mode)
{
    CoreNode *next;

    if (anchor == NULL) {
        node->prev = NULL;
        node->next = NULL;
        return;
    }
    if (mode == 0) {
        while (anchor->next != NULL) {
            anchor = anchor->next;
        }
        anchor->next = node;
        node->prev = anchor;
        node->next = NULL;
        return;
    }
    next = anchor->next;
    anchor->next = node;
    node->prev = anchor;
    if (next == NULL) {
        node->next = NULL;
        return;
    }
    next->prev = node;
    node->next = next;
}
