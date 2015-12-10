#include "Performance.h"

#include <iostream>

#ifdef LOG_PERFORMANCE
void Performance::startMeassurement()
{
    start = std::chrono::high_resolution_clock::now();
}
void Performance::endMeassuremet()
{
    end = std::chrono::high_resolution_clock::now();

    ++meassurements;
}
void Performance::printResult() 
{
    //Calculating average, min and max
    stepTime_t thisTime = std::chrono::duration<long double>(end-start);
    
    if(thisTime < low)
    {
        low = thisTime;
    }
    if(thisTime > high)
    {
        high = thisTime;
    }

    total += thisTime;
    average = total/(long double)meassurements;
    
    //Print the result
    std::cout << 
        "Frame time " << 
        std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() <<
        " average " << 
        std::chrono::duration_cast<std::chrono::microseconds>(average).count() <<
        " min " << 
        std::chrono::duration_cast<std::chrono::microseconds>(low).count() <<
        " max " << 
        std::chrono::duration_cast<std::chrono::microseconds>(high).count() <<
        " microseconds " << 
        std::endl;
}

#else

void Performance::startMeassurement()
{
    start = std::chrono::high_resolution_clock::now();
}
void Performance::endMeassuremet()
{
    end = std::chrono::high_resolution_clock::now();
}
void Performance::printResult() 
{
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
}
#endif

