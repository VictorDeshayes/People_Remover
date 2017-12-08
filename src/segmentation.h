#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "global.h"

class segmentation
{
private:
    cv::Mat crop_img;
    cv::Mat transformed_img;
public:
    segmentation(cv::Mat input_crop_img);
    cv::Mat process(bool auto_seg);
    cv::Mat watershed(bool auto_seg);
    cv::Mat active_snake(); //TODO remove or redo
    cv::Mat full_rect();
};

#endif // SEGMENTATION_H
