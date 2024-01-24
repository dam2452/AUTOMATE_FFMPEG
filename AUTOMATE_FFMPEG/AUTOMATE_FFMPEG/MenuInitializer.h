#pragma once
#include "menu.h"
#include "VideoProcessor.h"

class MenuInitializer {
public:
    static void initializeMenu(Menu& menu, VideoProcessor& processor);
};