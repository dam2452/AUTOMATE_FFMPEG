#include "FileProcessor.h"
#include <filesystem>
#include <iostream>
#include "ProgressBar.h"

namespace fs = std::filesystem;

FileProcessor::FileProcessor(const std::string& sourceDir, const std::string& targetDir)
    : sourceDirectory(sourceDir), targetDirectory(targetDir) {}

void FileProcessor::processFiles(std::function<void(const std::string&, const std::string&)> fileAction) {
    fs::path sourcePath(sourceDirectory);
    fs::path targetPath(targetDirectory);

    int totalFiles = std::count_if(fs::recursive_directory_iterator(sourcePath),
        fs::recursive_directory_iterator{},
        [](const auto& entry) { return entry.is_regular_file(); });
    ProgressBar progressBar(totalFiles);
    int processedFiles = 0;

    for (const auto& entry : fs::recursive_directory_iterator(sourcePath)) {
        if (entry.is_regular_file()) {
            // Tworzymy �cie�k� docelow� zachowuj�c struktur� katalog�w
            fs::path relativePath = fs::relative(entry.path(), sourcePath);
            fs::path outputPath = targetPath / relativePath;

            // Tworzenie katalog�w w �cie�ce docelowej, je�li nie istniej�
            fs::create_directories(outputPath.parent_path());

            if (!fs::exists(outputPath)) {
                fileAction(entry.path().string(), outputPath.string());
                processedFiles++;
            }

            progressBar.update(processedFiles);
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
