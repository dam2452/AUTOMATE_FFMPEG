#ifndef FFMPEGCOMMANDBUILDER_H
#define FFMPEGCOMMANDBUILDER_H

#include <string>
#include "FFProbe.h"

class FFmpegCommandBuilder {
public:
    FFmpegCommandBuilder(const std::string& inputFilePath, const std::string& outputDirectory, int maxResolution, int cqValue, const std::string& additionalFlags);
    std::string buildCommand();

private:
    std::string inputFilePath;
    std::string outputDirectory;
    int maxResolution;
    int cqValue;
    std::string additionalFlags;
    FFProbe ffprobe;
    bool checkAudioCodec(const nlohmann::json& stream) const;
    std::string createOutputFileName() const;
    std::string generateVideoFilter() const;
};

#endif // FFMPEGCOMMANDBUILDER_H
