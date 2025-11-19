#include <time.h>

#ifndef TIMING_H
#define TIMING_H

// Checks if the compiler is C++
#ifdef __cplusplus
// If C++, tell it to use C linkage (no name mangling)
extern "C"
{
#endif

    // The function declaration
    double getTimeStamp();
    double getTimeResolution();

#ifdef __cplusplus
    // Close the C linkage block
}
#endif

#endif // TIMING_H
