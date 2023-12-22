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

void processVideoFile(const std::string& filePath, const std::string& outputDir) {

    std::cout << "Przetwarzanie pliku: " << filePath << std::endl;
    

    // Przyk³adowe parametry dla FFmpegCommandBuilder
    int maxResolution = 1080; // Maksymalna rozdzielczoœæ
    int cqValue = 23; // Wartoœæ CQ 
    std::string additionalFlags = ""; // Dodatkowe flagi, jeœli s¹ potrzebne
    std::vector<int> videoStreams = { 0 }; // Przyk³adowe strumienie wideo
    std::vector<int> audioStreams = { 0 }; // Przyk³adowe strumienie audio
    std::vector<int> subtitleStreams = { 0 }; // Przyk³adowe strumienie napisów
    EncoderType encoderType = EncoderType::GPU; // Wybór enkodera

    //// Budowanie polecenia FFmpeg
    FFmpegCommandBuilder builder(filePath, outputDir, maxResolution, cqValue,
        additionalFlags, videoStreams, audioStreams, subtitleStreams, encoderType);
    std::string command = builder.buildCommand();

    // Wykonanie polecenia FFmpeg
    Commands::execute(command);

   // std::cout << "Wygenerowane polecenie FFmpeg: " << command << std::endl;

}



void funkcja1() {
    std::cout << "Funkcja 1 wywolana\n";




    
}

void funkcja2() {
    std::cout << "Funkcja 2 wywolana\n";

    FileProcessor processor("TEST", "TEST1");
    processor.processFiles(processVideoFile);


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

