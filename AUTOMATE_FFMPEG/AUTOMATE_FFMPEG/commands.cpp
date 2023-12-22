#include "commands.h"
#include <iostream>
#include <array>
#include <memory>

#ifdef _WIN32
// Implementacja dla Windows
std::string Commands::execute(const std::string& cmd, bool filterFFmpegSpeed) {
    std::array<char, 128> buffer;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    std::string result;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        std::string line = buffer.data();

        if (filterFFmpegSpeed) {
            if (line.find("speed=") != std::string::npos) {
                // Wyœwietla tylko linie zawieraj¹ce "speed"
                std::cout << line.substr(line.find("speed=")) << std::endl;
                break;
            }
        }
        else {
            result += line;
        }
    }

    return result;
}

#else
// Implementacja dla Linux
std::string Commands::execute(const std::string& cmd, bool filterFFmpegSpeed) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        std::string line = buffer.data();
        if (filterFFmpegSpeed) {
            if (line.find("speed=") != std::string::npos) {
                std::cout << line; // Wyœwietla tylko linie ze "speed"
            }
        }
        else {
            result += line;
        }
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
