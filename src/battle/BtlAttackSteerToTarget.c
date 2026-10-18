// bdc 0x08877ab0 BtlAttackSteerToTarget
#include "bdc.h"

/* Homing step of an attack: looks up its target unit by `targetId` (`BtlFindBakuganById`).
   Without one it sets `vel` = (`dir` * `speed` in xyz, 0 in w) and returns 0. Otherwise it aims
   at the target's `pos` (+ `offset` xyz when non-NULL, + `heightOffset` in y), normalises the
   vector from the attack's `pos` to that point (xyz clamped to [-1, 1], w = 0; a zero vector
   gives a zero xyz), moves `dir` towards it by `turnRate` (`dir += (aim - dir) * turnRate`, all
   four lanes), renormalises `dir` the same way, sets `vel` = (`dir` * `speed`, 0) and, when
   `decayTurn` is set and `age` is above 45, multiplies `turnRate` by 0.96; returns 1. */
int BtlAttackSteerToTarget(float speed, float heightOffset, BtlAttack *self, char decayTurn,
                           float *offset)
{
    float aim[4];
    float lenSq;
    float k;
    float rate;
    BtlBakugan *target;

    target = (BtlBakugan *)BtlFindBakuganById(self->targetId);
    if (target == NULL) {
        self->vel[0] = VfMul(self->dir[0], speed);
        self->vel[1] = VfMul(self->dir[1], speed);
        self->vel[2] = VfMul(self->dir[2], speed);
        self->vel[3] = 0.0f;
        return 0;
    }
    aim[0] = target->base.pos[0];
    aim[1] = target->base.pos[1];
    aim[2] = target->base.pos[2];
    aim[3] = target->base.pos[3];
    if (offset != NULL) {
        aim[0] = VfAdd(aim[0], offset[0]);
        aim[1] = VfAdd(aim[1], offset[1]);
        aim[2] = VfAdd(aim[2], offset[2]);
    }
    aim[1] = aim[1] + heightOffset;

    /* aim = normalize(aim - pos) */
    aim[0] = VfSub(aim[0], self->pos[0]);
    aim[1] = VfSub(aim[1], self->pos[1]);
    aim[2] = VfSub(aim[2], self->pos[2]);
    lenSq = VfDot3(aim[0], aim[1], aim[2], aim[0], aim[1], aim[2]);
    k = VfIsZero(lenSq) ? 0.0f : VfRsq(lenSq);
    aim[0] = VfSat1(VfMul(aim[0], k));
    aim[1] = VfSat1(VfMul(aim[1], k));
    aim[2] = VfSat1(VfMul(aim[2], k));
    aim[3] = 0.0f;

    /* dir += (aim - dir) * turnRate */
    rate = self->turnRate;
    self->dir[0] = VfAdd(self->dir[0], VfMul(VfSub(aim[0], self->dir[0]), rate));
    self->dir[1] = VfAdd(self->dir[1], VfMul(VfSub(aim[1], self->dir[1]), rate));
    self->dir[2] = VfAdd(self->dir[2], VfMul(VfSub(aim[2], self->dir[2]), rate));
    self->dir[3] = VfAdd(self->dir[3], VfMul(VfSub(aim[3], self->dir[3]), rate));

    /* dir = normalize(dir) */
    lenSq = VfDot3(self->dir[0], self->dir[1], self->dir[2], self->dir[0], self->dir[1], self->dir[2]);
    k = VfIsZero(lenSq) ? 0.0f : VfRsq(lenSq);
    self->dir[0] = VfSat1(VfMul(self->dir[0], k));
    self->dir[1] = VfSat1(VfMul(self->dir[1], k));
    self->dir[2] = VfSat1(VfMul(self->dir[2], k));
    self->dir[3] = 0.0f;

    /* vel = dir * speed */
    self->vel[0] = VfMul(self->dir[0], speed);
    self->vel[1] = VfMul(self->dir[1], speed);
    self->vel[2] = VfMul(self->dir[2], speed);
    self->vel[3] = 0.0f;

    if (decayTurn != 0 && self->age > 45) {
        self->turnRate = self->turnRate * 0.959999979f; /* 0x3f75c28f */
    }
    return 1;
}
