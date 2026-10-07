// bdc 0x089beb14 CoreRtcDiffMicroseconds
#include "bdc.h"

/* Returns the time from `start` to `end` in microseconds, comparing only the
   hour/minute/second/microsecond fields of two `ScePspDateTime`s; `end == NULL` means now
   (`sceRtcGetCurrentClockLocalTime`). Returns 0 for a NULL `start`.

   The carries are done with loops, not multiplies: hours fold into minutes, negative minutes are
   wrapped by +60 (no borrow), minutes fold into seconds, negative seconds wrapped by +60, then
   seconds are added as 1000000 µs one at a time; a negative microsecond total borrows seconds,
   and any seconds left after that are added with a multiply. */
int CoreRtcDiffMicroseconds(ScePspDateTime *start, ScePspDateTime *end)
{
    ScePspDateTime now;
    int hours;
    int minutes;
    int seconds;
    int usec;

    if (start == NULL) {
        return 0;
    }
    if (end == NULL) {
        sceRtcGetCurrentClockLocalTime(&now);
        end = &now;
    }
    hours = (int)end->hour - (int)start->hour;
    minutes = (int)end->minute - (int)start->minute;
    seconds = (int)end->second - (int)start->second;
    usec = (int)(end->microsecond - start->microsecond);

    for (; hours > 0; hours--) {
        minutes += 60;
    }
    while (minutes < 0) {
        minutes += 60;
    }
    for (; minutes > 0; minutes--) {
        seconds += 60;
    }
    while (seconds < 0) {
        seconds += 60;
    }
    for (; seconds > 0; seconds--) {
        usec += 1000000;
    }
    while (usec < 0) {
        usec += 1000000;
        seconds--;
    }
    if (seconds > 0) {
        usec += seconds * 1000000;
    }
    return usec;
}
