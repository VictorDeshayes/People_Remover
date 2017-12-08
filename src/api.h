#ifndef api_H
#define api_H

#include "simple_bounding_box_list.h"
#include "segmentation.h"
#include "filler.h"
#include "people_detector.h"

class api
{
private:
    cv::Mat img;
    cv::Mat display_img;
    bool reload_display;
    simple_bounding_box_list simple_bb;
    selected_bounding_box selected_bb;
    cv::Point first_pt;
    bool new_bb;
public:
    api(cv::Mat input_img);
    void process_people_detection();
    void draw();
    cv::Mat display();
    void print();
    void add_simple_bb(int new_x, int new_y, int new_w, int new_h);
    void add_simple_bb_list(simple_bounding_box_list people_det);
    bool select_bb(int click_x, int click_y);
    bool select_first_bb();
    bool input(int click_x, int click_y, int key);
    bool input_click(int click_x, int click_y);
};

#endif // api_H
