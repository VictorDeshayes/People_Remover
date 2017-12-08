#ifndef SELECTED_BOUNDING_BOX_H
#define SELECTED_BOUNDING_BOX_H

#include "simple_bounding_box.h"
#include "global.h"

#include <iostream>
#include <algorithm>

class selected_bounding_box
{
private:
    int x;
    int y;
    int w;
    int h;
    int arrow;
    bool is_set;
public:
    selected_bounding_box();
    void set(simple_bounding_box selected);
    void set_arrow(int key);
    bool selected_arrow();
    void move_arrow(int click_x, int click_y);
    void move_left(int click_x);
    void move_up(int click_y);
    void move_right(int click_x);
    void move_down(int click_y);
    void swap_x();
    void swap_y();
    void swap_arrow();
    bool remove_arrow();
    void clear();
    bool already_set();
    void draw(cv::Mat img);
    cv::Point pt1();
    cv::Point pt2();
    void print();
    int get_x();
    int get_y();
    int get_w();
    int get_h();
};

#endif // SELECTED_BOUNDING_BOX_H
