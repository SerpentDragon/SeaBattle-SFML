#include "Timer.hpp"

Timer::Timer() : seconds_(0), minutes_(0), hours_(0) {}

std::tuple<int, int, int> Timer::ticktock()
{
    seconds_++;
    if (seconds_ % 60 == 0 && seconds_)
    {
        minutes_++;
        seconds_ = 0;
    }
    if (minutes_ % 60 == 0 && minutes_)
    {
        hours_++;
        seconds_ = minutes_ = 0;
    }

    return { seconds_, minutes_, hours_ };
}
