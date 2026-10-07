// bdc 0x08864620 BtlBakuganInflictCounterHit
#include "bdc.h"

/* Makes `self` hit `target` back: marks `target`'s body collider as having a pending hit (flag
   0x10), copies the contact point from `self`'s body collider, and fills the hit record with
   attacker `self`, field `+0x164` = 1, `self`'s heading, hit id 0xb9, type 3 and a duration of 10
   (`strong` non-zero) or 5. */
void BtlBakuganInflictCounterHit(BtlBakugan *self, BtlBakugan *target, char strong)
{
    CollisionCollider *hit = target->collider0;
    CollisionCollider *own;

    hit->flags |= 0x10;
    own = self->collider0;
    hit->hitPos = own->hitPos;
    hit->hitType = 3;
    hit->hitKind = 0xb9;
    hit->hitHeading = self->base.rot[1];
    hit->hitAttacker = self;
    hit->hitParam164 = 1;
    if (strong != '\0') {
        hit->hitDuration = 10;
        return;
    }
    hit->hitDuration = 5;
}
