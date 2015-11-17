#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <boost/chrono.hpp>

using namespace std;
using boost::chrono::system_clock;
using boost::chrono::nanoseconds;
using boost::chrono::microseconds;
using boost::chrono::milliseconds;
using boost::chrono::duration_cast;

enum TimeIncrement {MILLI, MICRO, NANO};

class Timer {
private:
    system_clock::time_point start;

public:
    explicit Timer();
    ~Timer();

    void resetClock();
    int checkClock(TimeIncrement inc);
};

#endif