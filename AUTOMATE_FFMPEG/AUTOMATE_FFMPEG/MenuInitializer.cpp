#include "MenuInitializer.h"


void MenuInitializer::initializeMenu(Menu& menu, VideoProcessor& processor) {
    menu.addItem("Start processing", [&]() { processor.startProcessing(); });
    menu.addItem("Choose paths", [&]() { processor.choosePaths(); });
    menu.addItem("Select audio streams", [&]() { processor.selectAudioStreams(); });
    menu.addItem("Select subtitle streams", [&]() { processor.selectSubtitleStreams(); });
    menu.addItem("Select video streams", [&]() { processor.selectVideoStreams(); });
    menu.addItem("Select CQ", [&]() { processor.selectCQ(); });
    menu.addItem("Select resolution limit", [&]() { processor.selectResLimit(); });
    menu.addItem("Choose encoder type", [&]() { processor.chooseEncoderType(); });
    menu.addItem("Check settings", [&]() { processor.showSettings(); });
}
