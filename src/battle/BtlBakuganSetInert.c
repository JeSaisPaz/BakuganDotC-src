// bdc 0x088605c4 BtlBakuganSetInert
#include "bdc.h"

/* Makes a unit inert (`on != 0`) or active again. With `on` set: marks its body collider
   (`collider0`) as permanently hit (`flags` bit 0, `hitTimer = -1`), which `CollisionHitQuery`
   skips, sets 0x40 in its push collider's (`collider1`) `flags` so
   `BtlBakuganPushApartFromUnits` ignores it, and sets the input controller's `disabled` byte
   (see `BtlInputReadActions`). With `on` clear: clears both flag bits, sets `hitTimer` to 0 and
   clears `disabled`. A NULL collider is skipped. */
void BtlBakuganSetInert(BtlBakugan *self, u8 on)
{
    CollisionCollider *body = (CollisionCollider *)self->collider0;
    CollisionCollider *push;

    if (on != 0) {
        if (body != NULL) {
            body->flags |= 1;
            body->hitTimer = -1;
        }
        push = (CollisionCollider *)self->collider1;
        if (push != NULL) {
            push->flags |= 0x40;
        }
        self->input->disabled = 1;
        return;
    }
    if (body != NULL) {
        body->flags &= ~1u;
        body->hitTimer = 0;
    }
    push = (CollisionCollider *)self->collider1;
    if (push != NULL) {
        push->flags &= ~0x40u;
    }
    self->input->disabled = 0;
}
