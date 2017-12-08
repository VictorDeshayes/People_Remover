#ifndef SIMPLE_BOUNDING_BOX_H
#define SIMPLE_BOUNDING_BOX_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

class simple_bounding_box
{
private:
    int x;
    int y;
    int w;
    int h;
public:
    simple_bounding_box(int new_x, int new_y, int new_w, int new_h);
    void draw(cv::Mat img);
    void print();
    int get_x();
    int get_y();
    int get_w();
    int get_h();
};

#endif // SIMPLE_BOUNDING_BOX_H
