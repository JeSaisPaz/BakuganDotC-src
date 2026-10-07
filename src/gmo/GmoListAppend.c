// bdc 0x08a10958 GmoListAppend
#include "bdc.h"

/* Appends `node` at the end of the `GmoImage` list whose head pointer is `*head` (linked through
   `GmoImage.next`, `+4`); nothing when `head` or `node` is NULL. */

void GmoListAppend(GmoImage **head, GmoImage *node)
{
    GmoImage *cur;

    if (head == NULL || node == NULL) {
        return;
    }
    for (cur = *head; cur != NULL; cur = cur->next) {
        head = &cur->next;
    }
    *head = node;
}
