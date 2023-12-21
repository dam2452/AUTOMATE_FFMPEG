#include "FileProcessor.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

FileProcessor::FileProcessor(const std::string& sourceDir, const std::string& targetDir)
    : sourceDirectory(sourceDir), targetDirectory(targetDir) {}

void FileProcessor::processFiles(std::function<void(const std::string&)> fileAction) {
    fs::path sourcePath(sourceDirectory);
    fs::path targetPath(targetDirectory);
    copyDirectory(sourcePath, targetPath);
    for (const auto& entry : fs::recursive_directory_iterator(targetPath)) {
        if (entry.is_regular_file()) {
            const auto& path = entry.path();
            if (path.extension() == ".mkv" || path.extension() == ".ts" ||
                path.extension() == ".mp4" || path.extension() == ".avi") {
                fileAction(path.string());
            }
        }
    }
}

void FileProcessor::copyDirectory(const fs::path& source, const fs::path& target) {
    try {
        fs::create_directories(target);
        for (const auto& entry : fs::directory_iterator(source)) {
            const auto& path = entry.path();
            auto targetPath = target / path.filename();
            if (fs::is_directory(path)) {
                copyDirectory(path, targetPath);
            }
            else {
                fs::copy_file(path, targetPath, fs::copy_options::overwrite_existing);
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << e.what() << '\n';
    }
}
