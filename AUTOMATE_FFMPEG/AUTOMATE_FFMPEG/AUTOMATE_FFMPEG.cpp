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
std::string additionalFlags = "";  //DEBUG: -loglevel quiet   -b:v 1M -maxrate 1.2M -bufsize 2M
std::string sourcePath="XYZ";
std::string destinationPath="XYZOUT";

void processVideoFile(const std::string& filePath, const std::string& outputDir) {

    FFmpegCommandBuilder builder(filePath, outputDir, maxResolution, cqValue, additionalFlags, selectedVideoStreams, selectedAudioStreams, selectedSubtitleStreams, encoderType, ".mp4");
    std::string command = builder.buildCommand();

    std::cout << "Generated FFmpeg command: " << command << std::endl;

    // Execute FFmpeg command
    Commands::execute(command, true);
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
            std::cout << "Invalid index: " << item << std::endl;
        }
    }
    return indices;
}



void choosePaths() {

    std::cout << "Enter source path: ";
    std::getline(std::cin, sourcePath);

    // Checking if the source path is empty
    if (sourcePath.empty()) {
        std::cout << "No source path provided! Press Enter to continue..." << std::endl;
        return;
    }

    std::cout << "Enter destination path: ";
    std::getline(std::cin, destinationPath);

    // Checking if the destination path is empty
    if (destinationPath.empty()) {
        std::cout << "No destination path provided! Press Enter to continue..." << std::endl;
        return;
    }

    std::cout << "Selected source path: " << sourcePath << std::endl;
    std::cout << "Selected destination path: " << destinationPath << std::endl;
}


void startProcessing() {
    std::cout << "Starting file processing...\n";
    FileProcessor processor(sourcePath, destinationPath);
    processor.processFiles(processVideoFile);
    std::cout << "Processing completed.\n";
}

void selectVideoStreams() {
    std::cout << "Select video streams (default: ALL) (indices separated by commas): ";
    std::string input;
    std::getline(std::cin, input);

    // If input is empty, select all streams
    if (input.empty()) {
        std::cout << "Selected video streams: ALL" << std::endl;
        return;
    }

    std::istringstream iss(input);
    std::string token;
    std::vector<int> validIndices;
    bool invalidInput = false;

    // Parsing the indices
    while (std::getline(iss, token, ',')) {
        try {
            int index = std::stoi(token);
            validIndices.push_back(index);
        }
        catch (...) {
            // In case of invalid input (non-integer values)
            std::cerr << "Invalid input: " << token << ". Please enter valid indices." << std::endl;
            invalidInput = true;
            break;
        }
    }

    if (!invalidInput) {
        selectedVideoStreams = validIndices;
        std::cout << "Selected video streams: ";
        for (int index : selectedVideoStreams) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
    }
}

void selectAudioStreams() {
    std::cout << "Select audio streams (default: ALL) (indices separated by commas): ";
    std::string input;
    std::getline(std::cin, input);

    // If input is empty, select all streams
    if (input.empty()) {
        std::cout << "Selected audio streams: ALL" << std::endl;
        return;
    }

    std::istringstream iss(input);
    std::string token;
    std::vector<int> validIndices;
    bool invalidInput = false;

    // Parsing the indices
    while (std::getline(iss, token, ',')) {
        try {
            int index = std::stoi(token);
            validIndices.push_back(index);
        }
        catch (...) {
            // In case of invalid input (non-integer values)
            std::cerr << "Invalid input: " << token << ". Please enter valid indices." << std::endl;
            invalidInput = true;
            break;
        }
    }

    if (!invalidInput) {
        selectedAudioStreams = validIndices;
        std::cout << "Selected audio streams: ";
        for (int index : selectedAudioStreams) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
    }
}

void selectSubtitleStreams() {
    std::cout << "Select subtitle streams (default: ALL) (indices separated by commas): ";
    std::string input;
    std::getline(std::cin, input);

    // If input is empty, select all streams
    if (input.empty()) {
        std::cout << "Selected subtitle streams: ALL" << std::endl;
        return;
    }

    std::istringstream iss(input);
    std::string token;
    std::vector<int> validIndices;
    bool invalidInput = false;

    // Parsing the indices
    while (std::getline(iss, token, ',')) {
        try {
            int index = std::stoi(token);
            validIndices.push_back(index);
        }
        catch (...) {
            // In case of invalid input (non-integer values)
            std::cerr << "Invalid input: " << token << ". Please enter valid indices." << std::endl;
            invalidInput = true;
            break;
        }
    }

    if (!invalidInput) {
        selectedSubtitleStreams = validIndices;
        std::cout << "Selected subtitle streams: ";
        for (int index : selectedSubtitleStreams) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
    }
}

void selectCQ() {
    std::cout << "Select CQ (default: 28): ";

    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }

    std::string input;
    std::getline(std::cin, input);

    try {
        if (!input.empty()) {
            cqValue = std::stoi(input);
        }
        else {
            cqValue = 28; // Default value
        }
        std::cout << "Selected CQ: " << cqValue << std::endl;
    }
    catch (const std::invalid_argument& e) {
        // If the string cannot be converted to an integer
        std::cerr << "Invalid input. Please enter a valid number." << std::endl;
        cqValue = 28; // Default value
    }
    catch (const std::out_of_range& e) {
        // If the number is too large
        std::cerr << "Number is out of range. Please enter a smaller number." << std::endl;
        cqValue = 28; // Default value
    }
}

void selectResLimit() {
    std::cout << "Select resolution limit (default: 1080): ";

    std::string input;
    std::getline(std::cin, input);

    try {
        maxResolution = std::stoi(input);
        std::cout << "Selected resolution limit: " << maxResolution << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid input. Please enter a number." << std::endl;
        maxResolution = 1080;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Number is out of range. Please enter a smaller number." << std::endl;
        maxResolution = 1080;
    }
}


void chooseEncoderType() {
    std::cout << "Choose encoder type (default: GPU) : ";

    std::string input;

    std::getline(std::cin, input);

    if (input == "CPU") {
        encoderType = EncoderType::CPU;
    }
    else if (input == "GPU") {
        encoderType = EncoderType::GPU;
    }
    else {
        std::cout << "Invalid encoder type! Press Enter to continue..." << std::endl;
        return;
    }
    std::cout << "Selected encoder type: " << input;
    std::cout << std::endl;
}
//show settings
void showSettings() {
    std::cout << std::endl;
	std::cout << "Selected source path: " << sourcePath << std::endl;
	std::cout << "Selected destination path: " << destinationPath << std::endl;
	std::cout << "Selected video streams (Nothing = ALL): ";
    for (int index : selectedVideoStreams) {
		std::cout << index << " ";
	}
	std::cout << std::endl;
	std::cout << "Selected audio streams (Nothing = ALL): ";
    for (int index : selectedAudioStreams) {
		std::cout << index << " ";
	}
	std::cout << std::endl;
	std::cout << "Selected subtitle streams (Nothing = ALL): ";
    for (int index : selectedSubtitleStreams) {
		std::cout << index << " ";
	}
	std::cout << std::endl;
	std::cout << "Selected CQ: " << cqValue << std::endl;
	std::cout << "Selected resolution limit: " << maxResolution << std::endl;
	std::cout << "Selected encoder type (0 = CPU , 1 = GPU): " << static_cast<int>(encoderType) << std::endl;
	std::cout << "Selected additional flags: " << additionalFlags << std::endl;
    std::cout << std::endl;
}


int main()
{
    Menu menu("AUTOMATE_FFMPEG");

    menu.addItem("Start processing", startProcessing);
    menu.addItem("Choose paths", choosePaths);
    menu.addItem("Select audio streams", selectAudioStreams);
    menu.addItem("Select subtitle streams", selectSubtitleStreams);
    menu.addItem("Select video streams", selectVideoStreams);
    menu.addItem("Select CQ", selectCQ);
    menu.addItem("Select resolution limit", selectResLimit);
    menu.addItem("Choose encoder type", chooseEncoderType);
    menu.addItem("Check settings", showSettings);


    menu.run();

    return 0;
}
