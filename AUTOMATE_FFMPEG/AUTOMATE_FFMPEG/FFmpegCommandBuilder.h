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


    bool checkAudioCodec(const nlohmann::json& stream) const;
    bool isPGSSubtitle(const nlohmann::json& stream) const;
    std::string createOutputFileName() const;
    bool isStreamSelected(int streamIndex);
    bool isCompatibleSubtitle(const nlohmann::json& stream);
    void appendSubtitleProcessing(std::ostringstream& cmd, const std::vector<nlohmann::json>& streams);
    bool isCompatibleSubtitle(const nlohmann::json& stream) const;
    std::string generateVideoFilter() const;
    std::string generateStreamSelectors(const std::vector<int>& videoStreams, const std::vector<int>& audioStreams);
    std::string generateEncoderOptions() const;
    bool isCoverArt(const nlohmann::json& stream) const;
    bool isSubripSubtitle(const nlohmann::json& stream) const;
};

#endif // FFMPEGCOMMANDBUILDER_H
