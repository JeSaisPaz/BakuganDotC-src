// bdc 0x088e8254 ActorNpcSwitchRobotCheckSwitchHit
#include "bdc.h"

/* Called by `ActorBallCheckHit` when the ball hits a switch robot at `hitPos`: when the direction
   from the robot's head (matrix translation, +12 in y) to the hit lies strictly within 60 degrees of
   the robot's back axis (root matrix applied to -Z), i.e. its dot product is not <= cos(60),
   shuts the robot down: AI state 8, view cone hidden, head effects 0x29/0x2a removed, effect 0x45
   at the model matrix on `g_worldEffectMgr`, sound `0x2c0002c`, reports the placement's switch id
   to the field (`GameFieldCharSetQueueSwitchEvent`) and animates the node `psp_vxsrobo_swich`
   through `ActorNpcSwitchRobotSwitchWriteScrollV` (vec4 `switchScroll`, start 0.34).
   Otherwise does nothing.
   Both vectors are normalised with each lane clamped to [-1, 1]; a zero-length vector is scaled by
   the bank's 0 (S713). cos(60) is vcos.s of pi/3 times the bank's 2/pi (S703). */

void ActorNpcSwitchRobotCheckSwitchHit(ActorNpcSwitchRobot *self, const float *hitPos)
{
    float head[3];
    float toHit[3];
    float back[3];
    const float *m;
    float lenSq;
    float k;
    float dot;
    float cosLimit;

    head[0] = self->base.base.mtx[12];
    head[1] = self->base.base.mtx[13] + 12.0f;
    head[2] = self->base.base.mtx[14];

    /* toHit = normalize(hitPos - head), lanes saturated to [-1, 1] */
    toHit[0] = VfSub(hitPos[0], head[0]);
    toHit[1] = VfSub(hitPos[1], head[1]);
    toHit[2] = VfSub(hitPos[2], head[2]);
    lenSq = VfDot3(toHit[0], toHit[1], toHit[2], toHit[0], toHit[1], toHit[2]);
    k = VfIsZero(lenSq) ? 0.0f : VfRsq(lenSq);
    toHit[0] = VfSat1(VfMul(toHit[0], k));
    toHit[1] = VfSat1(VfMul(toHit[1], k));
    toHit[2] = VfSat1(VfMul(toHit[2], k));

    /* back = rootMatrix rows 0..2 applied to (0, 0, -1) (vtfm3.t, E form), normalised as above */
    m = self->base.base.base.data->rootMatrix;
    back[0] = VfDot3(m[0], m[4], m[8], 0.0f, 0.0f, -1.0f);
    back[1] = VfDot3(m[1], m[5], m[9], 0.0f, 0.0f, -1.0f);
    back[2] = VfDot3(m[2], m[6], m[10], 0.0f, 0.0f, -1.0f);
    lenSq = VfDot3(back[0], back[1], back[2], back[0], back[1], back[2]);
    k = VfIsZero(lenSq) ? 0.0f : VfRsq(lenSq);
    back[0] = VfSat1(VfMul(back[0], k));
    back[1] = VfSat1(VfMul(back[1], k));
    back[2] = VfSat1(VfMul(back[2], k));

    dot = VfDot3(toHit[0], toHit[1], toHit[2], back[0], back[1], back[2]);
    cosLimit = VfCosQuarter(1.04719758f /* pi/3 */ * 0.636619747f);
    if (dot <= cosLimit) {
        return;
    }
    self->base.aiState = 8;
    self->base.subStep = 0;
    ((ActorNpcViewCone *)self->base.viewCone)->visible = 0;
    ActorNpcShowHeadEffect(&self->base, 0x29, 0, 1);
    ActorNpcShowHeadEffect(&self->base, 0x2a, 0, 1);
    GfxEffectSpawnAtMatrix(g_worldEffectMgr, 0x45, self->base.base.mtx);
    ActorAddSoundEmitter(self, 0x2c0002c, 0, 0);
    GameFieldCharSetQueueSwitchEvent(g_gameFieldCharSet,
                                     ((GameFieldPlacedChar *)self->base.base.placement)->entry);
    self->switchScroll[1] = 0.0f;
    self->switchScroll[0] = 0.34f;
    self->switchScroll[2] = 0.0f;
    self->switchScroll[3] = 0.0f;
    GfxModelSetMaterialAnimCallback(&self->base.base.base, "psp_vxsrobo_swich",
                                    (void *)ActorNpcSwitchRobotSwitchWriteScrollV,
                                    self->switchScroll);
}
