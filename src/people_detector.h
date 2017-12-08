#ifndef PEOPLE_DETECTOR_H
#define PEOPLE_DETECTOR_H

#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

#include "simple_bounding_box_list.h"

class people_detector
{
public:
    people_detector();
    simple_bounding_box_list process(cv::Mat img);
};

#endif // PEOPLE_DETECTOR_H
