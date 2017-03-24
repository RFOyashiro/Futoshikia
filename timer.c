#include "timer.h"

/**
 * \fn getTimeElapsedInMilliseconds()
 * \author Julien TEULLE
 * \brief Calculate the time elapsed between begin and end in 
 * milliseconds.
 * \return The value of this time
 */
void getTimeElapsed(clock_t begin, clock_t end, int Result[])
{
    CalcTotalTime((((double)(end - begin)) / CLOCKS_PER_SEC) * 1000.000, Result);
}

/**
 * \fn CalcMinute (double Time)
 * \author Kurt SAVIO
 * \brief Calculate how many minute there are in "Time" milliseconds
 * \param Time : a time in milliseconds
 * \return minutes in "Time" ms.
 */
int CalcMinute (double Time)
{
    return Time / 60000;
}

/**
 * \fn CalcSeconde (double Time)
 * \author Kurt SAVIO
 * \brief Calculate how many second there are in "Time" milliseconds
 * \param Time : a time in milliseconds
 * \return seconds in "Time" ms.
 */
int CalcSeconde (double Time)
{
    return Time / 1000;
}

/**
 * \fn CalcTotalTime (double Time, int Result[])
 * \author Kurt SAVIO
 * \brief Convert a time in milliseconds to minute, seconds and milliseconds
 * \param Time : a time in milliseconds
 * \param Result[] : an array in which one minute, second and milliseconds will be stocked
 */
void CalcTotalTime (double Time, int Result[])
{
    int Minute = CalcMinute(Time);
    Time -= Minute * 60000;

    int Seconde = CalcSeconde(Time);
    Time -= Seconde * 1000;

    Result[0] = Minute;
    Result[1] = Seconde;
    Result[2] = Time;
}
