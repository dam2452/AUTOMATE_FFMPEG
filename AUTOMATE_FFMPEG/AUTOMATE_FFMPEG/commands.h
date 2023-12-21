#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>

class Commands {
public:
    static std::string execute(const std::string& cmd);
    static std::string checkFFmpegVersion();
    static void installFFmpegWindows();
    static void installFFmpegLinux();
    static void manageFFmpegInstallation();
};

#endif 
