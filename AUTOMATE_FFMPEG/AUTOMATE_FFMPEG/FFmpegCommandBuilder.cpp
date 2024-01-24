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
    const std::string codec = stream["codec_name"];
    return (codec == "aac" || codec == "eac3");
}

bool FFmpegCommandBuilder::isCompatibleSubtitle(const nlohmann::json& stream) {
    std::string codecName = stream["codec_name"];
    return (codecName == "mov_text" || codecName == "different_compatible_format");
}

bool FFmpegCommandBuilder::isCompatibleVideo(const nlohmann::json& stream) const {
    // Check if the stream is a video
    if (stream["codec_type"] != "video") {
        return false;
    }

    // Check if the stream has only one frame (typical for cover art)
    if (stream.contains("nb_frames") && stream["nb_frames"] == "1") {
        return false;
    }

    // Additional conditions to determine compatibility, e.g., checking size, metadata, etc.

    // If none of the above conditions are met, assume the video stream is compatible
    return true;
}

std::string FFmpegCommandBuilder::processSubtitleStreams(const std::vector<nlohmann::json>& streams) {
    std::ostringstream subtitleCmd;
    for (int streamIndex : subtitleStreams) {
        if (streamExists(streams, "subtitle", streamIndex)) {
            std::string codecOption = isCompatibleSubtitle(streams[streamIndex]) ? "copy" : "mov_text";
            subtitleCmd << " -map 0:s:" << streamIndex << " -c:s " << codecOption;
        }
    }
    return subtitleCmd.str();
}




std::string FFmpegCommandBuilder::processAudioStreams(const std::vector<nlohmann::json>& streams) {
    std::ostringstream audioCmd;
    bool audioNeedConversion = std::any_of(streams.begin(), streams.end(),
        [](const nlohmann::json& stream) {
            return stream["codec_type"] == "audio" &&
                !(stream["codec_name"] == "aac" || stream["codec_name"] == "eac3" ||
                    stream["codec_name"] == "dts" || stream["codec_name"] == "ac3");
        });

    audioCmd << (audioNeedConversion ? " -c:a aac" : " -c:a copy");
    return audioCmd.str();
}

std::string FFmpegCommandBuilder::processVideoStreams(const std::vector<nlohmann::json>& streams) {
    // Process video streams to generate video encoding options.
    std::ostringstream videoCmd;

    // Assuming we need options for the first compatible video stream only.
    for (const auto& stream : streams) {
        if (isCompatibleVideo(stream)) {
            videoCmd << generateEncoderOptions();
            break; // Exit loop after finding the first compatible stream.
        }
    }

    return videoCmd.str();
}

std::string FFmpegCommandBuilder::generateVideoFilter() const {
    // Generates a video filter string for scaling the video.
    // If maxResolution is set, the video is scaled to a maximum height of maxResolution,
    // while maintaining the aspect ratio.
    if (maxResolution > 0) {
        return "scale=-1:min(" + std::to_string(maxResolution) + R"(\,ih)";
    }
    return "";
}

std::string FFmpegCommandBuilder::generateStreamSelectors(
    const std::vector<int>& videoStreams,
    const std::vector<int>& audioStreams,
    const std::vector<int>& subtitleStreams,
    const std::vector<nlohmann::json>& streams) {
    std::ostringstream selectors;

    selectors << generateSelectorForStreamType(videoStreams, "v", streams);
    selectors << generateSelectorForStreamType(audioStreams, "a", streams);
    selectors << generateSelectorForStreamType(subtitleStreams, "s", streams);

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


std::string FFmpegCommandBuilder::generateEncoderOptions() const {
    std::ostringstream encoderOptions;
    if (encoderType == EncoderType::GPU) {
        encoderOptions << " -c:v hevc_nvenc -preset slow";
        // Additional GPU encoder options can be added here if needed.
    }
    else if (encoderType == EncoderType::CPU) {
        // libx265 settings for CPU encoding
        encoderOptions << " -c:v libx265"
            << " -cpu-used 4"      // Set CPU usage
            << " -threads 16"     // Number of threads to use
            << " -tile-columns 2" // Number of tile columns
            << " -tile-rows 1"    // Number of tile rows
            << " -row-mt 1"       // Enable row-based multithreading
            << " -speed 2"        // Speed setting
            << " -auto-alt-ref 1" // Enable automatic alternate reference frames
            << " -lag-in-frames 25"; // Number of frames to consider for lag
        // Additional CPU encoder options can be added here.
    }
    return encoderOptions.str();
}

std::string FFmpegCommandBuilder::createOutputFileName() const {

    std::string outputDirectoryCopy = outputDirectory;
    if (outputDirectoryCopy.size() >= 4) {
        outputDirectoryCopy.erase(outputDirectoryCopy.size() - 4);  // Removes the last three characters
    }

    return outputDirectoryCopy + "_CQ" + std::to_string(cqValue) + extension;
}


std::string FFmpegCommandBuilder::buildCommand() {

    // Analyze the media file to get stream information
    if (!ffprobe.analyze()) {
        // Handle analysis failure, e.g., by logging an error or throwing an exception
        std::cerr << "Failed to analyze file: " << inputFilePath << std::endl;
        return "";
    }
    auto streams = ffprobe.getStreams();

    std::ostringstream cmd;
    cmd << "ffmpeg -i \"" << inputFilePath << "\""; // Input file

    // Add selectors for video, audio, and subtitle streams
    cmd << generateStreamSelectors(videoStreams, audioStreams, subtitleStreams, streams);

    // Process audio, video, and subtitle streams
    cmd << processAudioStreams(streams);
    cmd << processVideoStreams(streams);
    cmd << processSubtitleStreams(streams);

    // Append additional flags if any
    if (!additionalFlags.empty()) {
        cmd << " " << additionalFlags;
    }

    // Specify the output file
    cmd << " \"" << createOutputFileName() << "\"";

    return cmd.str();
}



