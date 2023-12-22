#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <iostream>
#include <string>

class ProgressBar {
public:
    ProgressBar(int total);
    void update(int progress);
    void complete();
    int getCurrent() const { return current; }

private:
    int total;
    int current;
    void display() const;
};

#endif // PROGRESSBAR_H
