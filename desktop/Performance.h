#ifndef H_PERFORMANCE
#define H_PERFORMANCE

#include <chrono>
#include "config.h"

typedef std::chrono::time_point<std::chrono::high_resolution_clock> perfTime_t;
typedef std::chrono::duration<long double> stepTime_t;

class Performance
{
public:
    void startMeassurement();
    void endMeassuremet();

    void printResult();
    
private:
    perfTime_t start;
    perfTime_t end;

    int meassurements = 0;
    stepTime_t total = stepTime_t(0);
    stepTime_t average;
    stepTime_t low = stepTime_t(std::numeric_limits<long double>::max());
    stepTime_t high = stepTime_t(std::numeric_limits<long double>::min());
};
#endif
