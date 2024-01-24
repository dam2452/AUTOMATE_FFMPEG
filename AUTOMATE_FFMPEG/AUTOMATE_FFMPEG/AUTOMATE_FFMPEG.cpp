#include "MenuInitializer.h"
#include "menu.h"
#include "VideoProcessor.h"

int main()
{
    VideoProcessor processor;
    Menu menu("AUTOMATE_FFMPEG");
    MenuInitializer::initializeMenu(menu, processor);
    menu.run();
    return 0;
}
