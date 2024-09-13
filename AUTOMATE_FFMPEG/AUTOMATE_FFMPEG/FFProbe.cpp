#include "FFProbe.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

using json = nlohmann::json;

FFProbe::FFProbe(const std::string& filePath) : filePath(filePath) {}

bool FFProbe::analyze() {
    std::string command = "ffprobe -v quiet -print_format json -show_streams " + std::string("\"") + filePath + std::string("\"") + " > ffprobe_output.json";
    int commandResult = std::system(command.c_str());

    if (commandResult != 0) {
        std::cerr << "ffprobe command failed with exit code " << commandResult << std::endl;
        return false;
    }

    std::ifstream file("ffprobe_output.json");
    if (file.is_open()) {
        file >> jsonData;
        file.close();
        return true;
    }
    else {
        std::cerr << "Failed to open ffprobe output file" << std::endl;
        return false;
    }
}

void FFProbe::displayStreams() {
    if (jsonData.contains("streams")) {
        for (const auto& stream : jsonData["streams"]) {
            std::cout << "Stream index: " << stream["index"] << std::endl;
            std::cout << "Codec type: " << stream["codec_type"] << std::endl;
            std::cout << "Codec name: " << stream["codec_name"] << std::endl;

            if (stream["codec_type"] == "video") {
                std::cout << "Video resolution: " << stream["width"] << "x" << stream["height"] << std::endl;
                std::cout << "Frame rate: " << stream["r_frame_rate"] << std::endl;
            }
            else if (stream["codec_type"] == "audio") {
                std::cout << "Audio channels: " << stream["channels"] << std::endl;
                std::cout << "Sample rate: " << stream["sample_rate"] << std::endl;
            }
            else if (stream["codec_type"] == "subtitle") {
                std::cout << "Subtitle format: " << stream["codec_name"] << std::endl;
            }

            std::cout << std::endl;
        }
    }
}

nlohmann::json FFProbe::getStreams() const {
    if (jsonData.contains("streams")) {
        return jsonData["streams"];
    }
    else {
        return nlohmann::json::array();
    }
}
