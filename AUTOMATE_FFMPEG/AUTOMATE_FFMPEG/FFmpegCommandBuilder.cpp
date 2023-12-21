#include "FFmpegCommandBuilder.h"
#include "FFProbe.h"
#include <sstream>

FFmpegCommandBuilder::FFmpegCommandBuilder(const std::string& inputFilePath)
    : inputFilePath(inputFilePath), ffprobe(inputFilePath) {}

bool FFmpegCommandBuilder::checkAudioCodec(const nlohmann::json& stream) const {
    const std::string codec = stream["codec_name"];
    return (codec == "aac" || codec == "eac3");
}

std::string FFmpegCommandBuilder::buildCommand() {
    ffprobe.analyze();

    std::ostringstream cmd;
    cmd << "ffmpeg -i \"" << inputFilePath << "\"";

    bool audioCodecChanged = false;
    for (const auto& stream : ffprobe.getStreams()) {
        if (stream["codec_type"] == "audio" && !checkAudioCodec(stream)) {
            audioCodecChanged = true;
            break;
        }
    }

    if (audioCodecChanged) {
        cmd << " -c:a aac";
    }
    else {
        cmd << " -c:a copy";
    }

    cmd << " -c:v hevc_nvenc";
    cmd << " -c:s copy"; // Kopiowanie napisów 1:1

    std::string outputFilePath = inputFilePath;
    outputFilePath.replace(outputFilePath.length() - 4, 4, "_converted.mkv");
    cmd << " \"" << outputFilePath << "\"";

    return cmd.str();
}
