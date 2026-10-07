// bdc 0x089d8ca8 CoreNodeSpliceChain
#include "bdc.h"

/* Splices the chain that starts at `chain` into the sibling chain right behind `anchor`:
   `anchor->next = chain`, `chain->prev = anchor`; the nodes that used to follow `anchor` are
   re-attached behind the last node of `chain` (found by walking its `next` links), and the first
   of them gets `prev = chain` (the head of the spliced chain, not its last node, as the binary
   does). NULL `anchor` does nothing. */
void CoreNodeSpliceChain(CoreNode *chain, CoreNode *anchor)
{
    CoreNode *rest;
    CoreNode *last;

    if (anchor == NULL) {
        return;
    }
    rest = anchor->next;
    anchor->next = chain;
    chain->prev = anchor;

    last = chain;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = rest;

    if (rest != NULL) {
        rest->prev = chain;
    }
}
