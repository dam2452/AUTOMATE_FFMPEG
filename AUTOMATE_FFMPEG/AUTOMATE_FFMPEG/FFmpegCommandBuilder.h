#ifndef FFMPEGCOMMANDBUILDER_H
#define FFMPEGCOMMANDBUILDER_H

#include <string>
#include <vector>
#include "FFProbe.h"

enum class EncoderType {
    CPU,
    GPU
};

class FFmpegCommandBuilder {
public:
    FFmpegCommandBuilder(const std::string& inputFilePath, const std::string& outputDirectory,
        int maxResolution, int cqValue, const std::string& additionalFlags,
        const std::vector<int>& videoStreams,
        const std::vector<int>& audioStreams,
        const std::vector<int>& subtitleStreams,
        EncoderType encoderType, std::string extension);

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
    EncoderType encoderType;
    std::string extension;

    std::string createOutputFileName() const;

    bool streamExists(const std::vector<nlohmann::json>& streams, const std::string& type, int index);

    std::string generateVideoFilter() const;
    std::string generateStreamSelectors(
        const std::vector<int>& videoStreams,
        const std::vector<int>& audioStreams,
        const std::vector<int>& subtitleStreams,
        const std::vector<nlohmann::json>& streams);
    std::string generateEncoderOptions() const;
    std::string generateSelectorForStreamType(
        const std::vector<int>& streams,
        const std::string& streamType,
        const std::vector<nlohmann::json>& allStreams);

    std::string generateSelectorForAllStreamsOfType(const std::string& streamType, 
        const std::vector<nlohmann::json>& allStreams);

    bool isCompatibleAudio(const nlohmann::json& stream) const;
    bool isCompatibleSubtitle(const nlohmann::json& stream);
    bool isCompatibleVideo(const nlohmann::json& stream) const;
    bool isStandardVideoStream(const nlohmann::json& stream) const;

    std::string processAudioStreams(const std::vector<nlohmann::json>& streams);
    std::string processVideoStreams(const std::vector<nlohmann::json>& streams);
    std::string processSubtitleStreams(const std::vector<nlohmann::json>& streams);


};

#endif // FFMPEGCOMMANDBUILDER_H
