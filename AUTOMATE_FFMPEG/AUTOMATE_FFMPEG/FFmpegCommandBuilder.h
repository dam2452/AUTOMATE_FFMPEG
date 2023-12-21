#ifndef FFMPEGCOMMANDBUILDER_H
#define FFMPEGCOMMANDBUILDER_H

#include <string>
#include "FFProbe.h"

class FFmpegCommandBuilder {
public:
    FFmpegCommandBuilder(const std::string& inputFilePath);
    std::string buildCommand();

private:
    std::string inputFilePath;
    FFProbe ffprobe;
    bool checkAudioCodec(const nlohmann::json& stream) const;
};

#endif // FFMPEGCOMMANDBUILDER_H
