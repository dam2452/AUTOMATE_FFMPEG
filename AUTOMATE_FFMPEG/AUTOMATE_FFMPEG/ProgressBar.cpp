#include "ProgressBar.h"

ProgressBar::ProgressBar(int total) : total(total), current(0) {
    startTime = std::chrono::steady_clock::now();  // Zapisz czas startu
}

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
    int percent = int(float(current) / total * 100.0);

    std::cout << "Postep: " << percent << " %";

    if (current > 0) {
        auto now = std::chrono::steady_clock::now();
        auto timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        auto estimatedTotalTime = static_cast<long long>(timeElapsed) * total / current;
        auto remainingTime = estimatedTotalTime - timeElapsed;

        std::cout << ", Pozostaly czas: " << remainingTime << " s";
    }

    std::cout << "\r";
    std::cout.flush();
}
