#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <string>
#include <functional>
#include <filesystem>
#include "VideoProcessor.h"

namespace fs = std::filesystem;

class FileProcessor {
public:
    FileProcessor(const std::string& sourceDir, const std::string& targetDir);
    void processFiles(VideoProcessor& processor, std::function<void(VideoProcessor&, const std::string&, const std::string&)> fileAction);

private:
    std::string sourceDirectory;
    std::string targetDirectory;
    void copyDirectory(const fs::path& source, const fs::path& target);
};

#endif // FILEPROCESSOR_H
