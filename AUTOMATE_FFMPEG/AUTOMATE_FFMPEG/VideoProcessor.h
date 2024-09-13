#pragma once

#include <string>
#include <vector>
#include "FFmpegCommandBuilder.h"


class VideoProcessor {
public:
    VideoProcessor() : maxResolution(1080), cqValue(28), encoderType(EncoderType::GPU), sourcePath("XYZ"), destinationPath("XYZOUT"), additionalFlags("-loglevel quiet") {}

    //DEBUG , additionalFlags("-loglevel quiet")

    void choosePaths();
    void startProcessing();
    void selectVideoStreams();
    void selectAudioStreams();
    void selectSubtitleStreams();
    void selectCQ();
    void selectResLimit();
    void chooseEncoderType();
    void showSettings();


private:
    int maxResolution;
    int cqValue;
    EncoderType encoderType;
    std::vector<int> selectedVideoStreams;
    std::vector<int> selectedAudioStreams;
    std::vector<int> selectedSubtitleStreams;
    std::string additionalFlags; //DEBUG: -loglevel quiet   -b:v 1M -maxrate 1.2M -bufsize 2M
    std::string sourcePath;
    std::string destinationPath;

    void processVideoFile(const std::string& filePath, const std::string& outputDir);
    std::vector<int> parseStreamIndices(const std::string& input);
};
