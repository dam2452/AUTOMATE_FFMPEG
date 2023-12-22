#include "FFmpegCommandBuilder.h"
#include "FFProbe.h"
#include <sstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

FFmpegCommandBuilder::FFmpegCommandBuilder(const std::string& inputFilePath,
    const std::string& outputDirectory,
    int maxResolution, int cqValue,
    const std::string& additionalFlags,
    const std::vector<int>& videoStreams,
    const std::vector<int>& audioStreams,
    const std::vector<int>& subtitleStreams,
    EncoderType encoderType, std::string extension)
    : inputFilePath(inputFilePath), outputDirectory(outputDirectory),
    maxResolution(maxResolution), cqValue(cqValue), additionalFlags(additionalFlags),
    ffprobe(inputFilePath), videoStreams(videoStreams), audioStreams(audioStreams),
    subtitleStreams(subtitleStreams), encoderType(encoderType), extension(extension){
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
    cmd << generateEncoderOptions();
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

    std::string outputDirectoryCopy = outputDirectory;
    if (outputDirectoryCopy.size() >= 4) {
        outputDirectoryCopy.erase(outputDirectoryCopy.size() - 4);  // Usuwa ostatnie trzy znaki
    }

    return outputDirectoryCopy + "_CQ" + std::to_string(cqValue) + extension;
}



std::string FFmpegCommandBuilder::generateVideoFilter() const {
    if (maxResolution > 0) {
        return "scale=-1:min(" + std::to_string(maxResolution) + "\\,ih)";
    }
    return "";
}

std::string FFmpegCommandBuilder::generateStreamSelectors() {
    std::ostringstream selectors;
    std::string encoderOptions = generateEncoderOptions();

    // Video stream selectors
    if (videoStreams.empty()) {
        selectors << " -map 0:v";
    }
    else {
        for (int stream : videoStreams) {
            selectors << " -map 0:v:" << stream;
        }
    }

    // Audio stream selectors
    if (audioStreams.empty()) {
        selectors << " -map 0:a";
    }
    else {
        for (int stream : audioStreams) {
            selectors << " -map 0:a:" << stream;
        }
    }

    // Subtitle stream selectors
    if (subtitleStreams.empty()) {
        selectors << " -map 0:s";
    }
    else {
        for (int stream : subtitleStreams) {
            selectors << " -map 0:s:" << stream;
        }
    }

    return selectors.str();
}

std::string FFmpegCommandBuilder::generateEncoderOptions() const {
    std::ostringstream encoderOptions;
    if (encoderType == EncoderType::GPU) {
        encoderOptions << " -c:v hevc_nvenc";
    }
    else if (encoderType == EncoderType::CPU) {
        encoderOptions << " -c:v libx265 -cpu-used 4 -threads 16 -tile-columns 2 -tile-rows 1 -row-mt 1 -speed 2 -auto-alt-ref 1 -lag-in-frames 25";
        // Mo¿esz dodaæ opcje wielow¹tkowoœci, np. threads=16, jeœli chcesz ograniczyæ liczbê w¹tków
    }
    return encoderOptions.str();
}


//TO DO
bool FFmpegCommandBuilder::isCoverArt(const nlohmann::json& stream) const {
    // SprawdŸ, czy strumieñ ma tylko jedn¹ klatkê (typowe dla ok³adek)
    if (stream.contains("nb_frames") && stream["nb_frames"] == "1") {
        return true;
    }

    // Dodatkowe warunki, np. sprawdzenie rozmiaru, metadanych itp.

    return false;
}
