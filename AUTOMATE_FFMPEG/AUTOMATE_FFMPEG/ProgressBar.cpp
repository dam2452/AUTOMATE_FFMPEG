#include "ProgressBar.h"

ProgressBar::ProgressBar(int total) : total(total), current(0) {
    startTime = std::chrono::steady_clock::now();  // Save start time
}

void ProgressBar::update(int progress) {
    current = progress;
    display();
}

void ProgressBar::complete() {
    current = total;
    display();
    std::cout << "\nProcessing completed.\n";
}

void ProgressBar::display() const {
    int percent = int(float(current) / total * 100.0);

    std::cout << "\rProgress: " << percent << " %";  // We use '\r' to return the cursor

    if (current > 0 && current < total) {
        auto now = std::chrono::steady_clock::now();
        auto timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        auto estimatedTotalTime = static_cast<long long>(timeElapsed) * total / current;
        auto remainingTime = estimatedTotalTime - timeElapsed;

        // Convert time to hours, minutes, and seconds
        int hours = remainingTime / 3600;
        int minutes = (remainingTime % 3600) / 60;
        int seconds = remainingTime % 60;

        std::cout << ", Remaining time: ";
        if (hours > 0) std::cout << hours << " h ";
        if (hours > 0 || minutes > 0) std::cout << minutes << " min ";
        std::cout << seconds << " s ";
    }

    std::cout.flush();
}
