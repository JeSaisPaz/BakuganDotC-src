// bdc 0x08a323d8 CollisionSegmentGetCenter
#include "bdc.h"

/* Get-centre method of the segment shape (vtable `0x08af5564` entry 8, offset `+0x44`): returns
   `segment + 0x10`, its start point. */

ScePspFVector4 *CollisionSegmentGetCenter(SegmentShape *segment)

{
  return (ScePspFVector4 *)segment->start;
}

