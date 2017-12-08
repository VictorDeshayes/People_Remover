#include "global.h"

namespace global
{
    // KEYS PRESSED
    int left = 81;
    int up = 82;
    int right = 83;
    int down = 84;
    int esc = 27;
    int suppr = 255;
    int enter = 13;
    int n = 'n';
    int q = 'q';
    int s = 's';
    int a = 'a';

    // OTHERS
    int click_x = -1;
    int click_y = -1;
    int width = -1;
    int height = -1;

    bool segmentation = true;

    void clear_click(){
    click_x = -1;
        click_y = -1;
    }
}
