#include "FFmpegCommandBuilder.h"
#include "FFProbe.h"
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

FFmpegCommandBuilder::FFmpegCommandBuilder(const std::string& inputFilePath,
    const std::string& outputDirectory,
    int maxResolution, int cqValue,
    const std::string& additionalFlags,
    const std::vector<int>& videoStreams,
    const std::vector<int>& audioStreams,
    const std::vector<int>& subtitleStreams)
    : inputFilePath(inputFilePath), outputDirectory(outputDirectory),
    maxResolution(maxResolution), cqValue(cqValue), additionalFlags(additionalFlags),
    ffprobe(inputFilePath), videoStreams(videoStreams), audioStreams(audioStreams),
    subtitleStreams(subtitleStreams) {
    // Dodatkowa logika inicjalizacyjna, jeœli jest potrzebna
}

bool FFmpegCommandBuilder::checkAudioCodec(const nlohmann::json& stream) const {
    const std::string codec = stream["codec_name"];
    return (codec == "aac" || codec == "eac3");
}

std::string FFmpegCommandBuilder::buildCommand() {
    ffprobe.analyze();
    auto streams = ffprobe.getStreams();

    std::ostringstream cmd;
    cmd << "ffmpeg -i \"" << inputFilePath << "\"";

    // Stream selectors
    cmd << generateStreamSelectors();

    // Audio processing
    bool audioNeedConversion = false;
    for (const auto& stream : streams) {
        if (stream["codec_type"] == "audio" &&
            !(stream["codec_name"] == "aac" || stream["codec_name"] == "eac3")) {
            audioNeedConversion = true;
            break;
        }
    }

    if (audioNeedConversion) {
        cmd << " -c:a aac";
    }
    else {
        cmd << " -c:a copy";
    }

    // Video processing
    cmd << " -c:v hevc_nvenc";
    std::string vf = generateVideoFilter();
    if (!vf.empty()) {
        cmd << " -vf \"" << vf << "\"";
    }

    // Subtitles processing
    cmd << " -c:s copy";

    // Additional flags
    if (!additionalFlags.empty()) {
        cmd << " " << additionalFlags;
    }

    // Output file
    std::string outputFilePath = createOutputFileName();
    cmd << " \"" << outputFilePath << "\"";

    return cmd.str();
}

std::string FFmpegCommandBuilder::createOutputFileName() const {
    fs::path inputPath(inputFilePath);
    std::string baseName = inputPath.stem().string() + "_converted_CQ" + std::to_string(cqValue);
    return (fs::path(outputDirectory) / baseName).replace_extension(".mp4").string();
}

std::string FFmpegCommandBuilder::generateVideoFilter() const {
    if (maxResolution > 0) {
        return "scale=-1:min(" + std::to_string(maxResolution) + "\\,ih)";
    }
    return "";
}

std::string FFmpegCommandBuilder::generateStreamSelectors() {
    std::ostringstream selectors;

    // Video stream selectors
    if (!videoStreams.empty()) {
        for (int stream : videoStreams) {
            selectors << " -map 0:v:" << stream;
        }
    }

    // Audio stream selectors
    if (!audioStreams.empty()) {
        for (int stream : audioStreams) {
            selectors << " -map 0:a:" << stream;
        }
    }

    // Subtitle stream selectors
    if (!subtitleStreams.empty()) {
        for (int stream : subtitleStreams) {
            selectors << " -map 0:s:" << stream;
        }
    }

    return selectors.str();
}