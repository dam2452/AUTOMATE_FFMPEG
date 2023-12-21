#ifndef FFMPEGCOMMANDBUILDER_H
#define FFMPEGCOMMANDBUILDER_H

#include <string>
#include <vector>
#include "FFProbe.h"

class FFmpegCommandBuilder {
public:
    FFmpegCommandBuilder(const std::string& inputFilePath, const std::string& outputDirectory,
        int maxResolution, int cqValue, const std::string& additionalFlags,
        const std::vector<int>& videoStreams,
        const std::vector<int>& audioStreams,
        const std::vector<int>& subtitleStreams);

    std::string buildCommand();

private:
    std::string inputFilePath;
    std::string outputDirectory;
    int maxResolution;
    int cqValue;
    std::string additionalFlags;
    FFProbe ffprobe;
    std::vector<int> videoStreams;
    std::vector<int> audioStreams;
    std::vector<int> subtitleStreams;

    bool checkAudioCodec(const nlohmann::json& stream) const;
    std::string createOutputFileName() const;
    std::string generateVideoFilter() const;
    std::string generateStreamSelectors();
};

#endif // FFMPEGCOMMANDBUILDER_H
