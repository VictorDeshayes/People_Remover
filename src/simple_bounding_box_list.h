#ifndef SIMPLE_BOUNDING_BOX_LIST_H
#define SIMPLE_BOUNDING_BOX_LIST_H

#include <list>

#include "simple_bounding_box.h"
#include "selected_bounding_box.h"

class simple_bounding_box_list
{
private:
    std::list<simple_bounding_box> bbs;
    bool empty;
public:
    simple_bounding_box_list();
    void add_bb(int new_x, int new_y, int new_w, int new_h);
    simple_bounding_box select_bb(int click_x, int click_y);
    simple_bounding_box first_bb();
    bool draw(cv::Mat img);
    void print();
};

#endif // SIMPLE_BOUNDING_BOX_LIST_H
