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
#define NOMINMAX
#include <windows.h>
#include <memory>
#include <limits>

//XD 
#include "include\single_include\nlohmann\json.hpp"

int maxResolution = 1080;
int cqValue = 28;
EncoderType encoderType = EncoderType::GPU;
std::vector<int> selectedVideoStreams;
std::vector<int> selectedAudioStreams;
std::vector<int> selectedSubtitleStreams;
std::string additionalFlags = "-loglevel quiet";  //DEBUG: -loglevel debug 
std::string sourcePath="XYZ";
std::string destinationPath="XYZOUT";

void processVideoFile(const std::string& filePath, const std::string& outputDir) {

    FFmpegCommandBuilder builder(filePath, outputDir, maxResolution, cqValue, additionalFlags, selectedVideoStreams, selectedAudioStreams, selectedSubtitleStreams, encoderType,".mp4");
    std::string command = builder.buildCommand();

     //std::cout << "Wygenerowane polecenie FFmpeg: " << command << std::endl;

     // Wykonanie polecenia FFmpeg
     Commands::execute(command,true);

}



std::vector<int> parseStreamIndices(const std::string& input) {
    std::vector<int> indices;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        try {
            indices.push_back(std::stoi(item));
        }
        catch (...) {
            std::cout << "Nieprawidlowy indeks: " << item << std::endl;
        }
    }
    return indices;
}



void choosePaths() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignoruje wszystkie pozosta³oœci w buforze

    std::cout << "Podaj sciezke zrodlowa: ";
    std::getline(std::cin, sourcePath);

    // Sprawdzanie, czy œcie¿ka Ÿród³owa nie jest pusta
    if (sourcePath.empty()) {
        std::cout << "Nie podano sciezki zrodlowej! Nacisnij Enter, aby kontynuowac..." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorowanie bufora
        return;
    }

    std::cout << "Podaj sciezke docelowa: ";
    std::getline(std::cin, destinationPath);

    // Sprawdzanie, czy œcie¿ka docelowa nie jest pusta
    if (destinationPath.empty()) {
        std::cout << "Nie podano sciezki docelowej! Nacisnij Enter, aby kontynuowac..." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorowanie bufora
        return;
    }

    std::cout << "Wybrano sciezke zrodlowa: " << sourcePath << std::endl;
    std::cout << "Wybrano sciezke docelowa: " << destinationPath << std::endl;
    std::cout << "Nacisnij Enter, aby kontynuowac..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorowanie bufora
}

void startProcessing() {
    std::cout << "Rozpoczynam przetwarzanie plikow...\n";
    FileProcessor processor(sourcePath, destinationPath);
    processor.processFiles(processVideoFile);
    std::cout << "Przetwarzanie zakonczone.\n";
}

void selectVideoStreams() {
    std::cout << "Wybierz strumienie wideo (indeksy oddzielone przecinkami): ";
    // Usuniêcie pozosta³oœci w buforze wejœciowym
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string input;
    std::getline(std::cin, input);

    selectedVideoStreams = parseStreamIndices(input);
    std::cout << "Wybrane strumienie wideo: ";
    for (int index : selectedVideoStreams) {
        std::cout << index << " ";
    }
    std::cout << std::endl;
}


void selectAudioStreams() {
    std::cout << "Wybierz strumienie audio (indeksy oddzielone przecinkami): ";

    // Usuniêcie pozosta³oœci w buforze wejœciowym
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string input;

    std::getline(std::cin, input);

    selectedAudioStreams = parseStreamIndices(input);
    std::cout << "Wybrane strumienie audio: ";
    for (int index : selectedAudioStreams) {
        std::cout << index << " ";
    }
    std::cout << std::endl;
}


void selectSubtitleStreams() {
    std::cout << "Wybierz strumienie napisow (indeksy oddzielone przecinkami): ";
    // Usuniêcie pozosta³oœci w buforze wejœciowym
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string input;
    std::getline(std::cin, input);

    selectedSubtitleStreams = parseStreamIndices(input);
    std::cout << "Wybrane strumienie napisów: ";
    for (int index : selectedSubtitleStreams) {
        std::cout << index << " ";
    }
    std::cout << std::endl;
}

void selectCQ() {
    std::cout << "Wybierz CQ: ";

    // Usuniêcie pozosta³oœci w buforze wejœciowym
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string input;

    std::getline(std::cin, input);

    cqValue = std::stoi(input);
    std::cout << "Wybrane CQ: "<< cqValue;
    std::cout << std::endl;
}

void selectResLimit() {
    std::cout << "Wybierz limit rozdzielczosci: ";

    // Usuniêcie pozosta³oœci w buforze wejœciowym
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string input;

    std::getline(std::cin, input);

    maxResolution = std::stoi(input);
    std::cout << "Wybrany limit rozdzielczosci: " << maxResolution;
    std::cout << std::endl;
}

void chooseEncoderType() {
	std::cout << "Wybierz typ enkodera: ";

	// Usuniêcie pozosta³oœci w buforze wejœciowym
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::string input;

	std::getline(std::cin, input);

    if (input == "CPU") {
		encoderType = EncoderType::CPU;
	}
    else if (input == "GPU") {
		encoderType = EncoderType::GPU;
	}
    else {
		std::cout << "Nieprawidlowy typ enkodera! Nacisnij Enter, aby kontynuowac..." << std::endl;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorowanie bufora
		return;
	}
	std::cout << "Wybrany typ enkodera: " << input;
	std::cout << std::endl;
}



int main()
{
    Menu menu;

    menu.addItem("Rozpocznij przetwarzanie", startProcessing);
    menu.addItem("Wybierz sciezki", choosePaths);
    menu.addItem("Wybierz strumienie audio", selectAudioStreams);
    menu.addItem("Wybierz strumienie napisow", selectSubtitleStreams);
    menu.addItem("Wybierz strumienie wideo", selectVideoStreams);
    menu.addItem("Wybierz CQ", selectCQ);
    menu.addItem("Wybierz limit rozdzielczosci", selectResLimit);
    menu.addItem("Wybierz typ enkodera", chooseEncoderType);

    menu.run();

    return 0;
}

