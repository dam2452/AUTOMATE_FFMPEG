#include "FFmpegCommandBuilder.h"
#include "FFProbe.h"
#include <sstream>
#include <filesystem>
#include <string>
#include <set>
#include <iostream>

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
}




bool FFmpegCommandBuilder::isCompatibleAudio(const nlohmann::json& stream) const {
    const std::string codec = stream.value("codec_name", "");
    return (codec == "aac" || codec == "eac3");
}

bool FFmpegCommandBuilder::isCompatibleSubtitle(const nlohmann::json& stream) {
    std::string codecName = stream.value("codec_name", "");
    return (codecName == "mov_text" || codecName == "different_compatible_format");
}

bool FFmpegCommandBuilder::isCompatibleVideo(const nlohmann::json& stream) const {
    if (stream.value("codec_type", "") != "video") {
        return false;
    }

    if (stream.contains("nb_frames") && stream["nb_frames"].get<std::string>() == "1") {
        return false;
    }

    if (stream.contains("disposition") && stream["disposition"].value("attached_pic", 0) == 1) {
        return false;
    }


    return true;


std::string FFmpegCommandBuilder::processSubtitleStreams(const std::vector<nlohmann::json>& streams) {
    std::ostringstream subtitleCmd;
    for (const auto& stream : streams) {
        if (stream["codec_type"] == "subtitle") {
            int streamIndex = stream["index"];
            if (std::find(subtitleStreams.begin(), subtitleStreams.end(), streamIndex) != subtitleStreams.end()) {
                std::string codecOption = isCompatibleSubtitle(stream) ? "copy" : "mov_text";
                subtitleCmd << " -map 0:" << streamIndex << " -c:s " << codecOption;
            }
        }
    }
    return subtitleCmd.str();
}


std::string FFmpegCommandBuilder::processAudioStreams(const std::vector<nlohmann::json>& streams) {
    std::ostringstream audioCmd;
    bool audioNeedConversion = std::any_of(streams.begin(), streams.end(),
        [this](const nlohmann::json& stream) {
            return stream["codec_type"] == "audio" && !isCompatibleAudio(stream);
        });

    audioCmd << (audioNeedConversion ? " -c:a aac" : " -c:a copy");
    return audioCmd.str();
}

std::string FFmpegCommandBuilder::processVideoStreams(const std::vector<nlohmann::json>& streams) {
    std::ostringstream videoCmd;

    for (const auto& stream : streams) {
        if (isCompatibleVideo(stream)) {
            videoCmd << generateEncoderOptions();
            break;
        }
    }

    return videoCmd.str();
}

std::string FFmpegCommandBuilder::generateVideoFilter() const {
    if (maxResolution > 0) {
        return "scale=-1:min(" + std::to_string(maxResolution) + R"(\,ih))";
    }
    return "";
}

std::string FFmpegCommandBuilder::generateStreamSelectors(
    const std::vector<int>& videoStreams,
    const std::vector<int>& audioStreams,
    const std::vector<int>& subtitleStreams,
    const std::vector<nlohmann::json>& streams) {
    std::ostringstream selectors;

    if (videoStreams.empty()) {
        selectors << generateSelectorForAllStreamsOfType("video", streams);
    } else {
        selectors << generateSelectorForStreamType(videoStreams, "v", streams);
    }

    if (audioStreams.empty()) {
        selectors << generateSelectorForAllStreamsOfType("audio", streams);
    } else {
        selectors << generateSelectorForStreamType(audioStreams, "a", streams);
    }

    if (subtitleStreams.empty()) {
        selectors << generateSelectorForAllStreamsOfType("subtitle", streams);
    } else {
        selectors << generateSelectorForStreamType(subtitleStreams, "s", streams);
    }

    return selectors.str();
}

bool FFmpegCommandBuilder::streamExists(const std::vector<nlohmann::json>& streams, const std::string& type, int index) {
    return std::any_of(streams.begin(), streams.end(), [type, index](const nlohmann::json& stream) {
        return stream["codec_type"] == type && stream["index"].get<int>() == index;
        });
}


std::string FFmpegCommandBuilder::generateSelectorForStreamType(
    const std::vector<int>& streams,
    const std::string& streamType,
    const std::vector<nlohmann::json>& allStreams) {
    std::ostringstream selector;
    for (int streamIndex : streams) {
        if (streamExists(allStreams, streamType, streamIndex)) {
            selector << " -map 0:" << streamType << ":" << streamIndex;
        }
    }
    return selector.str();
}
std::string FFmpegCommandBuilder::generateSelectorForAllStreamsOfType(
    const std::string& streamType,
    const std::vector<nlohmann::json>& allStreams) {
    std::ostringstream selector;
    for (const auto& stream : allStreams) {
        if (stream["codec_type"] == streamType) {
            if (streamType == "video" && !isCompatibleVideo(stream)) {
                continue; // Skip non-compatible video streams
            }
            int streamIndex = stream["index"].get<int>();
            selector << " -map 0:" << streamIndex;
            if (streamType == "subtitle") {
                std::string codecOption = isCompatibleSubtitle(stream) ? "copy" : "mov_text";
                selector << " -c:s " << codecOption;
            }
        }
    }
    return selector.str();
}


std::string FFmpegCommandBuilder::generateEncoderOptions() const {
    std::ostringstream encoderOptions;
    if (encoderType == EncoderType::GPU) {
        encoderOptions << " -c:v hevc_nvenc -preset slow -cq "<<cqValue;
    }
    else if (encoderType == EncoderType::CPU) {
        // libx265 settings for CPU encoding
        encoderOptions << " -c:v libx265"
            << " -cpu-used 4"
            << " -threads 16"
            << " -tile-columns 2
            << " -tile-rows 1"
            << " -row-mt 1"
            << " -speed 2"
            << " -auto-alt-ref 1"
            << " -lag-in-frames 25"
            << " -cq "<<cqValue;

    }
    return encoderOptions.str();
}

std::string FFmpegCommandBuilder::createOutputFileName() const {

    std::string outputDirectoryCopy = outputDirectory;
    if (outputDirectoryCopy.size() >= 4) {
        outputDirectoryCopy.erase(outputDirectoryCopy.size() - 4);
    }

    return outputDirectoryCopy + "_CQ" + std::to_string(cqValue) + extension;
}


std::string FFmpegCommandBuilder::buildCommand() {
    if (!ffprobe.analyze()) {
        std::cerr << "Failed to analyze file: " << inputFilePath << std::endl;
        return "";
    }
    auto streams = ffprobe.getStreams();

    std::ostringstream cmd;
    cmd << "ffmpeg -i \"" << inputFilePath << "\"";

    std::string videoFilter = generateVideoFilter();
    if (!videoFilter.empty()) {
        cmd << " -vf \"" << videoFilter << "\"";
    }

    cmd << generateStreamSelectors(videoStreams, audioStreams, subtitleStreams, streams);

    cmd << processAudioStreams(streams);
    cmd << processVideoStreams(streams);
    cmd << processSubtitleStreams(streams);

    if (!additionalFlags.empty()) {
        cmd << " " << additionalFlags;
    }

    cmd << " \"" << createOutputFileName() << "\"";

    return cmd.str();
}

