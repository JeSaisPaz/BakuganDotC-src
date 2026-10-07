// bdc 0x0888f7ec BtlAiSetGoalPoint
#include "bdc.h"

/* Starts a move-to-point order of the AI: resets the first command channel
   (BtlAiChannelReset), marks it running (phase 1), stores the goal as
   (point[0], 0, point[2], 0) and drops the target (BtlAiSetTarget(self, NULL)). */
void BtlAiSetGoalPoint(BtlAi *self, float *point)
{
    BtlAiChannelReset(&self->channels[0]);
    self->channels[0].phase = 1;
    self->goal[0] = point[0];
    self->goal[1] = 0.0f;
    self->goal[2] = point[2];
    self->goal[3] = 0.0f;
    BtlAiSetTarget(self, NULL);
}
