#include <iostream>
#include <string>
#include <vector>
#include "commands.h"
#include "menu.h"
#include "FileProcessor.h"
#include "FFProbe.h"
#include "FFmpegCommandBuilder.h"
#include <string>
#include <algorithm>
#include <filesystem>

//XD 
#include "include\single_include\nlohmann\json.hpp"

///////////////

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

std::string GetExecutablePath() {
    char buffer[4096]; // Zdefiniuj w³asn¹ maksymaln¹ d³ugoœæ œcie¿ki

#if defined(_WIN32)
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
#elif defined(__linux__)
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
    }
    else {
        // Obs³uga b³êdu
        std::cerr << "Blad podczas odczytywania sciezki pliku wykonywalnego." << std::endl;
        return "";
    }
#endif

    return std::string(buffer);
}

//////////

void replaceSingleBackslashWithDouble(std::string& str) {
    size_t pos = 0;
    while ((pos = str.find("\\", pos)) != std::string::npos) {
        str.replace(pos, 1, "\\\\");
        pos += 2; // Przeskocz dwa znaki, aby unikn¹æ nieskoñczonej pêtli
    }
}

void processVideoFile(const std::string& filePath) {

    std::string modifiedPath = filePath;
    replaceSingleBackslashWithDouble(modifiedPath);

    std::cout << "Przetwarzanie pliku: " << modifiedPath << std::endl;

    // Przyk³adowe parametry dla FFmpegCommandBuilder
    std::string outputDirectory = "œcie¿ka/do/katalogu/wyjœciowego";
    int maxResolution = 1080; // Maksymalna rozdzielczoœæ
    int cqValue = 23; // Wartoœæ CQ dla NVENC
    std::string additionalFlags = ""; // Dodatkowe flagi, jeœli s¹ potrzebne
    std::vector<int> videoStreams = { 0 }; // Przyk³adowe strumienie wideo
    std::vector<int> audioStreams = { 0 }; // Przyk³adowe strumienie audio
    std::vector<int> subtitleStreams = { 0 }; // Przyk³adowe strumienie napisów
    EncoderType encoderType = EncoderType::GPU; // Wybór enkodera

    // Budowanie polecenia FFmpeg
    FFmpegCommandBuilder builder(filePath, outputDirectory, maxResolution, cqValue,
        additionalFlags, videoStreams, audioStreams, subtitleStreams, encoderType);
    std::string command = builder.buildCommand();

    std::cout << "Wygenerowane polecenie FFmpeg: " << command << std::endl;

    // Opcjonalnie: Uruchomienie polecenia FFmpeg
    // std::system(command.c_str());
}



void funkcja1() {
    std::cout << "Funkcja 1 wywolana\n";
    //std::cout << "Current path: " << std::filesystem::current_path() << std::endl;
   // FileProcessor processor("TEST", "TEST1");
   // processor.processFiles(processVideoFile);

   // std::string exePath = GetExecutablePath();
   // std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
    //std::string relativePath = exeDir.string() + "\\TEST1\\Nowy folder\\input4.mp4";
   // std::cout << "Relative path: " << relativePath << std::endl;

   // FFProbe probe(relativePath);
  // FFProbe probe(R"("C:\GIT_REPO\AUTOMATE_FFMPEG\AUTOMATE_FFMPEG\AUTOMATE_FFMPEG\AUTOMATE_FFMPEG\TEST\Nowy folder\input4.mp4")");


   FFProbe probe("\"\\TEST\\Nowy folder\\input4.mp4\"");  //CHUUUUUJJ
   probe.analyze();
   probe.displayStreams();
   
    
}

void funkcja2() {
    std::cout << "Funkcja 2 wywolana\n";
   // std::cout << Commands::execute("dir");

   /* FFProbe probe(R"("C:\GIT_REPO\AUTOMATE_FFMPEG\AUTOMATE_FFMPEG\AUTOMATE_FFMPEG\AUTOMATE_FFMPEG\TEST\Nowy folder\input4.mp4")");
    probe.analyze();
    probe.displayStreams();*/

    std::string inputFilePath = "test";
    std::string outputDirectory = "test1";
    int maxResolution = 1080;
    int cqValue = 23;
    std::string additionalFlags = "-additionalFlag1 value1 -additionalFlag2 value2";
    std::vector<int> videoStreams = { 0 }; // Wybrany strumieñ wideo
    std::vector<int> audioStreams = { 0, 1 }; // Wybrane strumienie audio
    std::vector<int> subtitleStreams = { 0, 1, 2 }; // Wybrane strumienie napisów

    EncoderType encoderType = EncoderType::CPU;
    FFmpegCommandBuilder builder(inputFilePath, outputDirectory, maxResolution, cqValue, additionalFlags, videoStreams, audioStreams, subtitleStreams, encoderType);
    std::string command = builder.buildCommand();
    std::cout << "Wygenerowane polecenie FFmpeg: " << command << std::endl;

}



int main()
{




    auto podMenu = std::make_shared<Menu>();
    podMenu->addItem("Podopcja 1", funkcja1);
    podMenu->addItem("Podopcja 2", funkcja2);

    Menu menu;
    menu.addItem("Opcja 1", funkcja1);
    menu.addSubMenu("Podmenu", podMenu);

    menu.run();




	return 0;
}

