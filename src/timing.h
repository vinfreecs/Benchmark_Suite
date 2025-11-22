#include <time.h>

#ifndef TIMING_H
#define TIMING_H

// TODO check why this is better than chrono
#ifdef __cplusplus

extern "C"
{
#endif

    double getTimeStamp();
    double getTimeResolution();

#ifdef __cplusplus
    
}
#endif

#endif // TIMING_H
