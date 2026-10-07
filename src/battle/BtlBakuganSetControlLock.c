// bdc 0x08863378 BtlBakuganSetControlLock
#include "bdc.h"

/* Sets the input-disabled byte of the unit's input controller (`self->input->disabled = lock`;
   `BtlInputReadActions` returns no actions while it is set). For non-player units with a body
   collider (`collider0`, a `CollisionCollider`): `lock != 0` first empties the buffered combo inputs
   (`BtlBakuganClearComboInputs`) and the frame's command flags, then sets bit 0 of the collider's
   `flags` and zeroes its `hitTimer`; `lock == 0` clears bit 0 and zeroes `hitTimer`. */
void BtlBakuganSetControlLock(BtlBakugan *self, u8 lock)
{
    CollisionCollider *body;

    self->input->disabled = lock;
    if (lock != 0) {
        if (self->isPlayer != 0) {
            return;
        }
        BtlBakuganClearComboInputs(self);
        self->commands = 0;
        body = self->collider0;
        if (body != NULL) {
            body->flags |= 1;
            body->hitTimer = 0;
        }
    } else {
        if (self->isPlayer != 0) {
            return;
        }
        body = self->collider0;
        if (body != NULL) {
            body->flags &= ~1u;
            body->hitTimer = 0;
        }
    }
}
