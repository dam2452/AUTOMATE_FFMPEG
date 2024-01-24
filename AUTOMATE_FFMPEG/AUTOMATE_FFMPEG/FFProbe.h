#ifndef FFPROBE_H
#define FFPROBE_H

#include <string>

//Dont even ask XD 
#include "include\single_include\nlohmann\json.hpp"

class FFProbe {
public:
    FFProbe(const std::string& filePath);
    bool analyze();
    void displayStreams();
    nlohmann::json getStreams() const;

private:
    std::string filePath;
    nlohmann::json jsonData;
};

#endif // FFPROBE_H
