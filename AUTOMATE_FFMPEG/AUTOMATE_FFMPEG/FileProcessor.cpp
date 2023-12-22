#include "FileProcessor.h"
#include <filesystem>
#include <iostream>
#include "ProgressBar.h"

namespace fs = std::filesystem;

FileProcessor::FileProcessor(const std::string& sourceDir, const std::string& targetDir)
    : sourceDirectory(sourceDir), targetDirectory(targetDir) {}

void FileProcessor::processFiles(std::function<void(const std::string&, const std::string&)> fileAction) {
    fs::path targetPath(targetDirectory);
    copyDirectory(sourceDirectory, targetPath);

    // Utwórz ProgressBar
    int totalFiles = std::count_if(fs::recursive_directory_iterator(targetPath),
        fs::recursive_directory_iterator{},
        [](const auto& entry) { return entry.is_regular_file(); });
    ProgressBar progressBar(totalFiles);
    int currentFile = 0;

    for (const auto& entry : fs::recursive_directory_iterator(targetPath)) {
        if (entry.is_regular_file()) {
            fs::path outputPath = targetPath / entry.path().filename();
            outputPath.replace_extension("_converted.mp4");
            if (!fs::exists(outputPath)) {  // SprawdŸ, czy plik wyjœciowy ju¿ istnieje
                fileAction(entry.path().string(), targetDirectory);
            }

            progressBar.update(++currentFile);  // Aktualizuj pasek postêpu
        }
    }

    progressBar.complete();
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
