#pragma once

#include <tuple>

class Timer
{
public:

    Timer();

    Timer(const Timer&) = delete;

    Timer(Timer&&) noexcept = default;

    Timer& operator=(Timer&) = delete;

    Timer& operator=(Timer&&) noexcept = default;

public:

    std::tuple<int, int, int> ticktock();

private:

    int seconds_;
    int minutes_;
    int hours_;
};
