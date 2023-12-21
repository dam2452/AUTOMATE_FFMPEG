#include <iostream>
#include <string>
#include <vector>
#include "commands.h"
#include "menu.h"
#include "FileProcessor.h"
#include "FFProbe.h"
#include "FFmpegCommandBuilder.h"

//XD 
#include "include\single_include\nlohmann\json.hpp"

void processVideoFile(const std::string& filePath) {
  //  FFProbe probe(filePath);
  //  probe.analyze();


    // Tutaj umieœæ logikê dotycz¹c¹ FFmpeg
   // std::cout << "Przetwarzanie pliku: " << filePath <<std::endl;
  //  probe.displayStreams();

    std::cout << "Przetwarzanie pliku: " << filePath << std::endl;

    std::string inputFilePath = "œcie¿ka/do/pliku.mkv";
    std::string outputDirectory = "œcie¿ka/do/katalogu/wyjœciowego";
    int maxResolution = 1080;
    int cqValue = 23;
    std::string additionalFlags = "-additionalFlag1 value1 -additionalFlag2 value2";
    std::vector<int> videoStreams = { 0 }; // Wybrany strumieñ wideo
    std::vector<int> audioStreams = { 0, 1 }; // Wybrane strumienie audio
    std::vector<int> subtitleStreams = { 0, 1, 2 }; // Wybrane strumienie napisów

    FFmpegCommandBuilder builder(inputFilePath, outputDirectory, maxResolution, cqValue, additionalFlags, videoStreams, audioStreams, subtitleStreams);
    std::string command = builder.buildCommand();
    std::cout << "Wygenerowane polecenie FFmpeg: " << command << std::endl;



}


void funkcja1() {
    std::cout << "Funkcja 1 wywolana\n";

    FileProcessor processor("TEST", "TEST1");
    processor.processFiles(processVideoFile);
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

    FFmpegCommandBuilder builder(inputFilePath, outputDirectory, maxResolution, cqValue, additionalFlags, videoStreams, audioStreams, subtitleStreams);
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

