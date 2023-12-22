#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <iostream>
#include <chrono>

class ProgressBar {
public:
    ProgressBar(int total);
    void update(int progress);
    void complete();

private:
    int total;
    int current;
    std::chrono::steady_clock::time_point startTime;
    void display() const;
};

#endif // PROGRESSBAR_H
