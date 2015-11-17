#include "timer.hpp"

Timer::Timer() : start(system_clock::now()) {

}
Timer::~Timer() {

}

void Timer::resetClock() {
    start = system_clock::now();
}

int Timer::checkClock(TimeIncrement inc) {
    switch (inc) {
        case NANO:
            return duration_cast<nanoseconds>(system_clock::now() - this->start).count();
        case MICRO:
            return duration_cast<microseconds>(system_clock::now() - this->start).count();
        case MILLI:
            return duration_cast<milliseconds>(system_clock::now() - this->start).count();
        default:
            fprintf(stderr, "Timer::checkClock ERROR invalid time increment %d\n",
                inc);
            exit(1);
    }
}