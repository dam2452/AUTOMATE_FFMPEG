#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <string>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

class FileProcessor {
public:
    FileProcessor(const std::string& sourceDir, const std::string& targetDir);
    void processFiles(std::function<void(const std::string&)> fileAction);

private:
    std::string sourceDirectory;
    std::string targetDirectory;
    void copyDirectory(const fs::path& source, const fs::path& target);
};

#endif // FILEPROCESSOR_H
