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
#include <windows.h>

//XD 
#include "include\single_include\nlohmann\json.hpp"

void processVideoFile(const std::string& filePath, const std::string& outputDir) {
    std::cout << "filePath: " << filePath << std::endl;

    int maxResolution = 1080; // Maksymalna rozdzielczoœæ
    int cqValue = 23; // Wartoœæ CQ 
    std::string additionalFlags = ""; // Dodatkowe flagi
    std::vector<int> videoStreams = { 0 }; // Strumienie wideo
    std::vector<int> audioStreams = { 0 }; // Strumienie audio
    std::vector<int> subtitleStreams = { 0 }; // Strumienie napisów
    EncoderType encoderType = EncoderType::GPU; // Wybór enkodera

    FFmpegCommandBuilder builder(filePath, outputDir, maxResolution, cqValue, additionalFlags, videoStreams, audioStreams, subtitleStreams, encoderType,".mp4");
    std::string command = builder.buildCommand();

    std::cout << "outputDir: " << outputDir << std::endl;
    std::cout << "Wygenerowane polecenie FFmpeg: " << command << std::endl;

    // Wykonanie polecenia FFmpeg
    // Commands::execute(command);
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
    funkcja2();


    //auto podMenu = std::make_shared<Menu>();
    //podMenu->addItem("Podopcja 1", funkcja1);
    //podMenu->addItem("Podopcja 2", funkcja2);

    //Menu menu;
    //menu.addItem("Opcja 1", funkcja1);
    //menu.addSubMenu("Podmenu", podMenu);

    //menu.run();




	return 0;
}

