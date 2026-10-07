// bdc 0x089bea68 CoreStopwatchLap
#include "bdc.h"

/* Reads the current local time and computes minutes, seconds and microseconds elapsed since
   `CoreStopwatchStart``(index)` (normalising negative parts by adding 60/60/1000000), but the
   three results are not stored or printed: the display code was compiled out of this release
   build, so the function has no visible effect apart from the RTC call. Ignores indices >=
   `g_coreStopwatchCount`. */
void CoreStopwatchLap(s32 index)
{
    ScePspDateTime now;
    const ScePspDateTime *start;
    int minutes;
    int seconds;
    int micros;

    if (index < g_coreStopwatchCount) {
        sceRtcGetCurrentClockLocalTime(&now);
        start = &g_coreStopwatchSlots[index];
        minutes = (int)now.minute - (int)start->minute;
        seconds = (int)now.second - (int)start->second;
        micros = (int)now.microsecond - (int)start->microsecond;
        while (minutes < 0) {
            minutes += 60;
        }
        while (seconds < 0) {
            seconds += 60;
        }
        while (micros < 0) {
            micros += 1000000;
        }
        (void)minutes;
        (void)seconds;
        (void)micros;
    }
}
