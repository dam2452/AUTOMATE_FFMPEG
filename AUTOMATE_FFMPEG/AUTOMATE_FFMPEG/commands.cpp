#include "commands.h"
#include <iostream>
#include <array>
#include <memory>

#ifdef _WIN32
// Implementacja dla Windows
std::string Commands::execute(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("_popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
}
    return result;
}
#else
// Implementacja dla Linux
std::string Commands::execute(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
#endif

std::string Commands::checkFFmpegVersion() {
    try {
        std::string output = execute("ffmpeg -version");
        size_t pos = output.find("ffmpeg version");
        if (pos != std::string::npos) {
            return output.substr(pos, output.find('\n', pos) - pos);
        }
        else {
            return "not_installed";
        }
    }
    catch (const std::runtime_error& e) {
        return "error_executing_command";
    }
}


void Commands::installFFmpegWindows() {
    execute("winget install -e --id FFmpeg.FFmpeg");
}

void Commands::installFFmpegLinux() {
    execute("sudo apt-get update && sudo apt-get install ffmpeg");
}

void Commands::manageFFmpegInstallation() {
    std::string ffmpegVersion = checkFFmpegVersion();

    if (ffmpegVersion == "not_installed" || ffmpegVersion == "error_executing_command") {
        std::cout << "FFmpeg nie jest zainstalowany." << std::endl;
        std::cout << "Czy chcesz zainstalowaæ FFmpeg? [tak/nie]: ";
        std::string odpowiedz;
        std::cin >> odpowiedz;

        if (odpowiedz == "tak") {
#ifdef _WIN32
            installFFmpegWindows();
#else
            installFFmpegLinux();
#endif
        }
    }
    else {
        std::cout << "Zainstalowana wersja FFmpeg: " << ffmpegVersion << std::endl;
    }
}
