#include "ProgressBar.h"

ProgressBar::ProgressBar(int total) : total(total), current(0) {}

void ProgressBar::update(int progress) {
    current = progress;
    display();
}

void ProgressBar::complete() {
    current = total;
    display();
    std::cout << "\nPrzetwarzanie zakonczone.\n";
}

void ProgressBar::display() const {
    int width = 50; // szerokoœæ paska postêpu
    int pos = width * current / total;

    std::cout << "[";

    for (int i = 0; i < width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }

    std::cout << "] " << int(float(current) / total * 100.0) << " %\r";
    std::cout.flush();
}
