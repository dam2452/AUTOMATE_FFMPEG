#include "FFmpegCommandBuilder.h"
#include "FFProbe.h"
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

FFmpegCommandBuilder::FFmpegCommandBuilder(const std::string& inputFilePath, const std::string& outputDirectory, int maxResolution, int cqValue, const std::string& additionalFlags)
    : inputFilePath(inputFilePath), outputDirectory(outputDirectory), maxResolution(maxResolution), cqValue(cqValue), additionalFlags(additionalFlags), ffprobe(inputFilePath) {
    // Tutaj mo¿na dodaæ dodatkow¹ logikê inicjalizacyjn¹, jeœli jest potrzebna
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

    // Audio processing
    bool audioProcessed = false;
    for (const auto& stream : streams) {
        if (stream["codec_type"] == "audio") {
            if (!checkAudioCodec(stream)) {
                cmd << " -c:a aac";
                audioProcessed = true;
                break;
            }
        }
    }
    if (!audioProcessed) {
        cmd << " -c:a copy";
    }

    // Video processing
    std::string vf = generateVideoFilter();
    if (!vf.empty()) {
        cmd << " -vf \"" << vf << "\"";
    }
    cmd << " -c:v hevc_nvenc -cq:v " << cqValue;

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